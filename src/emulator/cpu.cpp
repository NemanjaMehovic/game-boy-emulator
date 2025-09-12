#include "cpu.h"
#include "common.h"
#include "mmu.h"

CPU::CPU()
{
  initialize();
}

void
CPU::tick(uint64 Tcycle)
{
  // 4 T-cycles = 1 M-cycle = 1 CPU cycle
  switch (Tcycle % 4) {
    case 0:
      cycle();
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
  }
}

void
CPU::initialize()
{
  AF = 0x01B0;
  BC = 0x0013;
  DE = 0x00D8;
  HL = 0x014D;
  SP = 0xFFFE;
  PC = 0x0100;

  instruction_cycles = 0;
  use_prefix_instruction = false;
  curr_opcode = 0;
  halted = false;

  ime = Ime::Disable;
  IER = 0;
  IFR = 0xE1;

  ioData = 0;
  highByte = 0;
  lowByte = 0;
}

void
CPU::cycle()
{
  bool servicingInterrupt = false;
  if (instruction_cycles == 0) {
    uint8 interrupts = getInterrupts();
    if (interrupts != 0 && !use_prefix_instruction) {
      halted = false;
      if (ime == Ime::Enable) {
        current_instruction = std::bind(&CPU::serviceInterrupt, this);
        servicingInterrupt = true;
        log_debug(
          "DEBUG_STATE A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X "
          "L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X",
          readRegister(AF, RegisterBits::High),
          readRegister(AF, RegisterBits::Low),
          readRegister(BC, RegisterBits::High),
          readRegister(BC, RegisterBits::Low),
          readRegister(DE, RegisterBits::High),
          readRegister(DE, RegisterBits::Low),
          readRegister(HL, RegisterBits::High),
          readRegister(HL, RegisterBits::Low),
          SP,
          PC - 1,
          mmu->read(PC - 1, Component::CPU),
          mmu->read(PC, Component::CPU),
          mmu->read(PC + 1, Component::CPU),
          mmu->read(PC + 2, Component::CPU));
      }
    }

    if (!servicingInterrupt) {
      if (use_prefix_instruction) {
        current_instruction = fetchPrefixInstruction(ioData);
        use_prefix_instruction = false;
      } else {
        if (bad_opcodes.find(ioData) != bad_opcodes.end()) {
          log_error("Bad opcode: 0x%02X", ioData);
          abort();
        }
        if (!halted || servicingInterrupt) {
          // A:00 F:11 B:22 C:33 D:44 E:55 H:66 L:77 SP:8888 PC:9999
          // PCMEM:AA,BB,CC,DD
          log_debug(
            "DEBUG_STATE A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X "
            "L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X",
            readRegister(AF, RegisterBits::High),
            readRegister(AF, RegisterBits::Low),
            readRegister(BC, RegisterBits::High),
            readRegister(BC, RegisterBits::Low),
            readRegister(DE, RegisterBits::High),
            readRegister(DE, RegisterBits::Low),
            readRegister(HL, RegisterBits::High),
            readRegister(HL, RegisterBits::Low),
            SP,
            PC - 1,
            mmu->read(PC - 1, Component::CPU),
            mmu->read(PC, Component::CPU),
            mmu->read(PC + 1, Component::CPU),
            mmu->read(PC + 2, Component::CPU));
        }
        current_instruction = opcode_map.at(ioData);
      }
    }
    // not correct when servicing an interrupt
    curr_opcode = ioData;
  }

  if (halted) {
    return;
  }

  current_instruction();

  if (instruction_cycles == 0 &&
      (ime == Ime::PendingEnable || ime == Ime::RequestEnable)) {
    // ime will be enabled after the next instruction
    if (ime == Ime::RequestEnable) {
      ime = Ime::PendingEnable;
    } else {
      ime = Ime::Enable;
    }
  }
}

