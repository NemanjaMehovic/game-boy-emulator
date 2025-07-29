#include "emulator.h"

Emulator::Emulator(std::string file)
{
  log_info("Creating emulator with %s", file.c_str());
  m_cartridge = std::make_unique<Cartridge>(file);

  if (!m_cartridge->isValidCartridge()) {
    log_error("Failed to create a cartridge with %s", file.c_str());
  }
  m_cpu = std::make_unique<CPU>();
}

bool
Emulator::isValid()
{
  return m_cartridge->isValidCartridge();
}
