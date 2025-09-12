#ifndef CPU_H
#define CPU_H

#include <functional>

#include "common.h"

class MMU;

class CPU
{
  friend class MMU;

public:
  CPU();
  void tick(uint64 Tcycle);
  void setMMU(MMU* mmu) { this->mmu = mmu; }

private:
  enum class RegisterBits
  {
    High,
    Low,
    Full
  };

  enum class FlagBits
  {
    Zero = 0x80,
    Subtract = 0x40,
    HalfCarry = 0x20,
    Carry = 0x10
  };

  enum class Condition
  {
    Zero,
    NotZero,
    Carry,
    NotCarry
  };

  enum class Ime
  {
    Enable,
    Disable,
    PendingEnable,
    RequestEnable
  };

  uint16 AF = 0x01B0;
  uint16 BC = 0x0013;
  uint16 DE = 0x00D8;
  uint16 HL = 0x014D;
  uint16 SP = 0xFFFE;
  uint16 PC = 0x0100;

  Ime ime = Ime::Disable;
  uint8 IER = 0;
  uint8 IFR = 0xE1;

  uint8 ioData = 0;
  uint8 highByte = 0;
  uint8 lowByte = 0;

  uint32 instruction_cycles = 0;
  bool halted = false;
  bool use_prefix_instruction = false;
  uint8 curr_opcode = 0;
  std::function<void()> current_instruction;

  MMU* mmu = nullptr;

  void initialize();
  void cycle();
  std::function<void()> fetchPrefixInstruction(uint8 opcode);
  void iduInc(uint16& reg, uint16 value = 1);
  void iduDec(uint16& reg, uint16 value = 1);

  void read(uint16 addr);
  void write(uint16 addr, uint8 val);

  void next();

  void setRegister(uint16& reg, uint16 val, RegisterBits register_bits);
  uint16 readRegister(uint16& reg, RegisterBits register_bits);

  void setFlag(FlagBits flag, bool value);
  bool getFlag(FlagBits flag);

  bool checkCondition(Condition flag);
  uint8 getInterrupts();
  void serviceInterrupt();

  // load instructions
  void ld_r8_r8(uint16& regD,
                RegisterBits regDB,
                uint16& regS,
                RegisterBits regSB);
  void ld_r8_n8(uint16& regD, RegisterBits regB);
  void ld_r16_n16(uint16& regD);
  void ld_ar16_r8(uint16& regD, uint16& regS, RegisterBits regSB);
  void ld_ar16_n8(uint16& regD);
  void ld_r8_ar16(uint16& regD, RegisterBits regDB, uint16& regS);
  void ld_a16_r8(uint16& regS, RegisterBits regSB);
  void ldh_a8_r8(uint16& regS, RegisterBits regSB);
  void ldh_ar8_r8(uint16& regD,
                  RegisterBits regDB,
                  uint16& regS,
                  RegisterBits regSB);
  void ld_r8_a16(uint16& regD, RegisterBits regDB);
  void ldh_r8_a8(uint16& regD, RegisterBits regDB);
  void ldh_r8_ar8(uint16& regD,
                  RegisterBits regDB,
                  uint16& regS,
                  RegisterBits regSB);
  void ld_ar16i_r8(uint16& regD, uint16& regS, RegisterBits regSB);
  void ld_ar16d_r8(uint16& regD, uint16& regS, RegisterBits regSB);
  void ld_r8_ar16i(uint16& regD, RegisterBits regDB, uint16& regS);
  void ld_r8_ar16d(uint16& regD, RegisterBits regDB, uint16& regS);

  // arithmetic instructions
  void adc_r8(uint16& regS, RegisterBits regSB);
  void adc_ar16(uint16& regS);
  void adc_n8();
  void add_r8(uint16& regS, RegisterBits regSB);
  void add_ar16(uint16& regS);
  void add_n8();
  void cp_r8(uint16& regS, RegisterBits regSB);
  void cp_ar16(uint16& regS);
  void cp_n8();
  void dec_r8(uint16& regS, RegisterBits regSB);
  void dec_ar16(uint16& regS);
  void inc_r8(uint16& regS, RegisterBits regSB);
  void inc_ar16(uint16& regS);
  void sbc_r8(uint16& regS, RegisterBits regSB);
  void sbc_ar16(uint16& regS);
  void sbc_n8();
  void sub_r8(uint16& regS, RegisterBits regSB);
  void sub_ar16(uint16& regS);
  void sub_n8();
  void add_r16_r16(uint16& regD, uint16& regS);
  void dec_r16(uint16& regS);
  void inc_r16(uint16& regS);

