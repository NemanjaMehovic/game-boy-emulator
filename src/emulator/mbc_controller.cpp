#include "mbc_controller.h"

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

NoMBC_Handler::NoMBC_Handler(uint8* data, header* header)
  : MBC_Handler(data, header)
{
  if (m_header->ram_size > 0) {
    m_ram =
      std::make_unique<uint8[]>(RAM_SIZES.find(m_header->ram_size)->second);
  }
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
