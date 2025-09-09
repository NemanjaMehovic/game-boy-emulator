#include "cartridge.h"

#include <cstring>
#include <fstream>

#include "mbc_controller.h"

Cartridge::Cartridge(std::string location)
  : m_cartridge_location(location)
{
  log_info("Loading %s cartridge", m_cartridge_location.c_str());
  // Open cartridge
  std::ifstream ifs(m_cartridge_location, std::ifstream::binary);
  std::filebuf* pbuf = ifs.rdbuf();
  // get cartridge size
  std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
  pbuf->pubseekpos(0, ifs.in);
  m_data = std::make_unique<uint8[]>(size);
  // read data
  pbuf->sgetn(reinterpret_cast<char*>(m_data.get()), size);
  ifs.close();
  // set values
  m_cartridge_header = reinterpret_cast<header*>(m_data.get() + 0x100);
  // last byte also used to indicate if it supports gmb colour
  m_cartridge_header->title[15] = 0;
  m_cartridge_size = size;

  m_valid = checkData();
  m_valid = true;
  // if (!m_valid) {
  //   log_error("Loaded cartridge isn't valid");
  //   return;
  // }

  m_mbc_handler = MBC_Handler::CreateHandler(this);

  if (m_mbc_handler == nullptr) {
    log_error("Failed to create an MBC handler");
    m_valid = false;
    return;
  }

  std::string str = "Finished loading the cartridge:\n" + getDebugMsg();
  log_info("%s", str.c_str());
}

// needed because of fwd declaration of MBC_Handler
Cartridge::~Cartridge() = default;

std::string
Cartridge::getTitle()
{
  std::string str;
  str = m_cartridge_header->title;
  return str;
}

std::string
Cartridge::getTypeName()
{
  std::string str = "Unknown";
  auto itr = ROM_TYPES.find(m_cartridge_header->type);
  if (itr != ROM_TYPES.cend()) {
    str = itr->second;
  }
  return str;
}

std::string
Cartridge::getLicName()
{
  std::string str = "Unknown";
  if (m_cartridge_header->lic_code == 0x33) {
    auto itr = LIC_CODE.find(m_cartridge_header->lic_code_new);
    str = itr != LIC_CODE.cend() ? itr->second : "Unknown";
  } else {
    auto itr = OLD_LIC_CODE.find(m_cartridge_header->lic_code);
    str = itr != OLD_LIC_CODE.cend() ? itr->second : "Unknown";
  }
  return str;
}

void
Cartridge::write(uint16 address, uint8 val)
{
  m_mbc_handler->write(address, val);
}

uint8
Cartridge::read(uint16 address)
{
  return m_mbc_handler->read(address);
}

bool
Cartridge::isValidCartridge()
{
  return m_valid;
}

std::string
Cartridge::getDebugMsg()
{
  std::string str = "  Game title: " + getTitle() + "\n";
  str += "  Type: " + getTypeName() + "\n";
  str += "  LIC Code: " + getLicName() + "\n";
  str += "  ROM Size: " + std::to_string(32 << m_cartridge_header->rom_size) +
         " KB\n";
  str += "  RAM type: " + std::to_string(m_cartridge_header->ram_size) + "\n";
  str += "  ROM Version: " + std::to_string(m_cartridge_header->version);
  return str;
}

bool
Cartridge::checkData()
{
  bool flag = true;
  uint8_t checksum = 0;

  if (m_cartridge_header->ram_size != 0 &&
      RAM_SIZES.find(m_cartridge_header->ram_size) == RAM_SIZES.cend()) {
    log_error("Unknown ram size 0x%X", m_cartridge_header->ram_size);
    flag = false;
  }

  for (uint16_t address = 0x0134; address <= 0x014C; address++) {
    checksum = checksum - m_data[address] - 1;
  }

  if (m_cartridge_header->checksum != checksum) {
    log_error(
      "Checksums do not match %d is in the header and %d was calculated",
      m_cartridge_header->checksum,
      checksum);
    flag = false;
  }

  if (std::memcmp(
        NINTENDO_LOGO, m_cartridge_header->logo, sizeof(NINTENDO_LOGO)) != 0) {
    log_error("Nintendo logos do not match");
    flag = false;
  }
  return flag;
}
