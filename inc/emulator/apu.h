#ifndef APU_H
#define APU_H

#include "common.h"

// Won't have actual audio but will support APU registers for compatibility
class APU
{
public:
  APU();
  void tick();
  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 val);

private:
  uint8 nr10 = 0x80; // Channel 1 Sweep
  uint8 nr11 = 0xBF; // Channel 1 Sound length/Wave pattern duty
  uint8 nr12 = 0xF3; // Channel 1 Volume Envelope
  uint8 nr13 = 0xFF; // Channel 1 Frequency lo
  uint8 nr14 = 0xBF; // Channel 1 Frequency hi

  uint8 nr21 = 0x3F; // Channel 2 Sound length/Wave pattern duty
  uint8 nr22 = 0;    // Channel 2 Volume Envelope
  uint8 nr23 = 0xFF; // Channel 2 Frequency lo
  uint8 nr24 = 0xBF; // Channel 2 Frequency hi

  uint8 nr30 = 0x7F; // Channel 3 Sound on/off
  uint8 nr31 = 0xFF; // Channel 3 Sound length
  uint8 nr32 = 0x9F; // Channel 3 Select output level
  uint8 nr33 = 0xFF; // Channel 3 Frequency lo
  uint8 nr34 = 0xBF; // Channel 3 Frequency hi

  uint8 nr41 = 0xFF; // Channel 4 Sound length
  uint8 nr42 = 0;    // Channel 4 Volume Envelope
  uint8 nr43 = 0;    // Channel 4 Polynomial Counter
  uint8 nr44 = 0xBF; // Channel 4 Counter/consecutive; Initial

  uint8 nr50 = 0x77; // Channel control / ON-OFF / Volume
  uint8 nr51 = 0xF3; // Selection of Sound output terminal
  uint8 nr52 = 0xF1; // Sound on/off

  // Wave RAM (16 bytes)
  uint8 wave_ram[16] = { 0 };

  void initialize();
  uint8 readNR10();
  uint8 readNR11();
  uint8 readNR12();
  uint8 readNR13();
  uint8 readNR14();
  uint8 readNR21();
  uint8 readNR22();
  uint8 readNR23();
  uint8 readNR24();
  uint8 readNR30();
  uint8 readNR31();
  uint8 readNR32();
  uint8 readNR33();
  uint8 readNR34();
  uint8 readNR41();
  uint8 readNR42();
  uint8 readNR43();
  uint8 readNR44();
  uint8 readNR50();
  uint8 readNR51();
  uint8 readNR52();
  uint8 readWaveRAM(uint16 addr);
  void writeNR10(uint8 val);
  void writeNR11(uint8 val);
  void writeNR12(uint8 val);
  void writeNR13(uint8 val);
  void writeNR14(uint8 val);
  void writeNR21(uint8 val);
  void writeNR22(uint8 val);
  void writeNR23(uint8 val);
  void writeNR24(uint8 val);
  void writeNR30(uint8 val);
  void writeNR31(uint8 val);
  void writeNR32(uint8 val);
  void writeNR33(uint8 val);
  void writeNR34(uint8 val);
  void writeNR41(uint8 val);
  void writeNR42(uint8 val);
  void writeNR43(uint8 val);
  void writeNR44(uint8 val);
  void writeNR50(uint8 val);
  void writeNR51(uint8 val);
  void writeNR52(uint8 val);
  void writeWaveRAM(uint16 addr, uint8 val);
};

#endif // APU_H
