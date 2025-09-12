#include "joypad.h"
#include "mmu.h"

void
Joypad::initialize()
{
  select_high = true;
  dpad_high = true;
  buttons = 0xFF;
}

void
Joypad::handleButton(JoypadInputs button, bool released)
{
  if (released) {
    buttons |= static_cast<uint8>(button);
  } else {
    buttons &= ~(static_cast<uint8>(button));
  }

  bool irq =
    (!released) && ((!select_high && static_cast<uint8>(button) >= 0x10) ||
                    (!dpad_high && static_cast<uint8>(button) < 0x10));
  if (irq) {
    mmu->requestInterrupt(Interrupt::Joypad);
  }
}

void
Joypad::write(uint8 val)
{
  select_high = val & 0x20;
  dpad_high = val & 0x10;
}

uint8
Joypad::read()
{
  uint8 ret = 0xC0 | (select_high ? 0x20 : 0) | (dpad_high ? 0x10 : 0);
  uint8 tmp_keys = 0x0F;
  if (!select_high) {
    tmp_keys &= (buttons >> 4);
  }
  if (!dpad_high) {
    tmp_keys &= buttons;
  }
  return ret | tmp_keys;
}
