#include "cpu.h"

CPU::CPU()
{
  initialize();
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
}

void
CPU::cycle()
{
  // Implementation of the CPU cycle logic goes here
}

// load instructions
void
CPU::ld_r8_r8(uint16& regD,
              CPU::RegisterBits regDB,
              uint16& regS,
              CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::ld_r8_n8(uint16& regD, CPU::RegisterBits regB)
{
  // TODO
}

void
CPU::ld_r16_n16(uint16& regD)
{
  // TODO
}

void
CPU::ld_ar16_r8(uint16& regD, uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::ld_ar16_n8(uint16& regD)
{
  // TODO
}

void
CPU::ld_r8_ar16(uint16& regD, CPU::RegisterBits regDB, uint16& regS)
{
  // TODO
}

void
CPU::ld_a16_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::ldh_a8_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::ldh_ar8_r8(uint16& regD,
                CPU::RegisterBits regDB,
                uint16& regS,
                CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::ld_r8_a16(uint16& regD, CPU::RegisterBits regDB)
{
  // TODO
}

void
CPU::ldh_r8_a8(uint16& regD, CPU::RegisterBits regDB)
{
  // TODO
}

void
CPU::ldh_r8_ar8(uint16& regD,
                CPU::RegisterBits regDB,
                uint16& regS,
                CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::ld_ar16i_r8(uint16& regD, uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::ld_ar16d_r8(uint16& regD, uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::ld_r8_ar16i(uint16& regD, CPU::RegisterBits regSD, uint16& regS)
{
  // TODO
}

void
CPU::ld_r8_ar16d(uint16& regD, CPU::RegisterBits regSD, uint16& regS)
{
  // TODO
}

// arithmetic instructions
void
CPU::adc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::adc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::adc_n8()
{
  // TODO
}

void
CPU::add_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::add_ar16(uint16& regS)
{
  // TODO
}

void
CPU::add_n8()
{
  // TODO
}

void
CPU::cp_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::cp_ar16(uint16& regS)
{
  // TODO
}

void
CPU::cp_n8()
{
  // TODO
}

void
CPU::dec_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::dec_ar16(uint16& regS)
{
  // TODO
}

void
CPU::inc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::inc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::sbc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::sbc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::sbc_n8()
{
  // TODO
}

void
CPU::sub_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::sub_ar16(uint16& regS)
{
  // TODO
}

void
CPU::sub_n8()
{
  // TODO
}

void
CPU::add_r16_r16(uint16& regD, uint16& regS)
{
  // TODO
}

void
CPU::dec_r16(uint16& regS)
{
  // TODO
}

void
CPU::inc_r16(uint16& regS)
{
  // TODO
}

// bitwise logic instructions
void
CPU::and_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::and_ar16(uint16& regS)
{
  // TODO
}

void
CPU::and_n8()
{
  // TODO
}

void
CPU::cpl()
{
  // TODO
}

void
CPU::or_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::or_ar16(uint16& regS)
{
  // TODO
}

void
CPU::or_n8()
{
  // TODO
}

void
CPU::xor_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::xor_ar16(uint16& regS)
{
  // TODO
}

void
CPU::xor_n8()
{
  // TODO
}

// bit manipulation instructions
void
CPU::bit_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::bit_ar16(uint16& regS)
{
  // TODO
}

void
CPU::res_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::res_ar16(uint16& regS)
{
  // TODO
}

void
CPU::set_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::set_ar16(uint16& regS)
{
  // TODO
}

// bit shift instructions
void
CPU::rl_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::rl_ar16(uint16& regS)
{
  // TODO
}

void
CPU::rla()
{
  // TODO
}

void
CPU::rlc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::rlc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::rlca()
{
  // TODO
}

void
CPU::rr_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::rr_ar16(uint16& regS)
{
  // TODO
}

void
CPU::rra()
{
  // TODO
}

void
CPU::rrc_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::rrc_ar16(uint16& regS)
{
  // TODO
}

void
CPU::rrca()
{
  // TODO
}

void
CPU::sla_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::sla_ar16(uint16& regS)
{
  // TODO
}

void
CPU::sra_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::sra_ar16(uint16& regS)
{
  // TODO
}

void
CPU::srl_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::srl_ar16(uint16& regS)
{
  // TODO
}

void
CPU::swap_r8(uint16& regS, CPU::RegisterBits regSB)
{
  // TODO
}

void
CPU::swap_ar16(uint16& regS)
{
  // TODO
}

// jumps and subroutine instructions
void
CPU::call_a16()
{
  // TODO
}

void
CPU::call_cc(Condition flag)
{
  // TODO
}

void
CPU::jp_r16(uint16& regS)
{
  // TODO
}

void
CPU::jp_a16()
{
  // TODO
}

void
CPU::jp_cc_a16(Condition flag)
{
  // TODO
}

void
CPU::jr_a8()
{
  // TODO
}

void
CPU::jr_cc_a8(Condition flag)
{
  // TODO
}

void
CPU::ret_cc(Condition flag)
{
  // TODO
}

void
CPU::ret()
{
  // TODO
}

void
CPU::reti()
{
  // TODO
}

void
CPU::rst()
{
  // TODO
}

// carry flag instructions
void
CPU::ccf()
{
  // TODO
}

void
CPU::scf()
{
  // TODO
}

// stack manipulation instructions
void
CPU::add_sp_e8()
{
  // TODO
}

void
CPU::ld_a16_sp()
{
  // TODO
}

void
CPU::ld_hl_sp_e8()
{
  // TODO
}

void
CPU::ld_r16_r16(uint16& regD, uint16& regS)
{
  // TODO
}

void
CPU::pop_r16(uint16& regS)
{
  // TODO
}

void
CPU::push_r16(uint16& regS)
{
  // TODO
}

// miscellaneous instructions
void
CPU::di()
{
  // TODO
}

void
CPU::ei()
{
  // TODO
}

void
CPU::halt()
{
  // TODO
}

void
CPU::daa()
{
  // TODO
}

void
CPU::nop()
{
  // TODO
}

void
CPU::stop()
{
  // TODO
}

void
CPU::prefix_cb()
{
  // TODO
}