  // bitwise logic instructions
  void and_r8(uint16& regS, RegisterBits regSB);
  void and_ar16(uint16& regS);
  void and_n8();
  void cpl();
  void or_r8(uint16& regS, RegisterBits regSB);
  void or_ar16(uint16& regS);
  void or_n8();
  void xor_r8(uint16& regS, RegisterBits regSB);
  void xor_ar16(uint16& regS);
  void xor_n8();

  // bit manipulation instructions
  void bit_r8(uint16& regS, RegisterBits regSB, uint8 bit);
  void bit_ar16(uint16& regS, uint8 bit);
  void res_r8(uint16& regS, RegisterBits regSB, uint8 bit);
  void res_ar16(uint16& regS, uint8 bit);
  void set_r8(uint16& regS, RegisterBits regSB, uint8 bit);
  void set_ar16(uint16& regS, uint8 bit);

  // bit shift instructions
  void rl_r8(uint16& regS, RegisterBits regSB);
  void rl_ar16(uint16& regS);
  void rla();
  void rlc_r8(uint16& regS, RegisterBits regSB);
  void rlc_ar16(uint16& regS);
  void rlca();
  void rr_r8(uint16& regS, RegisterBits regSB);
  void rr_ar16(uint16& regS);
  void rra();
  void rrc_r8(uint16& regS, RegisterBits regSB);
  void rrc_ar16(uint16& regS);
  void rrca();
  void sla_r8(uint16& regS, RegisterBits regSB);
  void sla_ar16(uint16& regS);
  void sra_r8(uint16& regS, RegisterBits regSB);
  void sra_ar16(uint16& regS);
  void srl_r8(uint16& regS, RegisterBits regSB);
  void srl_ar16(uint16& regS);
  void swap_r8(uint16& regS, RegisterBits regSB);
  void swap_ar16(uint16& regS);

  // jumps and subroutine instructions
  void call_a16();
  void call_cc(Condition flag);
  void jp_r16(uint16& regS);
  void jp_a16();
  void jp_cc_a16(Condition flag);
  void jr_a8();
  void jr_cc_a8(Condition flag);
  void ret_cc(Condition flag);
  void ret();
  void reti();
  void rst();

  // carry flag instructions
  void ccf();
  void scf();

  // stack manipulation instructions
  void add_sp_e8();
  void ld_a16_sp();
  void ld_hl_sp_e8();
  void ld_r16_r16(uint16& regD, uint16& regS);
  void pop_r16(uint16& regS);
  void push_r16(uint16& regS);

  // miscellaneous instructions
  void di();
  void ei();
  void halt();
  void daa();
  void nop();
  void stop();
  void prefix_cb();

  const std::unordered_set<uint8> bad_opcodes = { 0xD3, 0xDB, 0xDD, 0xE3,
                                                  0xE4, 0xEB, 0xEC, 0xED,
                                                  0xF4, 0xFC, 0xFD };

