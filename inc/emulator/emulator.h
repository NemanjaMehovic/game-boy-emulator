#ifndef EMULATOR_H
#define EMULATOR_H

#include <memory>
#include <string>

#include "cartridge.h"

class Emulator
{
public:
  Emulator(std::string file);

  bool isValid();

private:
  std::unique_ptr<Cartridge> m_cartridge;
};

#endif // EMULATOR_H
