#include "cpu.h"

void
CPU::next()
{
  iduInc(PC);
  read(PC);
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
  uint8 val1 = readRegister(AF, RegisterBits::High);
  uint8 val2 = readRegister(regS, regSB);
  uint8 carry = getFlag(FlagBits::Carry) ? 1 : 0;
  uint8 result = val1 + val2 + carry;
  setRegister(AF, result, RegisterBits::High);
  setFlag(FlagBits::Zero, result == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, ((val1 & 0x0F) + (val2 & 0x0F) + carry) > 0x0F);
  setFlag(FlagBits::Carry, (val1 + val2 + carry) > 0xFF);
  next();
}

void
CPU::adc_ar16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      uint16 tmp = ioData;
      adc_r8(tmp, RegisterBits::Low); // next is called in adc_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::adc_n8()
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      uint16 tmp = ioData;
      adc_r8(tmp, RegisterBits::Low); // next is called in adc_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::add_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val1 = readRegister(AF, RegisterBits::High);
  uint8 val2 = readRegister(regS, regSB);
  uint8 result = val1 + val2;
  setRegister(AF, result, RegisterBits::High);
  setFlag(FlagBits::Zero, result == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, ((val1 & 0x0F) + (val2 & 0x0F)) > 0x0F);
  setFlag(FlagBits::Carry, (val1 + val2) > 0xFF);
  next();
}

void
CPU::add_ar16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      uint16 tmp = ioData;
      add_r8(tmp, RegisterBits::Low); // next is called in add_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::add_n8()
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      uint16 tmp = ioData;
      add_r8(tmp, RegisterBits::Low); // next is called in add_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::cp_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val1 = readRegister(AF, RegisterBits::High);
  uint8 val2 = readRegister(regS, regSB);
  setFlag(FlagBits::Zero, val1 == val2);
  setFlag(FlagBits::Subtract, true);
  setFlag(FlagBits::HalfCarry, (val1 & 0x0F) < (val2 & 0x0F));
  setFlag(FlagBits::Carry, val1 < val2);
  next();
}

void
CPU::cp_ar16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      uint16 tmp = ioData;
      cp_r8(tmp, RegisterBits::Low); // next is called in cp_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::cp_n8()
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      uint16 tmp = ioData;
      cp_r8(tmp, RegisterBits::Low); // next is called in cp_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::dec_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val = readRegister(regS, regSB);
  uint8 result = val - 1;
  setRegister(regS, result, regSB);
  setFlag(FlagBits::Zero, result == 0);
  setFlag(FlagBits::Subtract, true);
  setFlag(FlagBits::HalfCarry, (val & 0x0F) == 0);
  next();
}

void
CPU::dec_ar16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      write(regS, ioData - 1);
      setFlag(FlagBits::Zero, ioData == 1);
      setFlag(FlagBits::Subtract, true);
      setFlag(FlagBits::HalfCarry, (ioData & 0x0F) == 0);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::inc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val = readRegister(regS, regSB);
  uint8 result = val + 1;
  setRegister(regS, result, regSB);
  setFlag(FlagBits::Zero, result == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, (val & 0x0F) == 0x0F);
  next();
}

void
CPU::inc_ar16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      write(regS, ioData + 1);
      setFlag(FlagBits::Zero, ioData == 0xFF);
      setFlag(FlagBits::Subtract, true);
      setFlag(FlagBits::HalfCarry, (ioData & 0x0F) == 0x0F);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::sbc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val1 = readRegister(AF, RegisterBits::High);
  uint8 val2 = readRegister(regS, regSB);
  uint8 carry = getFlag(FlagBits::Carry) ? 1 : 0;
  uint8 result = val1 - val2 - carry;
  setRegister(AF, result, RegisterBits::High);
  setFlag(FlagBits::Zero, result == 0);
  setFlag(FlagBits::Subtract, true);
  setFlag(FlagBits::HalfCarry, (val1 & 0x0F) < ((val2 & 0x0F) + carry));
  setFlag(FlagBits::Carry, val1 < (val2 + carry));
  next();
}

void
CPU::sbc_ar16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      uint16 tmp = ioData;
      sbc_r8(tmp, RegisterBits::Low); // next is called in sbc_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::sbc_n8()
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      uint16 tmp = ioData;
      sbc_r8(tmp, RegisterBits::Low); // next is called in sbc_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::sub_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val1 = readRegister(AF, RegisterBits::High);
  uint8 val2 = readRegister(regS, regSB);
  uint8 result = val1 - val2;
  setRegister(AF, result, RegisterBits::High);
  setFlag(FlagBits::Zero, result == 0);
  setFlag(FlagBits::Subtract, true);
  setFlag(FlagBits::HalfCarry, (val1 & 0x0F) < (val2 & 0x0F));
  setFlag(FlagBits::Carry, val1 < val2);
  next();
}

void
CPU::sub_ar16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      uint16 tmp = ioData;
      sub_r8(tmp, RegisterBits::Low); // next is called in sub_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::sub_n8()
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      uint16 tmp = ioData;
      sub_r8(tmp, RegisterBits::Low); // next is called in sub_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::add_r16_r16(uint16& regD, uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      uint16 val1 = readRegister(regD, RegisterBits::Full);
      uint16 val2 = readRegister(regS, RegisterBits::Full);
      uint16 result = val1 + val2;
      setRegister(regD, result, RegisterBits::Full);
      setFlag(FlagBits::Zero, result == 0);
      setFlag(FlagBits::HalfCarry, ((val1 & 0x0FFF) + (val2 & 0x0FFF)) > 0x0FFF);
      setFlag(FlagBits::Carry, (val1 + val2) > 0xFFFF);
      instruction_cycles++;
      break;
    case 1:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::dec_r16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      iduDec(regS);
      instruction_cycles++;
      break;
    case 1:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::inc_r16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      iduInc(regS);
      instruction_cycles++;
      break;
    case 1:
      instruction_cycles = 0;
      next();
      break;
  }
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
