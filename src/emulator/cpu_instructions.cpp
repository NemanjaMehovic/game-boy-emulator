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
  log_debug("ld_r8_r8");
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
      log_debug("ld_r8_n8 0x%X", ioData);
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
      log_debug("ld_r16_n16 0x%X", (ioData << 8) | lowByte);
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
      log_debug("ld_ar16_r8");
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
      log_debug("ld_ar16_n8 0x%X", ioData);
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
      log_debug("ld_r8_ar16");
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
      log_debug("ld_a16_r8 0x%X", (ioData << 8) | lowByte);
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
      log_debug("ldh_a8_r8 0x%X", ioData);
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
      log_debug("ldh_ar8_r8");
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
      log_debug("ld_r8_a16 0x%X", (ioData << 8) | lowByte);
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
      log_debug("ldh_r8_a8 0x%X", ioData);
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
      log_debug("ldh_r8_ar8");
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
      log_debug("ld_ar16i_r8");
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
      log_debug("ld_ar16d_r8");
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
      log_debug("ld_r8_ar16i");
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
      log_debug("ld_r8_ar16d");
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
  log_debug("adc_r8");
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
      log_debug("adc_ar16 [ignore next log]");
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
      log_debug("adc_n8 0x%X [ignore next log]", tmp);
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
  log_debug("add_r8");
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
      log_debug("add_ar16 [ignore next log]");
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
      log_debug("add_n8 0x%X [ignore next log]", tmp);
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
  log_debug("cp_r8");
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
      log_debug("cp_ar16 [ignore next log]");
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
      log_debug("cp_n8 0x%X [ignore next log]", tmp);
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
  log_debug("dec_r8");
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
      log_debug("dec_ar16");
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
  log_debug("inc_r8");
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
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry, (ioData & 0x0F) == 0x0F);
      write(regS, ioData + 1);
      instruction_cycles++;
      break;
    case 2:
      instruction_cycles = 0;
      next();
      log_debug("inc_ar16");
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
  log_debug("sbc_r8");
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
      log_debug("sbc_ar16 [ignore next log]");
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
      log_debug("sbc_n8 0x%X [ignore next log]", tmp);
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
  log_debug("sub_r8");
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
      log_debug("sub_ar16 [ignore next log]");
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
      log_debug("sub_n8 0x%X [ignore next log]", tmp);
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
      setFlag(FlagBits::Subtract, false);
      setFlag(FlagBits::HalfCarry,
              ((val1 & 0x0FFF) + (val2 & 0x0FFF)) > 0x0FFF);
      setFlag(FlagBits::Carry, (val1 + val2) > 0xFFFF);
      instruction_cycles++;
      break;
    case 1:
      instruction_cycles = 0;
      next();
      log_debug("add_r16_r16");
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
      log_debug("dec_r16");
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
      log_debug("inc_r16");
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
  log_debug("and_r8");
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
      log_debug("and_ar16 [ignore next log]");
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
      log_debug("and_n8 0x%X [ignore next log]", tmp);
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
  log_debug("cpl");
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
  log_debug("or_r8");
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
      log_debug("or_ar16 [ignore next log]");
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
      log_debug("or_n8 0x%X [ignore next log]", tmp);
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
  log_debug("xor_r8");
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
      log_debug("xor_ar16 [ignore next log]");
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
      log_debug("xor_n8 0x%X [ignore next log]", tmp);
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
  log_debug("bit_r8");
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
      log_debug("bit_ar16 [ignore next log]");
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
  log_debug("res_r8");
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
      log_debug("res_ar16");
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
  log_debug("set_r8");
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
      log_debug("set_ar16");
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
  log_debug("rl_r8");
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
      log_debug("rl_ar16");
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
  log_debug("rla");
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
  log_debug("rlc_r8");
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
      log_debug("rlc_ar16");
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
  log_debug("rlca");
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
  log_debug("rr_r8");
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
      log_debug("rr_ar16");
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
  log_debug("rra");
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
  log_debug("rrc_r8");
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
      log_debug("rrc_ar16");
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
  log_debug("rrca");
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
  log_debug("sla_r8");
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
      log_debug("sla_ar16");
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
  log_debug("sra_r8");
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
      log_debug("sra_ar16");
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
  log_debug("srl_r8");
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
      log_debug("srl_ar16");
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
  log_debug("swap_r8");
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
      log_debug("swap_ar16");
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
      log_debug("call_a16 0x%X", (highByte << 8) | lowByte);
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
        log_debug("call_cc 0x%X [not taking]", (ioData << 8) | lowByte);
        next();
      }
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
      log_debug("call_cc 0x%X", (highByte << 8) | lowByte);
      break;
  }
}

void
CPU::jp_r16(uint16& regS)
{
  setRegister(PC, regS, RegisterBits::Full);
  next();
  log_debug("jp_r16");
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
      log_debug("jp_a16 0x%X",(ioData << 8) | lowByte);
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
        log_debug("jp_cc_a16 0x%X [not taking]", (ioData << 8) | lowByte);
        next();
      }
      break;
    case 3:
      instruction_cycles = 0;
      log_debug("jp_cc_a16 0x%X", (ioData << 8) | lowByte);
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
      log_debug("jr_a8 0x%X", ioData << 8);
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
        log_debug("jr_cc_a8 0x%X [not taking]", ioData << 8);
        next();
      }
      break;
    case 2:
      instruction_cycles = 0;
      log_debug("jr_cc_a8 0x%X", ioData << 8);
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
        log_debug("ret_cc [not taking]");
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
      log_debug("ret_cc");
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
      log_debug("ret")
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
      log_debug("reti");
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
      log_debug("rst 0x%X", curr_opcode);
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
  log_debug("ccf");
}

void
CPU::scf()
{
  setFlag(FlagBits::Carry, true);
  setFlag(FlagBits::Subtract, false);
  setFlag(FlagBits::HalfCarry, false);
  next();
  log_debug("scf");

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
      instruction_cycles++;
      break;
    case 3:
      instruction_cycles = 0;
      log_debug("add_sp_e8 0x%X", ioData);
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
      log_debug("ld_a16_sp 0x%X", (highByte << 8) | lowByte);
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
      log_debug("ld_hl_sp_e8 0x%X", ioData);
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
      log_debug("ld_r16_r16");
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
      log_debug("pop_r16");
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
      log_debug("push_r16");
      break;
  }
}

// miscellaneous instructions
void
CPU::di()
{
  ime = Ime::Disable;
  next();
  log_debug("di");
}

void
CPU::ei()
{
  if (ime == Ime::Disable) {
    ime = Ime::RequestEnable;
  }
  next();
  log_debug("ei");
}

void
CPU::halt()
{
  halted = getInterrupts() == 0;
  instruction_cycles = 0;
  read(PC);
  log_debug("halt halted = %d IME = %d IFR = %d IER = %d", halted, ime == Ime::Enable, IFR, IER);
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
  log_debug("daa");
}

void
CPU::nop()
{
  next();
  log_debug("nop");
}

void
CPU::stop()
{
  // TODO
  next();
  log_debug("stop");
}

void
CPU::prefix_cb()
{
  use_prefix_instruction = true;
  next();
  log_debug("prefix_cb");
}
