#ifndef EMULATOR_H
#define EMULATOR_H

#include <memory>
#include <string>
#include <thread>

#include "apu.h"
#include "cartridge.h"
#include "cpu.h"
#include "mmu.h"
#include "ppu.h"
#include "timer.h"

class Emulator
{
public:
  Emulator(std::string file);
  ~Emulator();

  void cycleFrame();
  void run();
  bool isValid();
  void mainLoop();
  std::thread* m_gameThread = nullptr;

private:
  std::unique_ptr<Cartridge> m_cartridge;
  std::unique_ptr<CPU> m_cpu;
  std::unique_ptr<PPU> m_ppu;
  std::unique_ptr<APU> m_apu;
  std::unique_ptr<Timer> m_timer;
  std::unique_ptr<MMU> m_mmu;
  uint64 m_Tcycles = 0;
};

#endif // EMULATOR_H
