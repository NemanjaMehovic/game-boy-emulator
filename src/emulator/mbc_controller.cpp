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

NoMBC_Handler::NoMBC_Handler(uint8* data, header* header)
  : MBC_Handler(data, header)
{
}

void
NoMBC_Handler::write(uint16 address, uint8 val)
{
  log_error("Not yet implemented");
}

uint8
NoMBC_Handler::read(uint16 address)
{
  log_error("Not yet implemented");
  return 0xFF;
}

void
NoMBC_Handler::write_rom(uint16 address, uint8 val)
{
  log_error("Not yet implemented");
}

void
NoMBC_Handler::write_ram(uint16 address, uint8 val)
{
  log_error("Not yet implemented");
}

uint8
NoMBC_Handler::read_rom(uint16 address)
{
  log_error("Not yet implemented");
  return 0xFF;
}

uint8
NoMBC_Handler::read_ram(uint16 address)
{
  log_error("Not yet implemented");
  return 0xFF;
}
