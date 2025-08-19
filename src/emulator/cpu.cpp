#include "cpu.h"

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
CPU::next()
{
  iduInc(PC);
  read(PC);
}

void
CPU::setRegister(uint16& reg, uint16 val, RegisterBits register_bits)
{
  switch (register_bits) {
    case RegisterBits::High:
      reg = (reg & 0x00FF) | (val << 8);
      break;
    case RegisterBits::Low: 
      reg = (reg & 0xFF00) | (val & 0x00FF);
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

// load instructions
void
CPU::ld_r8_r8(uint16& regD,
              CPU::RegisterBits regDB,
              uint16& regS,
              CPU::RegisterBits regSB)
{
  uint16 sourceValue = readRegister(regS, regSB);
  setRegister(regD, sourceValue, regDB);
  next();
}

void
CPU::ld_r8_n8(uint16& regD, CPU::RegisterBits regB)
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      setRegister(regD, ioData, regB);
      next();
      instruction_cycles = 0;
      break;
  }
}

void
CPU::ld_r16_n16(uint16& regD)
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      lowByte = ioData;
      instruction_cycles++;
      next();
      break;
    case 2:
      setRegister(regD, (ioData << 8) | lowByte, RegisterBits::Full);
      next();
      instruction_cycles = 0;
      break;
  }
}

void
CPU::ld_ar16_r8(uint16& regD, uint16& regS, CPU::RegisterBits regSB)
{
  switch (instruction_cycles) {
    case 0:
      write(regD, (uint8) readRegister(regS, regSB));
      instruction_cycles++;
      break;
    case 1:
      next();
      instruction_cycles = 0;
      break;
  }
}

void
CPU::ld_ar16_n8(uint16& regD)
{
  switch (instruction_cycles) {
    case 0 :
      instruction_cycles++;
      next();
      break;
    case 1:
      write(regD, ioData);
      instruction_cycles++;
      break;
    case 2:
      next();
      instruction_cycles = 0;
      break;
  }
}

