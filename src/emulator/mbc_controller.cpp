#include "mbc_controller.h"

#include <cstring>

std::unique_ptr<MBC_Handler>
MBC_Handler::CreateHandler(Cartridge* cartridge)
{
  uint8* data = cartridge->m_data.get();
  header* cartridge_header = cartridge->m_cartridge_header;
  std::unique_ptr<MBC_Handler> handler = nullptr;
  switch (cartridge_header->type) {
    case NoMBC:
      handler = std::make_unique<NoMBC_Handler>(data, cartridge_header);
      log_info("Created a NoMbc handler");
      break;
    case MBC1:
    case MBC1Ram:
    case MBC1RamB:
      handler = std::make_unique<MBC1_Handler>(data, cartridge_header);
      log_info("Created an MBC1 handler");
      break;
    case MBC2:
    case MBC2B:
      handler = std::make_unique<MBC2_Handler>(data, cartridge_header);
      log_info("Created an MBC2 handler");
      break;
    default:
      log_error("Attempting to create an unsupported MBC handler of type 0x%X",
                cartridge_header->type);
      break;
  }
  return handler;
}

MBC_Handler::MBC_Handler(uint8* data, header* header)
  : m_data(data)
  , m_header(header)
{
  m_has_battery = HAS_BATTERY.find(m_header->type) != HAS_BATTERY.cend();
  if (m_header->ram_size > 0) {
    m_ram =
      std::make_unique<uint8[]>(RAM_SIZES.find(m_header->ram_size)->second);
  }
  if (m_has_battery && m_ram) {
    log_info("Loading from a save file");
    // TODO implement restore from save
  }
}

MBC_Handler::~MBC_Handler()
{
  if (m_has_battery && m_ram) {
    log_info("Creating a save file");
    // TODO implement save
  }
}

void
MBC_Handler::write(uint16 address, uint8 val)
{
  if (address < 0x8000)
    write_rom(address, val);
  else if (address >= 0xA000 && address <= 0xBFFF)
    write_ram(address, val);
  else
    log_error("Called MBC write for address 0x%X", address);
}

uint8
MBC_Handler::read(uint16 address)
{
  if (address < 0x8000)
    return read_rom(address);
  else if (address >= 0xA000 && address <= 0xBFFF)
    return read_ram(address);

  log_error("Called MBC read for address 0x%X", address);
  return 0xFF;
}

void
NoMBC_Handler::write_rom(uint16 address, uint8 val)
{
  // Shouldn't do anything
  log_error("Called write to rom for NoMBC");
}

void
NoMBC_Handler::write_ram(uint16 address, uint8 val)
{
  if (!m_ram) {
    log_error("Called write to ram for NoMBC that has no ram");
    return;
  }
  m_ram[address] = val;
}

uint8
NoMBC_Handler::read_rom(uint16 address)
{
  return m_data[address];
}

uint8
NoMBC_Handler::read_ram(uint16 address)
{
  if (!m_ram) {
    log_error("Called read from ram for NoMBC that has no ram");
    return 0xFF;
  }
  return m_ram[address];
}

MBC1_Handler::MBC1_Handler(uint8* data, header* header)
  : MBC_Handler(data, header)
{
  m_is_mbc1m = checkIsMBC1M();
  if (m_is_mbc1m) {
    log_info("MBC1M detected");
  }
}

bool
MBC1_Handler::checkIsMBC1M()
{
  // all known mbc1m are 1MiB
  if (((32 * 1024) << m_header->rom_size) != 0x100000) {
    return false;
  }
  // checking  0x00104, 0x40104, 0x80104, and 0xC0104 for multiple logos
  uint8 logos = 0;
  for (uint8_t i = 0; i < 4; i++) {
    header* tmp_header =
      reinterpret_cast<header*>(m_data + (0x40000 * i) + 0x100);
    logos +=
      std::memcmp(NINTENDO_LOGO, tmp_header->logo, sizeof(NINTENDO_LOGO)) == 0;
  }
  return logos > 1;
}

