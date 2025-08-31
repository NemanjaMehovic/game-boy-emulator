#ifndef EMULATOR_H
#define EMULATOR_H

#include <memory>
#include <string>

#include "cartridge.h"
#include "cpu.h"
#include "mmu.h"
#include "ppu.h"
#include "timer.h"

class Emulator
{
public:
  Emulator(std::string file);

  bool isValid();

private:
  std::unique_ptr<Cartridge> m_cartridge;
  std::unique_ptr<CPU> m_cpu;
  std::unique_ptr<PPU> m_ppu;
  std::unique_ptr<Timer> m_timer;
  std::unique_ptr<MMU> m_mmu;
};

#endif // EMULATOR_H
