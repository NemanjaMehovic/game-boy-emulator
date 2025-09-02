#include "emulator.h"

Emulator::Emulator(std::string file)
{
  log_info("Creating emulator with %s", file.c_str());
  m_cartridge = std::make_unique<Cartridge>(file);

  if (!m_cartridge->isValidCartridge()) {
    log_error("Failed to create a cartridge with %s", file.c_str());
  }
  m_cpu = std::make_unique<CPU>();
  m_ppu = std::make_unique<PPU>();
  m_timer = std::make_unique<Timer>();
  m_mmu = std::make_unique<MMU>(m_cpu.get(), m_cartridge.get(), m_ppu.get(), m_timer.get());
  m_cpu->setMMU(m_mmu.get());
  m_ppu->setMMU(m_mmu.get());
  m_timer->setMMU(m_mmu.get());
}

bool
Emulator::isValid()
{
  return m_cartridge->isValidCartridge();
}
