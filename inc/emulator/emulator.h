#ifndef EMULATOR_H
#define EMULATOR_H

#include <memory>
#include <string>

#include "cartridge.h"
#include "cpu.h"

class Emulator
{
public:
  Emulator(std::string file);

  bool isValid();

private:
  std::unique_ptr<Cartridge> m_cartridge;
  std::unique_ptr<CPU> m_cpu;
};

#endif // EMULATOR_H