  const std::unordered_map<uint8, std::function<void()>> opcode_map = {
    { 0x00, std::bind(&CPU::nop, this) },
    { 0x01, std::bind(&CPU::ld_r16_n16, this, std::ref(BC)) },
    { 0x02,
      std::bind(&CPU::ld_ar16_r8,
                this,
                std::ref(BC),
                std::ref(AF),
                RegisterBits::High) },
    { 0x03, std::bind(&CPU::inc_r16, this, std::ref(BC)) },
    { 0x04, std::bind(&CPU::inc_r8, this, std::ref(BC), RegisterBits::High) },
    { 0x05, std::bind(&CPU::dec_r8, this, std::ref(BC), RegisterBits::High) },
    { 0x06, std::bind(&CPU::ld_r8_n8, this, std::ref(BC), RegisterBits::High) },
    { 0x07, std::bind(&CPU::rlca, this) },
    { 0x08, std::bind(&CPU::ld_a16_sp, this) },
    { 0x09, std::bind(&CPU::add_r16_r16, this, std::ref(HL), std::ref(BC)) },
    { 0x0A,
      std::bind(&CPU::ld_r8_ar16,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(BC)) },
    { 0x0B, std::bind(&CPU::dec_r16, this, std::ref(BC)) },
    { 0x0C, std::bind(&CPU::inc_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0x0D, std::bind(&CPU::dec_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0x0E, std::bind(&CPU::ld_r8_n8, this, std::ref(BC), RegisterBits::Low) },
    { 0x0F, std::bind(&CPU::rrca, this) },

    { 0x10, std::bind(&CPU::stop, this) },
    { 0x11, std::bind(&CPU::ld_r16_n16, this, std::ref(DE)) },
    { 0x12,
      std::bind(&CPU::ld_ar16_r8,
                this,
                std::ref(DE),
                std::ref(AF),
                RegisterBits::High) },
    { 0x13, std::bind(&CPU::inc_r16, this, std::ref(DE)) },
    { 0x14, std::bind(&CPU::inc_r8, this, std::ref(DE), RegisterBits::High) },
    { 0x15, std::bind(&CPU::dec_r8, this, std::ref(DE), RegisterBits::High) },
    { 0x16, std::bind(&CPU::ld_r8_n8, this, std::ref(DE), RegisterBits::High) },
    { 0x17, std::bind(&CPU::rla, this) },
    { 0x18, std::bind(&CPU::jr_a8, this) },
    { 0x19, std::bind(&CPU::add_r16_r16, this, std::ref(HL), std::ref(DE)) },
    { 0x1A,
      std::bind(&CPU::ld_r8_ar16,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(DE)) },
    { 0x1B, std::bind(&CPU::dec_r16, this, std::ref(DE)) },
    { 0x1C, std::bind(&CPU::inc_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0x1D, std::bind(&CPU::dec_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0x1E, std::bind(&CPU::ld_r8_n8, this, std::ref(DE), RegisterBits::Low) },
    { 0x1F, std::bind(&CPU::rra, this) },

    { 0x20, std::bind(&CPU::jr_cc_a8, this, Condition::NotZero) },
    { 0x21, std::bind(&CPU::ld_r16_n16, this, std::ref(HL)) },
    { 0x22,
      std::bind(&CPU::ld_ar16i_r8,
                this,
                std::ref(HL),
                std::ref(AF),
                RegisterBits::High) },
    { 0x23, std::bind(&CPU::inc_r16, this, std::ref(HL)) },
    { 0x24, std::bind(&CPU::inc_r8, this, std::ref(HL), RegisterBits::High) },
    { 0x25, std::bind(&CPU::dec_r8, this, std::ref(HL), RegisterBits::High) },
    { 0x26, std::bind(&CPU::ld_r8_n8, this, std::ref(HL), RegisterBits::High) },
    { 0x27, std::bind(&CPU::daa, this) },
    { 0x28, std::bind(&CPU::jr_cc_a8, this, Condition::Zero) },
    { 0x29, std::bind(&CPU::add_r16_r16, this, std::ref(HL), std::ref(HL)) },
    { 0x2A,
      std::bind(&CPU::ld_r8_ar16i,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(HL)) },
    { 0x2B, std::bind(&CPU::dec_r16, this, std::ref(HL)) },
    { 0x2C, std::bind(&CPU::inc_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0x2D, std::bind(&CPU::dec_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0x2E, std::bind(&CPU::ld_r8_n8, this, std::ref(HL), RegisterBits::Low) },
    { 0x2F, std::bind(&CPU::cpl, this) },

    { 0x30, std::bind(&CPU::jr_cc_a8, this, Condition::NotCarry) },
    { 0x31, std::bind(&CPU::ld_r16_n16, this, std::ref(SP)) },
    { 0x32,
      std::bind(&CPU::ld_ar16d_r8,
                this,
                std::ref(HL),
                std::ref(AF),
                RegisterBits::High) },
    { 0x33, std::bind(&CPU::inc_r16, this, std::ref(SP)) },
    { 0x34, std::bind(&CPU::inc_ar16, this, std::ref(HL)) },
    { 0x35, std::bind(&CPU::dec_ar16, this, std::ref(HL)) },
    { 0x36, std::bind(&CPU::ld_ar16_n8, this, std::ref(HL)) },
    { 0x37, std::bind(&CPU::scf, this) },
    { 0x38, std::bind(&CPU::jr_cc_a8, this, Condition::Carry) },
    { 0x39, std::bind(&CPU::add_r16_r16, this, std::ref(HL), std::ref(SP)) },
    { 0x3A,
      std::bind(&CPU::ld_r8_ar16d,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(HL)) },
    { 0x3B, std::bind(&CPU::dec_r16, this, std::ref(SP)) },
    { 0x3C, std::bind(&CPU::inc_r8, this, std::ref(AF), RegisterBits::High) },
    { 0x3D, std::bind(&CPU::dec_r8, this, std::ref(AF), RegisterBits::High) },
    { 0x3E, std::bind(&CPU::ld_r8_n8, this, std::ref(AF), RegisterBits::High) },
    { 0x3F, std::bind(&CPU::ccf, this) },

    { 0x40,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::High,
                std::ref(BC),
                RegisterBits::High) },
    { 0x41,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::High,
                std::ref(BC),
                RegisterBits::Low) },
    { 0x42,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::High,
                std::ref(DE),
                RegisterBits::High) },
    { 0x43,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::High,
                std::ref(DE),
                RegisterBits::Low) },
    { 0x44,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::High,
                std::ref(HL),
                RegisterBits::High) },
    { 0x45,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::High,
                std::ref(HL),
                RegisterBits::Low) },
    { 0x46,
      std::bind(&CPU::ld_r8_ar16,
                this,
                std::ref(BC),
                RegisterBits::High,
                std::ref(HL)) },
    { 0x47,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::High,
                std::ref(AF),
                RegisterBits::High) },
    { 0x48,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::Low,
                std::ref(BC),
                RegisterBits::High) },
    { 0x49,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::Low,
                std::ref(BC),
                RegisterBits::Low) },
    { 0x4A,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::Low,
                std::ref(DE),
                RegisterBits::High) },
    { 0x4B,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::Low,
                std::ref(DE),
                RegisterBits::Low) },
    { 0x4C,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::Low,
                std::ref(HL),
                RegisterBits::High) },
    { 0x4D,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::Low,
                std::ref(HL),
                RegisterBits::Low) },
    { 0x4E,
      std::bind(&CPU::ld_r8_ar16,
                this,
                std::ref(BC),
                RegisterBits::Low,
                std::ref(HL)) },
    { 0x4F,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(BC),
                RegisterBits::Low,
                std::ref(AF),
                RegisterBits::High) },

    { 0x50,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::High,
                std::ref(BC),
                RegisterBits::High) },
    { 0x51,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::High,
                std::ref(BC),
                RegisterBits::Low) },
    { 0x52,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::High,
                std::ref(DE),
                RegisterBits::High) },
    { 0x53,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::High,
                std::ref(DE),
                RegisterBits::Low) },
    { 0x54,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::High,
                std::ref(HL),
                RegisterBits::High) },
    { 0x55,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::High,
                std::ref(HL),
                RegisterBits::Low) },
    { 0x56,
      std::bind(&CPU::ld_r8_ar16,
                this,
                std::ref(DE),
                RegisterBits::High,
                std::ref(HL)) },
    { 0x57,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::High,
                std::ref(AF),
                RegisterBits::High) },
    { 0x58,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::Low,
                std::ref(BC),
                RegisterBits::High) },
    { 0x59,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::Low,
                std::ref(BC),
                RegisterBits::Low) },
    { 0x5A,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::Low,
                std::ref(DE),
                RegisterBits::High) },
    { 0x5B,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::Low,
                std::ref(DE),
                RegisterBits::Low) },
    { 0x5C,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::Low,
                std::ref(HL),
                RegisterBits::High) },
    { 0x5D,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::Low,
                std::ref(HL),
                RegisterBits::Low) },
    { 0x5E,
      std::bind(&CPU::ld_r8_ar16,
                this,
                std::ref(DE),
                RegisterBits::Low,
                std::ref(HL)) },
    { 0x5F,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(DE),
                RegisterBits::Low,
                std::ref(AF),
                RegisterBits::High) },

    { 0x60,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::High,
                std::ref(BC),
                RegisterBits::High) },
    { 0x61,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::High,
                std::ref(BC),
                RegisterBits::Low) },
    { 0x62,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::High,
                std::ref(DE),
                RegisterBits::High) },
    { 0x63,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::High,
                std::ref(DE),
                RegisterBits::Low) },
    { 0x64,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::High,
                std::ref(HL),
                RegisterBits::High) },
    { 0x65,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::High,
                std::ref(HL),
                RegisterBits::Low) },
    { 0x66,
      std::bind(&CPU::ld_r8_ar16,
                this,
                std::ref(HL),
                RegisterBits::High,
                std::ref(HL)) },
    { 0x67,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::High,
                std::ref(AF),
                RegisterBits::High) },
    { 0x68,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::Low,
                std::ref(BC),
                RegisterBits::High) },
    { 0x69,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::Low,
                std::ref(BC),
                RegisterBits::Low) },
    { 0x6A,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::Low,
                std::ref(DE),
                RegisterBits::High) },
    { 0x6B,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::Low,
                std::ref(DE),
                RegisterBits::Low) },
    { 0x6C,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::Low,
                std::ref(HL),
                RegisterBits::High) },
    { 0x6D,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::Low,
                std::ref(HL),
                RegisterBits::Low) },
    { 0x6E,
      std::bind(&CPU::ld_r8_ar16,
                this,
                std::ref(HL),
                RegisterBits::Low,
                std::ref(HL)) },
    { 0x6F,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(HL),
                RegisterBits::Low,
                std::ref(AF),
                RegisterBits::High) },

    { 0x70,
      std::bind(&CPU::ld_ar16_r8,
                this,
                std::ref(HL),
                std::ref(BC),
                RegisterBits::High) },
    { 0x71,
      std::bind(&CPU::ld_ar16_r8,
                this,
                std::ref(HL),
                std::ref(BC),
                RegisterBits::Low) },
    { 0x72,
      std::bind(&CPU::ld_ar16_r8,
                this,
                std::ref(HL),
                std::ref(DE),
                RegisterBits::High) },
    { 0x73,
      std::bind(&CPU::ld_ar16_r8,
                this,
                std::ref(HL),
                std::ref(DE),
                RegisterBits::Low) },
    { 0x74,
      std::bind(&CPU::ld_ar16_r8,
                this,
                std::ref(HL),
                std::ref(HL),
                RegisterBits::High) },
    { 0x75,
      std::bind(&CPU::ld_ar16_r8,
                this,
                std::ref(HL),
                std::ref(HL),
                RegisterBits::Low) },
    { 0x76, std::bind(&CPU::halt, this) },
    { 0x77,
      std::bind(&CPU::ld_ar16_r8,
                this,
                std::ref(HL),
                std::ref(AF),
                RegisterBits::High) },
    { 0x78,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(BC),
                RegisterBits::High) },
    { 0x79,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(BC),
                RegisterBits::Low) },
    { 0x7A,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(DE),
                RegisterBits::High) },
    { 0x7B,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(DE),
                RegisterBits::Low) },
    { 0x7C,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(HL),
                RegisterBits::High) },
    { 0x7D,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(HL),
                RegisterBits::Low) },
    { 0x7E,
      std::bind(&CPU::ld_r8_ar16,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(HL)) },
    { 0x7F,
      std::bind(&CPU::ld_r8_r8,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(AF),
                RegisterBits::High) },

    { 0x80, std::bind(&CPU::add_r8, this, std::ref(BC), RegisterBits::High) },
    { 0x81, std::bind(&CPU::add_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0x82, std::bind(&CPU::add_r8, this, std::ref(DE), RegisterBits::High) },
    { 0x83, std::bind(&CPU::add_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0x84, std::bind(&CPU::add_r8, this, std::ref(HL), RegisterBits::High) },
    { 0x85, std::bind(&CPU::add_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0x86, std::bind(&CPU::add_ar16, this, std::ref(HL)) },
    { 0x87, std::bind(&CPU::add_r8, this, std::ref(AF), RegisterBits::High) },
    { 0x88, std::bind(&CPU::adc_r8, this, std::ref(BC), RegisterBits::High) },
    { 0x89, std::bind(&CPU::adc_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0x8A, std::bind(&CPU::adc_r8, this, std::ref(DE), RegisterBits::High) },
    { 0x8B, std::bind(&CPU::adc_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0x8C, std::bind(&CPU::adc_r8, this, std::ref(HL), RegisterBits::High) },
    { 0x8D, std::bind(&CPU::adc_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0x8E, std::bind(&CPU::adc_ar16, this, std::ref(HL)) },
    { 0x8F, std::bind(&CPU::adc_r8, this, std::ref(AF), RegisterBits::High) },

    { 0x90, std::bind(&CPU::sub_r8, this, std::ref(BC), RegisterBits::High) },
    { 0x91, std::bind(&CPU::sub_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0x92, std::bind(&CPU::sub_r8, this, std::ref(DE), RegisterBits::High) },
    { 0x93, std::bind(&CPU::sub_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0x94, std::bind(&CPU::sub_r8, this, std::ref(HL), RegisterBits::High) },
    { 0x95, std::bind(&CPU::sub_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0x96, std::bind(&CPU::sub_ar16, this, std::ref(HL)) },
    { 0x97, std::bind(&CPU::sub_r8, this, std::ref(AF), RegisterBits::High) },
    { 0x98, std::bind(&CPU::sbc_r8, this, std::ref(BC), RegisterBits::High) },
    { 0x99, std::bind(&CPU::sbc_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0x9A, std::bind(&CPU::sbc_r8, this, std::ref(DE), RegisterBits::High) },
    { 0x9B, std::bind(&CPU::sbc_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0x9C, std::bind(&CPU::sbc_r8, this, std::ref(HL), RegisterBits::High) },
    { 0x9D, std::bind(&CPU::sbc_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0x9E, std::bind(&CPU::sbc_ar16, this, std::ref(HL)) },
    { 0x9F, std::bind(&CPU::sbc_r8, this, std::ref(AF), RegisterBits::High) },

    { 0xA0, std::bind(&CPU::and_r8, this, std::ref(BC), RegisterBits::High) },
    { 0xA1, std::bind(&CPU::and_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0xA2, std::bind(&CPU::and_r8, this, std::ref(DE), RegisterBits::High) },
    { 0xA3, std::bind(&CPU::and_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0xA4, std::bind(&CPU::and_r8, this, std::ref(HL), RegisterBits::High) },
    { 0xA5, std::bind(&CPU::and_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0xA6, std::bind(&CPU::and_ar16, this, std::ref(HL)) },
    { 0xA7, std::bind(&CPU::and_r8, this, std::ref(AF), RegisterBits::High) },
    { 0xA8, std::bind(&CPU::xor_r8, this, std::ref(BC), RegisterBits::High) },
    { 0xA9, std::bind(&CPU::xor_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0xAA, std::bind(&CPU::xor_r8, this, std::ref(DE), RegisterBits::High) },
    { 0xAB, std::bind(&CPU::xor_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0xAC, std::bind(&CPU::xor_r8, this, std::ref(HL), RegisterBits::High) },
    { 0xAD, std::bind(&CPU::xor_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0xAE, std::bind(&CPU::xor_ar16, this, std::ref(HL)) },
    { 0xAF, std::bind(&CPU::xor_r8, this, std::ref(AF), RegisterBits::High) },

    { 0xB0, std::bind(&CPU::or_r8, this, std::ref(BC), RegisterBits::High) },
    { 0xB1, std::bind(&CPU::or_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0xB2, std::bind(&CPU::or_r8, this, std::ref(DE), RegisterBits::High) },
    { 0xB3, std::bind(&CPU::or_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0xB4, std::bind(&CPU::or_r8, this, std::ref(HL), RegisterBits::High) },
    { 0xB5, std::bind(&CPU::or_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0xB6, std::bind(&CPU::or_ar16, this, std::ref(HL)) },
    { 0xB7, std::bind(&CPU::or_r8, this, std::ref(AF), RegisterBits::High) },
    { 0xB8, std::bind(&CPU::cp_r8, this, std::ref(BC), RegisterBits::High) },
    { 0xB9, std::bind(&CPU::cp_r8, this, std::ref(BC), RegisterBits::Low) },
    { 0xBA, std::bind(&CPU::cp_r8, this, std::ref(DE), RegisterBits::High) },
    { 0xBB, std::bind(&CPU::cp_r8, this, std::ref(DE), RegisterBits::Low) },
    { 0xBC, std::bind(&CPU::cp_r8, this, std::ref(HL), RegisterBits::High) },
    { 0xBD, std::bind(&CPU::cp_r8, this, std::ref(HL), RegisterBits::Low) },
    { 0xBE, std::bind(&CPU::cp_ar16, this, std::ref(HL)) },
    { 0xBF, std::bind(&CPU::cp_r8, this, std::ref(AF), RegisterBits::High) },

    { 0xC0, std::bind(&CPU::ret_cc, this, Condition::NotZero) },
    { 0xC1, std::bind(&CPU::pop_r16, this, std::ref(BC)) },
    { 0xC2, std::bind(&CPU::jp_cc_a16, this, Condition::NotZero) },
    { 0xC3, std::bind(&CPU::jp_a16, this) },
    { 0xC4, std::bind(&CPU::call_cc, this, Condition::NotZero) },
    { 0xC5, std::bind(&CPU::push_r16, this, std::ref(BC)) },
    { 0xC6, std::bind(&CPU::add_n8, this) },
    { 0xC7, std::bind(&CPU::rst, this) },
    { 0xC8, std::bind(&CPU::ret_cc, this, Condition::Zero) },
    { 0xC9, std::bind(&CPU::ret, this) },
    { 0xCA, std::bind(&CPU::jp_cc_a16, this, Condition::Zero) },
    { 0xCB, std::bind(&CPU::prefix_cb, this) },
    { 0xCC, std::bind(&CPU::call_cc, this, Condition::Zero) },
    { 0xCD, std::bind(&CPU::call_a16, this) },
    { 0xCE, std::bind(&CPU::adc_n8, this) },
    { 0xCF, std::bind(&CPU::rst, this) },

    { 0xD0, std::bind(&CPU::ret_cc, this, Condition::NotCarry) },
    { 0xD1, std::bind(&CPU::pop_r16, this, std::ref(DE)) },
    { 0xD2, std::bind(&CPU::jp_cc_a16, this, Condition::NotCarry) },
    { 0xD4, std::bind(&CPU::call_cc, this, Condition::NotCarry) },
    { 0xD5, std::bind(&CPU::push_r16, this, std::ref(DE)) },
    { 0xD6, std::bind(&CPU::sub_n8, this) },
    { 0xD7, std::bind(&CPU::rst, this) },
    { 0xD8, std::bind(&CPU::ret_cc, this, Condition::Carry) },
    { 0xD9, std::bind(&CPU::reti, this) },
    { 0xDA, std::bind(&CPU::jp_cc_a16, this, Condition::Carry) },
    { 0xDC, std::bind(&CPU::call_cc, this, Condition::Carry) },
    { 0xDE, std::bind(&CPU::sbc_n8, this) },
    { 0xDF, std::bind(&CPU::rst, this) },

    { 0xE0,
      std::bind(&CPU::ldh_a8_r8, this, std::ref(AF), RegisterBits::High) },
    { 0xE1, std::bind(&CPU::pop_r16, this, std::ref(HL)) },
    { 0xE2,
      std::bind(&CPU::ldh_ar8_r8,
                this,
                std::ref(BC),
                RegisterBits::Low,
                std::ref(AF),
                RegisterBits::High) },
    { 0xE5, std::bind(&CPU::push_r16, this, std::ref(HL)) },
    { 0xE6, std::bind(&CPU::and_n8, this) },
    { 0xE7, std::bind(&CPU::rst, this) },
    { 0xE8, std::bind(&CPU::add_sp_e8, this) },
    { 0xE9, std::bind(&CPU::jp_r16, this, std::ref(HL)) },
    { 0xEA,
      std::bind(&CPU::ld_a16_r8, this, std::ref(AF), RegisterBits::High) },
    { 0xEE, std::bind(&CPU::xor_n8, this) },
    { 0xEF, std::bind(&CPU::rst, this) },

    { 0xF0,
      std::bind(&CPU::ldh_r8_a8, this, std::ref(AF), RegisterBits::High) },
    { 0xF1, std::bind(&CPU::pop_r16, this, std::ref(AF)) },
    { 0xF2,
      std::bind(&CPU::ldh_r8_ar8,
                this,
                std::ref(AF),
                RegisterBits::High,
                std::ref(BC),
                RegisterBits::Low) },
    { 0xF3, std::bind(&CPU::di, this) },
    { 0xF5, std::bind(&CPU::push_r16, this, std::ref(AF)) },
    { 0xF6, std::bind(&CPU::or_n8, this) },
    { 0xF7, std::bind(&CPU::rst, this) },
    { 0xF8, std::bind(&CPU::ld_hl_sp_e8, this) },
    { 0xF9, std::bind(&CPU::ld_r16_r16, this, std::ref(SP), std::ref(HL)) },
    { 0xFA,
      std::bind(&CPU::ld_r8_a16, this, std::ref(AF), RegisterBits::High) },
    { 0xFB, std::bind(&CPU::ei, this) },
    { 0xFE, std::bind(&CPU::cp_n8, this) },
    { 0xFF, std::bind(&CPU::rst, this) }
  };
};

#endif // CPU_H
