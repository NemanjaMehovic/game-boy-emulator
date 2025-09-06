#include "cpu.h"

void
CPU::next()
{
  read(PC);
  iduInc(PC);
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
      write(regD, (uint8)readRegister(regS, regSB));
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
    case 0:
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
      write((ioData << 8) | lowByte, (uint8)readRegister(regS, regSB));
      instruction_cycles++;
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
      write(0xFF00 + ioData, (uint8)readRegister(regS, regSB));
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
      write(0xFF00 + readRegister(regD, regDB),
            (uint8)readRegister(regS, regSB));
      instruction_cycles++;
      break;
    case 1:
      instruction_cycles = 0;
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
      write(regD, (uint8)readRegister(regS, regSB));
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
      write(regD, (uint8)readRegister(regS, regSB));
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
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      adc_r8(tmp, RegisterBits::Low); // next is called in adc_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::adc_n8()
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      tmp = ioData;
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
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      add_r8(tmp, RegisterBits::Low); // next is called in add_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::add_n8()
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      tmp = ioData;
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
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      cp_r8(tmp, RegisterBits::Low); // next is called in cp_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::cp_n8()
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      tmp = ioData;
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
      setFlag(FlagBits::Zero, ioData == 1);
      setFlag(FlagBits::Subtract, true);
      setFlag(FlagBits::HalfCarry, (ioData & 0x0F) == 0);
      write(regS, ioData - 1);
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
      setFlag(FlagBits::Zero, ioData == 0xFF);
      setFlag(FlagBits::Subtract, true);
      setFlag(FlagBits::HalfCarry, (ioData & 0x0F) == 0x0F);
      write(regS, ioData + 1);
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
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      sbc_r8(tmp, RegisterBits::Low); // next is called in sbc_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::sbc_n8()
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      tmp = ioData;
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
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      sub_r8(tmp, RegisterBits::Low); // next is called in sub_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::sub_n8()
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      tmp = ioData;
      sub_r8(tmp, RegisterBits::Low); // next is called in sub_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::add_r16_r16(uint16& regD, uint16& regS)
{
  uint16 val1 = 0;
  uint16 val2 = 0;
  uint16 result = 0;
  switch (instruction_cycles) {
    case 0:
      val1 = readRegister(regD, RegisterBits::Full);
      val2 = readRegister(regS, RegisterBits::Full);
      result = val1 + val2;
      setRegister(regD, result, RegisterBits::Full);
      setFlag(FlagBits::Zero, result == 0);
      setFlag(FlagBits::HalfCarry,
              ((val1 & 0x0FFF) + (val2 & 0x0FFF)) > 0x0FFF);
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
  uint8 val1 = readRegister(AF, RegisterBits::High);
  uint8 val2 = readRegister(regS, regSB);
  uint8 result = val1 & val2;
  setRegister(AF, result, RegisterBits::High);
  setFlag(FlagBits::Zero, result == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, true);
  setFlag(FlagBits::Carry, false);
  next();
}

void
CPU::and_ar16(uint16& regS)
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      and_r8(tmp, RegisterBits::Low); // next is called in and_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::and_n8()
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      tmp = ioData;
      and_r8(tmp, RegisterBits::Low); // next is called in and_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::cpl()
{
  uint8 val = readRegister(AF, RegisterBits::High);
  val = ~val;
  setRegister(AF, val, RegisterBits::High);
  setFlag(FlagBits::Subtract, true);
  setFlag(FlagBits::HalfCarry, true);
  next();
}

void
CPU::or_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val1 = readRegister(AF, RegisterBits::High);
  uint8 val2 = readRegister(regS, regSB);
  uint8 result = val1 | val2;
  setRegister(AF, result, RegisterBits::High);
  setFlag(FlagBits::Zero, result == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, false);
  next();
}

void
CPU::or_ar16(uint16& regS)
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      or_r8(tmp, RegisterBits::Low); // next is called in or_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::or_n8()
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      tmp = ioData;
      or_r8(tmp, RegisterBits::Low); // next is called in or_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::xor_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val1 = readRegister(AF, RegisterBits::High);
  uint8 val2 = readRegister(regS, regSB);
  uint8 result = val1 ^ val2;
  setRegister(AF, result, RegisterBits::High);
  setFlag(FlagBits::Zero, result == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, false);
  next();
}

