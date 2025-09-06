#include "timer.h"
#include "mmu.h"

Timer::Timer()
{
  initialize();
}

void
Timer::initialize()
{
  div = 0xABCC;
  tima = 0;
  tma = 0;
  tac = 0xF8;
  prev_bit = false;
  state = State::None;
}

uint8
Timer::read_div() const
{
  return (div >> 8) & 0xFF;
}

void
Timer::reset_div()
{
  div = 0;
  falling_edge();
}

uint8
Timer::read_tima() const
{
  return tima;
}

void
Timer::write_tima(uint8 val)
{
  if (state == State::Reload) {
    // Writing to Tima while it's being reloaded has no effect
    return;
  }
  tima = val;
  state = State::None;
}

uint8
Timer::read_tma() const
{
  return tma;
}

void
Timer::write_tma(uint8 val)
{
  tma = val;
  if (state == State::Reload) {
    tima = tma;
  }
}

uint8
Timer::read_tac() const
{
  return tac | 0xF8;
}

void
Timer::write_tac(uint8 val)
{
  tac = val & 0x07;
  falling_edge();
}

void
Timer::tima_tick()
{
  if (tima == 0xFF) {
    tima = 0; // Tima is 0 for one M-cycle before being reloaded with TMA
    state = State::Overflow;
  } else {
    tima++;
  }
}

void
Timer::write(uint16 addr, uint8 val)
{
  switch (addr) {
    case 0xFF04:
      reset_div();
      break;
    case 0xFF05:
      write_tima(val);
      break;
    case 0xFF06:
      write_tma(val);
      break;
    case 0xFF07:
      write_tac(val);
      break;
    default:
      log_error("Attempted to write to invalid timer address: 0x%04x", addr);
      break;
  }
}

uint8
Timer::read(uint16 addr) const
{
  switch (addr) {
    case 0xFF04:
      return read_div();
    case 0xFF05:
      return read_tima();
    case 0xFF06:
      return read_tma();
    case 0xFF07:
      return read_tac();
    default:
      log_error("Attempted to read from invalid timer address: 0x%04x", addr);
      return 0xFF;
  }
}

void
Timer::M_tick()
{
  if (state == State::Overflow) {
    tima = tma;
    mmu->requestInterrupt(Interrupt::Timer);
    state = State::Reload;
  } else if (state == State::Reload) {
    state = State::None;
  }
  div += 4;
  falling_edge();
}

void
Timer::falling_edge()
{
  constexpr uint8 freq_bits[] = { 9, 3, 5, 7 };
  uint8 bit = freq_bits[tac & 0x03];
  bool current_bit = (div >> bit) & 0x01 && (tac & 0x04);
  if (prev_bit && !current_bit) {
    tima_tick();
  }
  prev_bit = current_bit;
}
