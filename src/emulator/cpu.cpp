#include "cpu.h"
#include "common.h"

CPU::CPU()
{
  initialize();
}

void
CPU::initialize()
{
  AF = 0x01B0;
  BC = 0x0013;
  DE = 0x00D8;
  HL = 0x014D;
  SP = 0xFFFE;
  PC = 0x0100;

  instruction_cycles = 0;
  use_prefix_instruction = false;
}

void
CPU::cycle()
{
  if(instruction_cycles == 0) {
    if (bad_opcodes.find(ioData) != bad_opcodes.end()) {
      log_error("Bad opcode: 0x%02X", ioData);
      abort();
    }
    current_instruction = opcode_map.at(ioData);
  }
  current_instruction();
}

void
CPU::iduInc(uint16& reg, uint16 value)
{
  reg += value;
  // need to handle OAM Bug
}

void
CPU::iduDec(uint16& reg, uint16 value)
{
  reg -= value;
  // need to handle OAM Bug
}

void
CPU::setRegister(uint16& reg, uint16 val, RegisterBits register_bits)
{
  switch (register_bits) {
    case RegisterBits::High:
      reg = (reg & 0x00FF) | (val << 8);
      break;
    case RegisterBits::Low:
      if (&reg == &AF) {
        // special handling of F register
        // lowest 4 bits don't exist
        reg = (reg & 0xFF00) | (val & 0x00F0);
      } else {
        reg = (reg & 0xFF00) | (val & 0x00FF);
      }
      break;
    case RegisterBits::Full:
      reg = val;
      break;
  }
}

uint16
CPU::readRegister(uint16& reg, RegisterBits register_bits)
{
  switch (register_bits) {
    case RegisterBits::High:
      return (reg >> 8) & 0x00FF;
    case RegisterBits::Low:
      return reg & 0x00FF;
    case RegisterBits::Full:
      return reg;
  }
}

void
CPU::setFlag(FlagBits flag, bool value)
{
  if (value) {
    AF |= static_cast<uint16>(flag);
  } else {
    AF &= ~static_cast<uint16>(flag);
  }
}

bool CPU::getFlag(FlagBits flag)
{
  return (AF & static_cast<uint16>(flag)) != 0;
}