void
CPU::xor_ar16(uint16& regS)
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      xor_r8(tmp, RegisterBits::Low); // next is called in xor_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::xor_n8()
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      tmp = ioData;
      xor_r8(tmp, RegisterBits::Low); // next is called in xor_r8
      instruction_cycles = 0;
      break;
  }
}

// bit manipulation instructions
void
CPU::bit_r8(uint16& regS, CPU::RegisterBits regSB, uint8 bit)
{
  uint8 val = readRegister(regS, regSB);
  setFlag(FlagBits::Zero, (val & (1 << bit)) == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, true);
  next();
}

void
CPU::bit_ar16(uint16& regS, uint8 bit)
{
  uint16 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      bit_r8(tmp, RegisterBits::Low, bit); // next is called in bit_r8
      instruction_cycles = 0;
      break;
  }
}

void
CPU::res_r8(uint16& regS, CPU::RegisterBits regSB, uint8 bit)
{
  uint8 val = readRegister(regS, regSB);
  val &= ~(1 << bit);
  setRegister(regS, val, regSB);
  next();
}

void
CPU::res_ar16(uint16& regS, uint8 bit)
{
  uint8 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData & ~(1 << bit);
      write(regS, tmp);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::set_r8(uint16& regS, CPU::RegisterBits regSB, uint8 bit)
{
  uint8 val = readRegister(regS, regSB);
  val |= (1 << bit);
  setRegister(regS, val, regSB);
  next();
}

void
CPU::set_ar16(uint16& regS, uint8 bit)
{
  uint8 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData | (1 << bit);
      write(regS, tmp);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

// bit shift instructions
void
CPU::rl_r8(uint16& regS, CPU::RegisterBits regSB)
{
  bool carry = getFlag(FlagBits::Carry);
  uint8 val = readRegister(regS, regSB);
  bool newCarry = (val & 0x80) != 0;
  val = (val << 1) | (carry ? 1 : 0);
  setRegister(regS, val, regSB);
  setFlag(FlagBits::Zero, val == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::rl_ar16(uint16& regS)
{
  uint8 tmp = 0;
  bool carry = false;
  bool newCarry = false;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      carry = getFlag(FlagBits::Carry);
      newCarry = (tmp & 0x80) != 0;
      tmp = (tmp << 1) | (carry ? 1 : 0);
      write(regS, tmp);
      setFlag(FlagBits::Zero, tmp == 0);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, false);
      setFlag(FlagBits::Carry, newCarry);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::rla()
{
  bool carry = getFlag(FlagBits::Carry);
  uint8 val = readRegister(AF, RegisterBits::High);
  bool newCarry = (val & 0x80) != 0;
  val = (val << 1) | (carry ? 1 : 0);
  setRegister(AF, val, RegisterBits::High);
  setFlag(FlagBits::Zero, false);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::rlc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val = readRegister(regS, regSB);
  bool newCarry = (val & 0x80) != 0;
  val = (val << 1) | (newCarry ? 1 : 0);
  setRegister(regS, val, regSB);
  setFlag(FlagBits::Zero, val == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::rlc_ar16(uint16& regS)
{
  uint8 tmp = 0;
  bool newCarry = false;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      newCarry = (tmp & 0x80) != 0;
      tmp = (tmp << 1) | (newCarry ? 1 : 0);
      write(regS, tmp);
      setFlag(FlagBits::Zero, tmp == 0);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, false);
      setFlag(FlagBits::Carry, newCarry);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::rlca()
{
  uint8 val = readRegister(AF, RegisterBits::High);
  bool newCarry = (val & 0x80) != 0;
  val = (val << 1) | (newCarry ? 1 : 0);
  setRegister(AF, val, RegisterBits::High);
  setFlag(FlagBits::Zero, false);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::rr_r8(uint16& regS, CPU::RegisterBits regSB)
{
  bool carry = getFlag(FlagBits::Carry);
  uint8 val = readRegister(regS, regSB);
  bool newCarry = (val & 0x01) != 0;
  val = (val >> 1) | (carry ? 0x80 : 0);
  setRegister(regS, val, regSB);
  setFlag(FlagBits::Zero, val == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::rr_ar16(uint16& regS)
{
  uint8 tmp = 0;
  bool carry = false;
  bool newCarry = false;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      carry = getFlag(FlagBits::Carry);
      newCarry = (tmp & 0x01) != 0;
      tmp = (tmp >> 1) | (carry ? 0x80 : 0);
      write(regS, tmp);
      setFlag(FlagBits::Zero, tmp == 0);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, false);
      setFlag(FlagBits::Carry, newCarry);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::rra()
{
  uint8 val = readRegister(AF, RegisterBits::High);
  bool carry = getFlag(FlagBits::Carry);
  bool newCarry = (val & 0x01) != 0;
  val = (val >> 1) | (carry ? 0x80 : 0);
  setRegister(AF, val, RegisterBits::High);
  setFlag(FlagBits::Zero, false);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::rrc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val = readRegister(regS, regSB);
  bool newCarry = (val & 0x01) != 0;
  val = (val >> 1) | (newCarry ? 0x80 : 0);
  setRegister(regS, val, regSB);
  setFlag(FlagBits::Zero, val == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::rrc_ar16(uint16& regS)
{
  uint8 tmp = 0;
  bool newCarry = false;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      newCarry = (tmp & 0x01) != 0;
      tmp = (tmp >> 1) | (newCarry ? 0x80 : 0);
      write(regS, tmp);
      setFlag(FlagBits::Zero, tmp == 0);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, false);
      setFlag(FlagBits::Carry, newCarry);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::rrca()
{
  uint8 val = readRegister(AF, RegisterBits::High);
  bool newCarry = (val & 0x01) != 0;
  val = (val >> 1) | (newCarry ? 0x80 : 0);
  setRegister(AF, val, RegisterBits::High);
  setFlag(FlagBits::Zero, false);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::sla_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val = readRegister(regS, regSB);
  bool newCarry = (val & 0x80) != 0;
  val = val << 1;
  setRegister(regS, val, regSB);
  setFlag(FlagBits::Zero, val == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::sla_ar16(uint16& regS)
{
  uint8 tmp = 0;
  bool newCarry = false;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      newCarry = (tmp & 0x80) != 0;
      tmp = tmp << 1;
      write(regS, tmp);
      setFlag(FlagBits::Zero, tmp == 0);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, false);
      setFlag(FlagBits::Carry, newCarry);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::sra_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val = readRegister(regS, regSB);
  bool newCarry = (val & 0x01) != 0;
  val = (val >> 1) | (val & 0x80);
  setRegister(regS, val, regSB);
  setFlag(FlagBits::Zero, val == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::sra_ar16(uint16& regS)
{
  uint8 tmp = 0;
  bool newCarry = false;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      newCarry = (tmp & 0x01) != 0;
      tmp = (tmp >> 1) | (tmp & 0x80);
      write(regS, tmp);
      setFlag(FlagBits::Zero, tmp == 0);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, false);
      setFlag(FlagBits::Carry, newCarry);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::srl_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val = readRegister(regS, regSB);
  bool newCarry = (val & 0x01) != 0;
  val = val >> 1;
  setRegister(regS, val, regSB);
  setFlag(FlagBits::Zero, val == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, newCarry);
  next();
}

void
CPU::srl_ar16(uint16& regS)
{
  uint8 tmp = 0;
  bool newCarry = false;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      newCarry = (tmp & 0x01) != 0;
      tmp = tmp >> 1;
      write(regS, tmp);
      setFlag(FlagBits::Zero, tmp == 0);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, false);
      setFlag(FlagBits::Carry, newCarry);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::swap_r8(uint16& regS, CPU::RegisterBits regSB)
{
  uint8 val = readRegister(regS, regSB);
  val = (val << 4) | (val >> 4);
  setRegister(regS, val, regSB);
  setFlag(FlagBits::Zero, val == 0);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  setFlag(FlagBits::Carry, false);
  next();
}

void
CPU::swap_ar16(uint16& regS)
{
  uint8 tmp = 0;
  switch (instruction_cycles) {
    case 0:
      read(regS);
      instruction_cycles++;
      break;
    case 1:
      tmp = ioData;
      tmp = (tmp << 4) | (tmp >> 4);
      write(regS, tmp);
      setFlag(FlagBits::Zero, tmp == 0);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, false);
      setFlag(FlagBits::Carry, false);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

// jumps and subroutine instructions
void
CPU::call_a16()
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
      highByte = ioData;
      iduDec(SP);
      instruction_cycles++;
      break;
    case 3:
      write(
        SP,
        (uint8)readRegister(PC, RegisterBits::High)); // push high byte of PC
      iduDec(SP);
      instruction_cycles++;
      break;
    case 4:
      write(SP,
            (uint8)readRegister(PC, RegisterBits::Low)); // push low byte of PC
      setRegister(PC,
                  (highByte << 8) | lowByte,
                  RegisterBits::Full); // set PC to new address
      instruction_cycles++;
      break;
    case 5:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::call_cc(Condition flag)
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
      if (checkCondition(flag)) {
        highByte = ioData;
        iduDec(SP);
        instruction_cycles++;
      } else {
        instruction_cycles = 0;
        next();
      }
      break;
    case 3:
      iduDec(SP);
      write(
        SP,
        (uint8)readRegister(PC, RegisterBits::High)); // push high byte of PC
      instruction_cycles++;
      break;
    case 4:
      write(SP,
            (uint8)readRegister(PC, RegisterBits::Low)); // push low byte of PC
      setRegister(PC,
                  (highByte << 8) | lowByte,
                  RegisterBits::Full); // set PC to new address
      instruction_cycles++;
      break;
    case 5:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::jp_r16(uint16& regS)
{
  setRegister(PC, regS, RegisterBits::Full);
  next();
}

void
CPU::jp_a16()
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
      setRegister(PC, (ioData << 8) | lowByte, RegisterBits::Full);
      instruction_cycles++;
      break;
    case 3:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::jp_cc_a16(Condition flag)
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
      if (checkCondition(flag)) {
        setRegister(PC, (ioData << 8) | lowByte, RegisterBits::Full);
        instruction_cycles++;
      } else {
        instruction_cycles = 0;
        next();
      }
      break;
    case 3:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::jr_a8()
{
  int16 new_addr = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      new_addr = (int16)PC + (int8)ioData;
      setRegister(PC, new_addr, RegisterBits::Full);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::jr_cc_a8(Condition flag)
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      if (checkCondition(flag)) {
        int16 new_addr = (int16)PC + (int8)ioData;
        setRegister(PC, new_addr, RegisterBits::Full);
        instruction_cycles++;
      } else {
        instruction_cycles = 0;
        next();
      }
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ret_cc(Condition flag)
{
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      break;
    case 1:
      if (checkCondition(flag)) {
        read(SP);
        iduInc(SP);
        instruction_cycles++;
      } else {
        instruction_cycles = 0;
        next();
      }
      break;
    case 2:
      lowByte = ioData;
      read(SP);
      iduInc(SP);
      instruction_cycles++;
      break;
    case 3:
      setRegister(PC, (ioData << 8) | lowByte, RegisterBits::Full);
      instruction_cycles++;
      break;
    case 4:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ret()
{
  switch (instruction_cycles) {
    case 0:
      read(SP);
      iduInc(SP);
      instruction_cycles++;
      break;
    case 1:
      lowByte = ioData;
      read(SP);
      iduInc(SP);
      instruction_cycles++;
      break;
    case 2:
      setRegister(PC, (ioData << 8) | lowByte, RegisterBits::Full);
      instruction_cycles++;
      break;
    case 3:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::reti()
{
  switch (instruction_cycles) {
    case 0:
      read(SP);
      iduInc(SP);
      instruction_cycles++;
      break;
    case 1:
      lowByte = ioData;
      read(SP);
      iduInc(SP);
      instruction_cycles++;
      break;
    case 2:
      setRegister(PC, (ioData << 8) | lowByte, RegisterBits::Full);
      ime = Ime::Enable;
      instruction_cycles++;
      break;
    case 3:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::rst()
{
  switch (instruction_cycles) {
    case 0:
      iduDec(SP);
      instruction_cycles++;
      break;
    case 1:
      write(
        SP,
        (uint8)readRegister(PC, RegisterBits::High)); // push high byte of PC
      iduDec(SP);
      instruction_cycles++;
      break;
    case 2:
      write(SP,
            (uint8)readRegister(PC, RegisterBits::Low)); // push low byte of PC
      setRegister(
        PC, curr_opcode & 0x38, RegisterBits::Full); // set PC to new address
      instruction_cycles++;
      break;
    case 3:
      instruction_cycles = 0;
      next();
      break;
  }
}

// carry flag instructions
void
CPU::ccf()
{
  bool carry = getFlag(FlagBits::Carry);
  setFlag(FlagBits::Carry, !carry);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  next();
}

void
CPU::scf()
{
  setFlag(FlagBits::Carry, true);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  next();
}

// stack manipulation instructions
void
CPU::add_sp_e8()
{
  int8 val = 0;
  uint16 result = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      val = (int8)ioData;
      result = (int16)SP + (int16)val;
      setFlag(FlagBits::Zero, false);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, ((SP & 0x0F) + ((uint16)val & 0x0F)) > 0x0F);
      setFlag(FlagBits::Carry, ((SP & 0xFF) + ((uint16)val & 0xFF)) > 0xFF);
      setRegister(SP, result, RegisterBits::Full);
      instruction_cycles++;
      break;
    case 2:
      break;
    case 3:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ld_a16_sp()
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
      highByte = ioData;
      write((highByte << 8) | lowByte,
            (uint8)readRegister(SP, RegisterBits::Low)); // write low byte of SP
      instruction_cycles++;
      break;
    case 3:
      write(
        ((highByte << 8) | lowByte) + 1,
        (uint8)readRegister(SP, RegisterBits::High)); // write high byte of SP
      instruction_cycles++;
      break;
    case 4:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ld_hl_sp_e8()
{
  int8 val = 0;
  uint16 result = 0;
  switch (instruction_cycles) {
    case 0:
      instruction_cycles++;
      next();
      break;
    case 1:
      val = (int8)ioData;
      result = (int16)SP + (int16)val;
      setRegister(HL, result, RegisterBits::Full);
      setFlag(FlagBits::Zero, false);
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, ((SP & 0x0F) + ((uint16)val & 0x0F)) > 0x0F);
      setFlag(FlagBits::Carry, ((SP & 0xFF) + ((uint16)val & 0xFF)) > 0xFF);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::ld_r16_r16(uint16& regD, uint16& regS)
{
  uint16 val = 0;
  switch (instruction_cycles) {
    case 0:
      val = readRegister(regS, RegisterBits::Full);
      setRegister(regD, val, RegisterBits::Full);
      instruction_cycles++;
      break;
    case 1:
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::pop_r16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      read(SP);
      iduInc(SP);
      instruction_cycles++;
      break;
    case 1:
      lowByte = ioData;
      read(SP);
      iduInc(SP);
      instruction_cycles++;
      break;
    case 2:
      setRegister(regS, (ioData << 8) | lowByte, RegisterBits::Full);
      instruction_cycles = 0;
      next();
      break;
  }
}

void
CPU::push_r16(uint16& regS)
{
  switch (instruction_cycles) {
    case 0:
      iduDec(SP);
      instruction_cycles++;
      break;
    case 1:
      write(SP,
            (uint8)(readRegister(regS, RegisterBits::High))); // push high byte
      iduDec(SP);
      instruction_cycles++;
      break;
    case 2:
      write(SP,
            (uint8)(readRegister(regS, RegisterBits::Low))); // push low byte
      instruction_cycles++;
      break;
    case 3:
      instruction_cycles = 0;
      next();
      break;
  }
}

// miscellaneous instructions
void
CPU::di()
{
  ime = Ime::Disable;
  next();
}

void
CPU::ei()
{
  if (ime == Ime::Disable) {
    ime = Ime::RequestEnable;
  }
  next();
}

void
CPU::halt()
{
  halted = getInterrupts() == 0;
  instruction_cycles = 0;
  read(PC);

  if (ime != Ime::Enable && !halted) {
    return;
  }
  iduInc(PC);
}

void
CPU::daa()
{
  uint8 val = readRegister(AF, RegisterBits::High);
  bool carry = getFlag(FlagBits::Carry);
  bool halfCarry = getFlag(FlagBits::HalfCarry);
  bool subtract = getFlag(FlagBits::Subtract);
  if (!subtract) {
    if (carry || val > 0x99) {
      val += 0x60;
      carry = true;
    }
    if (halfCarry || (val & 0x0F) > 0x09) {
      val += 0x06;
    }
  } else {
    if (carry) {
      val -= 0x60;
    }
    if (halfCarry) {
      val -= 0x06;
    }
  }
  setRegister(AF, val, RegisterBits::High);
  setFlag(FlagBits::Zero, val == 0);
  setFlag(FlagBits::Carry, carry);
  setFlag(FlagBits::HalfCarry, false);
  next();
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
  use_prefix_instruction = true;
  next();
}
