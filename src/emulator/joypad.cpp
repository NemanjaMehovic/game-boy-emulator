#include "joypad.h"
#include "mmu.h"

void
Joypad::initialize()
{
  joypad_state = 0xFF; // All buttons released
  joypad_select = 0x00; // 0x00 for directional, 0x10 for action buttons
  is_directional = true; // true if directional buttons are selected
  is_action = false; // true if action buttons are selected
}

void
Joypad::handleButton(uint8 button, bool pressed)
{
  
}

void
Joypad::write(uint8 val)
{
  
}

uint8
Joypad::read()
{
  uint8 result = joypad_state;
  if (is_directional) {
    result &= 0x0F; // Only directional buttons
  } else if (is_action) {
    result &= 0xF0; // Only action buttons
  }
  result |= joypad_select; // Add the select bits
  return result;
}