void
MBC1_Handler::write_rom(uint16 address, uint8 val)
{
  if (address < 0x2000) {
    m_enabled_ram = mask_n_bits(4, val) == 0xA;
    log_info("Set m_enable_ram to %d", m_enabled_ram);
    return;
  }
  if (address < 0x4000) {
    uint8 tmpVal = mask_n_bits(m_is_mbc1m ? 4 : 5, val);
    m_low_banking_bits = tmpVal != 0 ? tmpVal : 1;
    log_info("Set m_low_banking_bits to 0x%X", m_low_banking_bits);
    return;
  }
  if (address < 0x6000) {
    m_high_banking_bits = mask_n_bits(2, val);
    log_info("Set m_high_banking_bits to 0x%X", m_high_banking_bits);
    return;
  }
  m_mode = val & 1;
  log_info("Set m_mode to %d", m_mode);
}

void
MBC1_Handler::write_ram(uint16 address, uint8 val)
{
  if (!m_ram || !m_enabled_ram) {
    log_error("Called write to ram for MBC1 that has no ram/hasn't enabled it");
    return;
  }
  uint32 tmp_address = mask_n_bits(13, address);
  if (m_mode == 1) {
    tmp_address = (m_high_banking_bits << 13) | tmp_address;
  }
  m_ram[tmp_address] = val;
}

uint8
MBC1_Handler::read_rom(uint16 address)
{
  uint32 tmp_address = 0;
  if (address >= 0x4000) {
    tmp_address =
      m_high_banking_bits << (m_is_mbc1m ? 4 : 5) | m_low_banking_bits;
  } else if (m_mode == 1) {
    tmp_address = m_high_banking_bits << (m_is_mbc1m ? 4 : 5);
  }
  tmp_address = (tmp_address << 14) | mask_n_bits(14, address);
  return m_data[tmp_address];
}

uint8
MBC1_Handler::read_ram(uint16 address)
{
  if (!m_ram || !m_enabled_ram) {
    log_error(
      "Called read from ram for MBC1 that has no ram/hasn't enabled it");
    return 0xFF;
  }
  uint32 tmp_address = mask_n_bits(13, address);
  if (m_mode == 1) {
    tmp_address = (m_high_banking_bits << 13) | tmp_address;
  }
  return m_ram[tmp_address];
}

MBC2_Handler::MBC2_Handler(uint8* data, header* header)
  : MBC_Handler(data, header)
{
  // MBC2 always has a fixed ram size of 512 half bytes
  m_ram = std::make_unique<uint8[]>(512);
  if (m_has_battery && m_ram) {
    log_info("Loading from a save file");
    // TODO implement restore from save
  }
}

void
MBC2_Handler::write_rom(uint16 address, uint8 val)
{
  if ((address & (1 << 8)) != 0) {
    uint8 tmpVal = mask_n_bits(4, val);
    m_banking_bits = tmpVal != 0 ? tmpVal : 1;
    log_info("Set m_banking_bits to 0x%X", m_banking_bits);
  } else {
    m_enabled_ram = mask_n_bits(4, val) == 0xA;
    log_info("Set m_enable_ram to %d", m_enabled_ram);
  }
}

void
MBC2_Handler::write_ram(uint16 address, uint8 val)
{
  if (!m_enabled_ram) {
    log_error("Called write to ram for MBC2 that hasn't enabled it");
    return;
  }
  uint32 tmp_address = mask_n_bits(9, address);
  m_ram[tmp_address] = val;
}

uint8
MBC2_Handler::read_rom(uint16 address)
{
  uint16 tmp_address = address;
  if (address >= 0x4000) {
    tmp_address = (m_banking_bits << 14) | mask_n_bits(14, address);
  }
  return m_data[tmp_address];
}

uint8
MBC2_Handler::read_ram(uint16 address)
{
  if (!m_ram || !m_enabled_ram) {
    log_error("Called read from ram for MBC2 that hasn't enabled it");
    return 0xFF;
  }
  return m_ram[mask_n_bits(9, address)];
}
