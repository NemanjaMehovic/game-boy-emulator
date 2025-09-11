#include "apu.h"

APU::APU()
{
  initialize();
}

void
APU::initialize()
{
  nr10 = 0x80; // Channel 1 Sweep
  nr11 = 0xBF; // Channel 1 Sound length/Wave pattern duty
  nr12 = 0xF3; // Channel 1 Volume Envelope
  nr13 = 0xFF; // Channel 1 Frequency lo
  nr14 = 0xBF; // Channel 1 Frequency hi

  nr21 = 0x3F; // Channel 2 Sound length/Wave pattern duty
  nr22 = 0;    // Channel 2 Volume Envelope
  nr23 = 0xFF; // Channel 2 Frequency lo
  nr24 = 0xBF; // Channel 2 Frequency hi

  nr30 = 0x7F; // Channel 3 Sound on/off
  nr31 = 0xFF; // Channel 3 Sound length
  nr32 = 0x9F; // Channel 3 Select output level
  nr33 = 0xFF; // Channel 3 Frequency lo
  nr34 = 0xBF; // Channel 3 Frequency hi

  nr41 = 0xFF; // Channel 4 Sound length
  nr42 = 0;    // Channel 4 Volume Envelope
  nr43 = 0;    // Channel 4 Polynomial Counter
  nr44 = 0xBF; // Channel 4 Counter/consecutive; Initial

  nr50 = 0x77; // Channel control / ON-OFF / Volume
  nr51 = 0xF3; // Selection of Sound output terminal
  nr52 = 0xF1; // Sound on/off

  // Wave RAM (16 bytes)
  for(int i = 0; i < 16; i++) {
   wave_ram[i] = 0;
  }
}

void
APU::tick()
{
  // TODO
}

uint8
APU::read(uint16 addr)
{
  switch (addr) {
    case 0xFF10:
      return readNR10();
    case 0xFF11:
      return readNR11();
    case 0xFF12:
      return readNR12();
    case 0xFF13:
      return readNR13();
    case 0xFF14:
      return readNR14();
    case 0xFF16:
      return readNR21();
    case 0xFF17:
      return readNR22();
    case 0xFF18:
      return readNR23();
    case 0xFF19:
      return readNR24();
    case 0xFF1A:
      return readNR30();
    case 0xFF1B:
      return readNR31();
    case 0xFF1C:
      return readNR32();
    case 0xFF1D:
      return readNR33();
    case 0xFF1E:
      return readNR34();
    case 0xFF20:
      return readNR41();
    case 0xFF21:
      return readNR42();
    case 0xFF22:
      return readNR43();
    case 0xFF23:
      return readNR44();
    case 0xFF24:
      return readNR50();
    case 0xFF25:
      return readNR51();
    case 0xFF26:
      return readNR52();
    default:
      if (addr >= 0xFF30 && addr <= 0xFF3F) {
        return readWaveRAM(addr);
      } else {
        // Invalid APU address
        log_error("Attempted to read from invalid APU address 0x%X", addr);
        return 0xFF;
      }
      break;
  }
}

void
APU::write(uint16 addr, uint8 val)
{
  switch (addr) {
    case 0xFF10:
      writeNR10(val);
      break;
    case 0xFF11:
      writeNR11(val);
      break;
    case 0xFF12:
      writeNR12(val);
      break;
    case 0xFF13:
      writeNR13(val);
      break;
    case 0xFF14:
      writeNR14(val);
      break;
    case 0xFF16:
      writeNR21(val);
      break;
    case 0xFF17:
      writeNR22(val);
      break;
    case 0xFF18:
      writeNR23(val);
      break;
    case 0xFF19:
      writeNR24(val);
      break;
    case 0xFF1A:
      writeNR30(val);
      break;
    case 0xFF1B:
      writeNR31(val);
      break;
    case 0xFF1C:
      writeNR32(val);
      break;
    case 0xFF1D:
      writeNR33(val);
      break;
    case 0xFF1E:
      writeNR34(val);
      break;
    case 0xFF20:
      writeNR41(val);
      break;
    case 0xFF21:
      writeNR42(val);
      break;
    case 0xFF22:
      writeNR43(val);
      break;
    case 0xFF23:
      writeNR44(val);
      break;
    case 0xFF24:
      writeNR50(val);
      break;
    case 0xFF25:
      writeNR51(val);
      break;
    case 0xFF26:
      writeNR52(val);
      break;
    default:
      if (addr >= 0xFF30 && addr <= 0xFF3F) {
        writeWaveRAM(addr, val);
      } else {
        // Invalid APU address
        log_error("Attempted to write to invalid APU address 0x%X", addr);
      }
      break;
  }
}

