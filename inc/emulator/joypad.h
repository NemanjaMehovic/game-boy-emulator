#ifndef JOYPAD_H
#define JOYPAD_H

#include "common.h"

class MMU;

enum class JoypadInputs
{
  RIGHT = 0x01,
  LEFT = 0x02,
  UP = 0x04,
  DOWN = 0x08,
  A = 0x10,
  B = 0x20,
  SELECT = 0x40,
  START = 0x80
};

class Joypad
{
public:
  Joypad() = default;
  void initialize();
  void write(uint8 val);
  uint8 read();
  void handleButton(JoypadInputs button, bool released);
  void setMMU(MMU* mmu) { this->mmu = mmu; }

private:
  bool select_high = true;
  bool dpad_high = true;
  uint8 buttons = 0xFF;
  MMU* mmu = nullptr;
};

#endif