void
CPU::ld_r8_ar16(uint16& regD, CPU::RegisterBits regDB, uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      setRegister(regD, ioData, regDB);
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ld_a16_r8(uint16& regS, CPU::RegisterBits regSB)
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      lowByte = ioData;
      instruction_cycles++;
      next();
      break;
    case 2:
      write((ioData << 8) | lowByte, (uint8) readRegister(regS, regSB));
      instruction_cycles ++;
      break;
    case 3:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ldh_a8_r8(uint16& regS, CPU::RegisterBits regSB)
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      write(0xFF00 + ioData, (uint8) readRegister(regS, regSB));
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ldh_ar8_r8(uint16& regD,
                CPU::RegisterBits regDB,
                uint16& regS,
                CPU::RegisterBits regSB)
{
  switch (instruction_cycles) {
    case 0:
      write(0xFF00 + readRegister(regD, regDB), (uint8) readRegister(regS, regSB));
      instruction_cycles++;
      break;
    case 1:
      instruction_cycles=0;
      next();
      break;
  }
}

void
CPU::ld_r8_a16(uint16& regD, CPU::RegisterBits regDB)
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      lowByte = ioData;
      instruction_cycles++;
      next();
      break;
    case 2:
      read((ioData << 8) | lowByte);
      instruction_cycles++;
      break;
    case 3:
      setRegister(regD, ioData, regDB);
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ldh_r8_a8(uint16& regD, CPU::RegisterBits regDB)
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      read(0xFF00 + ioData);
      instruction_cycles++;
      break;
    case 2:
      setRegister(regD, ioData, regDB);
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ldh_r8_ar8(uint16& regD,
                CPU::RegisterBits regDB,
                uint16& regS,
                CPU::RegisterBits regSB)
{
  switch (instruction_cycles) {
    case 0:
      read(0xFF00 + readRegister(regS, regSB));
      instruction_cycles++;
      break;
    case 1:
      setRegister(regD, ioData, regDB);
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ld_ar16i_r8(uint16& regD, uint16& regS, CPU::RegisterBits regSB)
{
  switch (instruction_cycles) {
    case 0:
      write(regD, (uint8) readRegister(regS, regSB));
      iduInc(regD);
      instruction_cycles++;
      break;
    case 1:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ld_ar16d_r8(uint16& regD, uint16& regS, CPU::RegisterBits regSB)
{
  switch (instruction_cycles) {
    case 0:
      write(regD, (uint8) readRegister(regS, regSB));
      iduDec(regD);
      instruction_cycles++;
      break;
    case 1:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ld_r8_ar16i(uint16& regD, CPU::RegisterBits regDB, uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      iduInc(regS);
      instruction_cycles++;
      break;
    case 1:
      setRegister(regD, ioData, regDB);
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ld_r8_ar16d(uint16& regD, CPU::RegisterBits regDB, uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      iduDec(regS);
      instruction_cycles++;
      break;
    case 1:
      setRegister(regD, ioData, regDB);
      instruction_cycles = 0;
      next();
      break;
  }
}

// arithmetic instructions
void
CPU::adc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::adc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::adc_n8()
{
  // TODO
}

void
CPU::add_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::add_ar16(uint16& regS)
{
  // TODO
}

void
CPU::add_n8()
{
  // TODO
}

void
CPU::cp_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::cp_ar16(uint16& regS)
{
  // TODO
}

void
CPU::cp_n8()
{
  // TODO
}

void
CPU::dec_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::dec_ar16(uint16& regS)
{
  // TODO
}

void
CPU::inc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::inc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::sbc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::sbc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::sbc_n8()
{
  // TODO
}

void
CPU::sub_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::sub_ar16(uint16& regS)
{
  // TODO
}

void
CPU::sub_n8()
{
  // TODO
}

void
CPU::add_r16_r16(uint16& regD, uint16& regS)
{
  // TODO
}

void
CPU::dec_r16(uint16& regS)
{
  // TODO
}

void
CPU::inc_r16(uint16& regS)
{
  // TODO
}

// bitwise logic instructions
void
CPU::and_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::and_ar16(uint16& regS)
{
  // TODO
}

void
CPU::and_n8()
{
  // TODO
}

void
CPU::cpl()
{
  // TODO
}

void
CPU::or_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::or_ar16(uint16& regS)
{
  // TODO
}

void
CPU::or_n8()
{
  // TODO
}

void
CPU::xor_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::xor_ar16(uint16& regS)
{
  // TODO
}

void
CPU::xor_n8()
{
  // TODO
}

// bit manipulation instructions
void
CPU::bit_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::bit_ar16(uint16& regS)
{
  // TODO
}

void
CPU::res_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::res_ar16(uint16& regS)
{
  // TODO
}

void
CPU::set_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::set_ar16(uint16& regS)
{
  // TODO
}

// bit shift instructions
void
CPU::rl_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::rl_ar16(uint16& regS)
{
  // TODO
}

void
CPU::rla()
{
  // TODO
}

void
CPU::rlc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::rlc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::rlca()
{
  // TODO
}

void
CPU::rr_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::rr_ar16(uint16& regS)
{
  // TODO
}

void
CPU::rra()
{
  // TODO
}

void
CPU::rrc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::rrc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::rrca()
{
  // TODO
}

void
CPU::sla_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::sla_ar16(uint16& regS)
{
  // TODO
}

void
CPU::sra_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::sra_ar16(uint16& regS)
{
  // TODO
}

void
CPU::srl_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::srl_ar16(uint16& regS)
{
  // TODO
}

void
CPU::swap_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::swap_ar16(uint16& regS)
{
  // TODO
}

// jumps and subroutine instructions
void
CPU::call_a16()
{
  // TODO
}

void
CPU::call_cc(Condition flag)
{
  // TODO
}

void
CPU::jp_r16(uint16& regS)
{
  // TODO
}

void
CPU::jp_a16()
{
  // TODO
}

void
CPU::jp_cc_a16(Condition flag)
{
  // TODO
}

void
CPU::jr_a8()
{
  // TODO
}

void
CPU::jr_cc_a8(Condition flag)
{
  // TODO
}

void
CPU::ret_cc(Condition flag)
{
  // TODO
}

void
CPU::ret()
{
  // TODO
}

void
CPU::reti()
{
  // TODO
}

void
CPU::rst()
{
  // TODO
}

// carry flag instructions
void
CPU::ccf()
{
  // TODO
}

void
CPU::scf()
{
  // TODO
}

// stack manipulation instructions
void
CPU::add_sp_e8()
{
  // TODO
}

void
CPU::ld_a16_sp()
{
  // TODO
}

void
CPU::ld_hl_sp_e8()
{
  // TODO
}

void
CPU::ld_r16_r16(uint16& regD, uint16& regS)
{
  // TODO
}

void
CPU::pop_r16(uint16& regS)
{
  // TODO
}

void
CPU::push_r16(uint16& regS)
{
  // TODO
}

// miscellaneous instructions
void
CPU::di()
{
  // TODO
}

void
CPU::ei()
{
  // TODO
}

void
CPU::halt()
{
  // TODO
}

void
CPU::daa()
{
  // TODO
}

void
CPU::nop()
{
  next();
}

void
CPU::stop()
{
  // TODO
}

void
CPU::prefix_cb()
{
  // TODO
}