uint8
APU::readNR10()
{
  return nr10 | 0x80;
}

uint8
APU::readNR11()
{
  return nr11 | 0x3F;
}

uint8
APU::readNR12()
{
  return nr12;
}

uint8
APU::readNR13()
{
  // write only
  return 0xFF;
}

uint8
APU::readNR14()
{
  return nr14 | 0xBF;
}

uint8
APU::readNR21()
{
  return nr21 | 0x3F;
}

uint8
APU::readNR22()
{
  return nr22;
}

uint8
APU::readNR23()
{
  // write only
  return 0xFF;
}

uint8
APU::readNR24()
{
  return nr24 | 0xBF;
}

uint8
APU::readNR30()
{
  return nr30 | 0x7F;
}

uint8
APU::readNR31()
{
  // write only
  return 0xFF;
}

uint8
APU::readNR32()
{
  return nr32 | 0x9F;
}

uint8
APU::readNR33()
{
  // write only
  return 0xFF;
}

uint8
APU::readNR34()
{
  return nr34 | 0xBF;
}

uint8
APU::readNR41()
{
  // write only
  return 0xFF;
}

uint8
APU::readNR42()
{
  return nr42;
}

uint8
APU::readNR43()
{
  return nr43;
}

uint8
APU::readNR44()
{
  return nr44 | 0xBF;
}

uint8
APU::readNR50()
{
  return nr50;
}

uint8
APU::readNR51()
{
  return nr51;
}

uint8
APU::readNR52()
{
  return nr52 | 0x70;
}

uint8
APU::readWaveRAM(uint16 addr)
{
  // TODO handle read restrictions based on channel 3 state
  return wave_ram[addr - 0xFF30];
}

void
APU::writeNR10(uint8 val)
{
  nr10 = val | 0x80;
}

void
APU::writeNR11(uint8 val)
{
  nr11 = val;
}

void
APU::writeNR12(uint8 val)
{
  nr12 = val;
}

void
APU::writeNR13(uint8 val)
{
  nr13 = val;
}

void
APU::writeNR14(uint8 val)
{
  nr14 = val | 0x38;
}

void
APU::writeNR21(uint8 val)
{
  nr21 = val;
}

void
APU::writeNR22(uint8 val)
{
  nr22 = val;
}

void
APU::writeNR23(uint8 val)
{
  nr23 = val;
}

void
APU::writeNR24(uint8 val)
{
  nr24 = val | 0x38;
}

void
APU::writeNR30(uint8 val)
{
  nr30 = val | 0x7F;
}

void
APU::writeNR31(uint8 val)
{
  nr31 = val;
}

void
APU::writeNR32(uint8 val)
{
  nr32 = val | 0x9F;
}

void
APU::writeNR33(uint8 val)
{
  nr33 = val;
}

void
APU::writeNR34(uint8 val)
{
  nr34 = val | 0x38;
}

void
APU::writeNR41(uint8 val)
{
  nr41 = val | 0xC0;
}

void
APU::writeNR42(uint8 val)
{
  nr42 = val;
}

void
APU::writeNR43(uint8 val)
{
  nr43 = val;
}

void
APU::writeNR44(uint8 val)
{
  nr44 = val | 0x3F;
}

void
APU::writeNR50(uint8 val)
{
  nr50 = val;
}

void
APU::writeNR51(uint8 val)
{
  nr51 = val;
}

void
APU::writeNR52(uint8 val)
{
  nr52 = (nr52 & 0x0F) | (val & 0x80) | 0x70;
}

void
APU::writeWaveRAM(uint16 addr, uint8 val)
{
  // TODO handle read restrictions based on channel 3 state
  wave_ram[addr - 0xFF30] = val;
}