std::function<void()>
CPU::fetchPrefixInstruction(uint8 opcode)
{
  std::function<void()> instr = nullptr;
  uint16* reg = nullptr;
  bool indirect = false;
  CPU::RegisterBits reg_bits = CPU::RegisterBits::Full;

  uint8 reg_val = opcode & 0x7;
  uint8 index_val = (opcode >> 3) & 0x7;

  switch (reg_val) {
    case 0:
      reg = &BC;
      reg_bits = CPU::RegisterBits::High;
      break;
    case 1:
      reg = &BC;
      reg_bits = CPU::RegisterBits::Low;
      break;
    case 2:
      reg = &DE;
      reg_bits = CPU::RegisterBits::High;
      break;
    case 3:
      reg = &DE;
      reg_bits = CPU::RegisterBits::Low;
      break;
    case 4:
      reg = &HL;
      reg_bits = CPU::RegisterBits::High;
      break;
    case 5:
      reg = &HL;
      reg_bits = CPU::RegisterBits::Low;
      break;
    case 6:
      reg = &HL;
      indirect = true;
      break;
    case 7:
      reg = &AF;
      reg_bits = CPU::RegisterBits::High;
      break;
  }

  switch (opcode & 0xC0) {
    case 0x00:
      switch (index_val) {
        case 0:
          if (indirect) {
            instr = std::bind(&CPU::rlc_ar16, this, std::ref(*reg));
          } else {
            instr = std::bind(&CPU::rlc_r8, this, std::ref(*reg), reg_bits);
          }
          break;
        case 1:
          if (indirect) {
            instr = std::bind(&CPU::rrc_ar16, this, std::ref(*reg));
          } else {
            instr = std::bind(&CPU::rrc_r8, this, std::ref(*reg), reg_bits);
          }
          break;
        case 2:
          if (indirect) {
            instr = std::bind(&CPU::rl_ar16, this, std::ref(*reg));
          } else {
            instr = std::bind(&CPU::rl_r8, this, std::ref(*reg), reg_bits);
          }
          break;
        case 3:
          if (indirect) {
            instr = std::bind(&CPU::rr_ar16, this, std::ref(*reg));
          } else {
            instr = std::bind(&CPU::rr_r8, this, std::ref(*reg), reg_bits);
          }
          break;
        case 4:
          if (indirect) {
            instr = std::bind(&CPU::sla_ar16, this, std::ref(*reg));
          } else {
            instr = std::bind(&CPU::sla_r8, this, std::ref(*reg), reg_bits);
          }
          break;
        case 5:
          if (indirect) {
            instr = std::bind(&CPU::sra_ar16, this, std::ref(*reg));
          } else {
            instr = std::bind(&CPU::sra_r8, this, std::ref(*reg), reg_bits);
          }
          break;
        case 6:
          if (indirect) {
            instr = std::bind(&CPU::swap_ar16, this, std::ref(*reg));
          } else {
            instr = std::bind(&CPU::swap_r8, this, std::ref(*reg), reg_bits);
          }
          break;
        case 7:
          if (indirect) {
            instr = std::bind(&CPU::srl_ar16, this, std::ref(*reg));
          } else {
            instr = std::bind(&CPU::srl_r8, this, std::ref(*reg), reg_bits);
          }
          break;
      }
      break;
    case 0x40:
      if (indirect) {
        instr = std::bind(&CPU::bit_ar16, this, std::ref(*reg), index_val);
      } else {
        instr =
          std::bind(&CPU::bit_r8, this, std::ref(*reg), reg_bits, index_val);
      }
      break;
    case 0x80:
      if (indirect) {
        instr = std::bind(&CPU::res_ar16, this, std::ref(*reg), index_val);
      } else {
        instr =
          std::bind(&CPU::res_r8, this, std::ref(*reg), reg_bits, index_val);
      }
      break;
    case 0xC0:
      if (indirect) {
        instr = std::bind(&CPU::set_ar16, this, std::ref(*reg), index_val);
      } else {
        instr =
          std::bind(&CPU::set_r8, this, std::ref(*reg), reg_bits, index_val);
      }
      break;
  }

  return instr;
}

