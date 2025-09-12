#ifndef JOYPAD_H
#define JOYPAD_H

#include "common.h"

class MMU;

class Joypad
{
public:
  Joypad() = default;
  void initialize();
  void write(uint8 val);
  uint8 read();
  void handleButton(uint8 button, bool pressed);
  void setMMU(MMU* mmu) { this->mmu = mmu; }
private:
    uint8 joypad_state = 0xFF;
    uint8 joypad_select = 0x00;
    bool is_directional = true;
    bool is_action = false;
    MMU* mmu = nullptr;
};


#endif