void
CPU::iduInc(uint16& reg, uint16 value)
{
  reg += value;
  // TODO handle OAM Bug
}

void
CPU::iduDec(uint16& reg, uint16 value)
{
  reg -= value;
  // TODO handle OAM Bug
}

void
CPU::read(uint16 addr)
{
  ioData = mmu->read(addr, Component::CPU);
}

void
CPU::write(uint16 addr, uint8 val)
{
  mmu->write(addr, val, Component::CPU);
}

void
CPU::setRegister(uint16& reg, uint16 val, RegisterBits register_bits)
{
  switch (register_bits) {
    case RegisterBits::High:
      reg = (reg & 0x00FF) | (val << 8);
      break;
    case RegisterBits::Low:
      if (&reg == &AF) {
        // special handling of F register
        // lowest 4 bits don't exist
        reg = (reg & 0xFF00) | (val & 0x00F0);
      } else {
        reg = (reg & 0xFF00) | (val & 0x00FF);
      }
      break;
    case RegisterBits::Full:
      if (&reg == &AF) {
        // special handling of F register
        // lowest 4 bits don't exist
        reg = val & 0xFFF0;
      } else {
        reg = val;
      }
      break;
  }
}

uint16
CPU::readRegister(uint16& reg, RegisterBits register_bits)
{
  switch (register_bits) {
    case RegisterBits::High:
      return (reg >> 8) & 0x00FF;
    case RegisterBits::Low:
      return reg & 0x00FF;
    case RegisterBits::Full:
      return reg;
  }
  log_error("CPU reading unknown bits from register.");
  return reg;
}

void
CPU::setFlag(FlagBits flag, bool value)
{
  if (value) {
    AF |= static_cast<uint16>(flag);
  } else {
    AF &= ~static_cast<uint16>(flag);
  }
}

bool
CPU::getFlag(FlagBits flag)
{
  return (AF & static_cast<uint16>(flag)) != 0;
}

bool
CPU::checkCondition(Condition flag)
{
  switch (flag) {
    case Condition::Zero:
      return getFlag(FlagBits::Zero);
    case Condition::NotZero:
      return !getFlag(FlagBits::Zero);
    case Condition::Carry:
      return getFlag(FlagBits::Carry);
    case Condition::NotCarry:
      return !getFlag(FlagBits::Carry);
  }
  log_error("CPU checking unknown condition.");
  return false;
}

uint8
CPU::getInterrupts()
{
  // only the first 5 bits are used
  return IER & IFR & 0x1F;
}

void
CPU::serviceInterrupt()
{
  const std::vector<std::pair<uint8, uint16>> interrupt_vectors = {
    { 0x01, 0x40 }, // V-Blank
    { 0x02, 0x48 }, // LCD STAT
    { 0x04, 0x50 }, // Timer
    { 0x08, 0x58 }, // Serial
    { 0x10, 0x60 }  // Joypad
  };
  switch (instruction_cycles) {
    case 0:
      // interrupts should happen before the fetch of the next instruction
      // but we alwayys fetch the next instruction at the end of the cycle
      // so we need to decrement PC here to offset the increment as a little
      // hack
      PC -= 1;
      instruction_cycles++;
      break;
    case 1:
      iduDec(SP);
      instruction_cycles++;
      break;
    case 2:
      write(SP, (uint8)readRegister(PC, RegisterBits::High));
      iduDec(SP);
      instruction_cycles++;
      break;
    case 3:
      write(SP, (uint8)readRegister(PC, RegisterBits::Low));
      for (const auto& [bit, vector] : interrupt_vectors) {
        if ((getInterrupts() & bit) != 0) {
          // clear the interrupt flag
          log_debug("Servicing interrupt 0x%X", vector);
          IFR &= ~bit;
          setRegister(PC, vector, RegisterBits::Full);
        }
      }
      instruction_cycles++;
      break;
    case 4:
      ime = Ime::Disable;
      instruction_cycles = 0;
      next();
      break;
  }
}
