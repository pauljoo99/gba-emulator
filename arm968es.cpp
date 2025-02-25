#include "arm968es.h"
#include "arm_instructions.h"
#include <cstring>
#include <stdio.h>

namespace Emulator::Arm

{

inline uint32_t generateMask(uint8_t a, uint8_t b) {
  return ((1U << (b - a + 1)) - 1) << a;
}

struct CPSR_Flags {
  uint32_t M : 5;
  uint32_t T : 1;
  uint32_t F : 1;
  uint32_t I : 1;
  uint32_t reserve : 20;
  uint32_t V : 1;
  uint32_t C : 1;
  uint32_t Z : 1;
  uint32_t N : 1;
};

union CPSR_Register {
  uint32_t value;
  CPSR_Flags bits;

  CPSR_Register(uint32_t val = 0) : value(val) {}

  operator uint32_t() const { return value; } // Implicit conversion
};

[[nodiscard]] bool process_instr(uint32_t instr, Instr::Instr instr_type,
                                 const Memory::Memory &memory, CPU &cpu) {

  printf("Instr: %s, ", toString(instr_type));
  printf("Raw Instr: 0x%08X", instr);
  printf("\n");

  switch (instr_type) {
  case Instr::Instr::B:
    return cpu.dispatch_B(instr);
  case Instr::Instr::BX:
    return cpu.dispatch_BX(instr);
  case Instr::Instr::MOV:
    return cpu.dispatch_MOV(instr, memory);
  case Instr::Instr::MSR:
    return cpu.dispatch_MSR(instr);
  case Instr::Instr::LDR:
    return cpu.dispatch_LDR(instr, memory);
  case Instr::Instr::ADD:
    return cpu.dispatch_ADD(instr);
  case Instr::Instr::STR:
    return cpu.dispatch_STR(instr, memory);
  default:
    return false;
  }
}

[[nodiscard]] bool get_reg(uint8_t reg_num, Registers &regs, uint32_t *&reg) {

  switch (reg_num) {
  case 0:
    reg = &regs.r0;
    return true;
  case 1:
    reg = &regs.r1;
    return true;
  case 2:
    reg = &regs.r2;
    return true;
  case 3:
    reg = &regs.r3;
    return true;
  case 4:
    reg = &regs.r4;
    return true;
  case 5:
    reg = &regs.r5;
    return true;
  case 6:
    reg = &regs.r6;
    return true;
  case 7:
    reg = &regs.r7;
    return true;
  case 8:
    reg = &regs.r8;
    return true;
  case 9:
    reg = &regs.r9;
    return true;
  case 10:
    reg = &regs.r10;
    return true;
  case 11:
    reg = &regs.r11;
    return true;
  case 12:
    reg = &regs.r12;
    return true;
  case 13:
    reg = &regs.r13;
    return true;
  case 14:
    reg = &regs.r14;
    return true;
  case 15:
    reg = &regs.r15;
    return true;
  default:
    printf("Could not find register %u", reg_num);
    return false;
  }
}

bool evaluate_cond(ConditionCode cond, CPSR_Register cpsr) {
  switch (cond) {
  case ConditionCode::EQ:
    return cpsr.bits.Z;
  case ConditionCode::NE:
    return !cpsr.bits.Z;
  case ConditionCode::CS:
    return cpsr.bits.C;
  case ConditionCode::CC:
    return !cpsr.bits.C;
  case ConditionCode::MI:
    return cpsr.bits.N;
  case ConditionCode::PL:
    return !cpsr.bits.N;
  case ConditionCode::VS:
    return cpsr.bits.V;
  case ConditionCode::VC:
    return !cpsr.bits.V;
  case ConditionCode::HI:
    return cpsr.bits.C && !cpsr.bits.Z;
  case ConditionCode::LS:
    return !cpsr.bits.C && cpsr.bits.Z;
  case ConditionCode::GE:
    return cpsr.bits.N == cpsr.bits.V;
  case ConditionCode::LT:
    return cpsr.bits.N != cpsr.bits.V;
  case ConditionCode::GT:
    return !cpsr.bits.Z && cpsr.bits.N == cpsr.bits.V;
  case ConditionCode::LE:
    return cpsr.bits.Z || cpsr.bits.N != cpsr.bits.V;
  case ConditionCode::AL:
    return true;
  }
}

[[nodiscard]] bool get_spsr_reg(CPSR_Register cpsr, Registers &regs,
                                uint32_t *&reg) {
  switch (cpsr.bits.M) {
  case 0b10001:
    reg = &regs.SPSR_fiq;
    break;
  case 0b10010:
    reg = &regs.SPSR_irq;
    break;
  case 0b10011:
    reg = &regs.SPSR_svc;
    break;
  case 0b10111:
    reg = &regs.SPSR_abt;
    break;
  case 0b11011:
    reg = &regs.SPSR_und;
    break;
  default:
    return false;
  }
  return true;
}

[[nodiscard]] bool CPU::dispatch(const GameCard::GameCard &game_card,
                                 const Memory::Memory &memory) noexcept {

  if (registers.r15 > sizeof(game_card.mem) / sizeof(game_card.mem[0])) {
    return false;
  }

  uint32_t instr;
  memcpy(&instr, (void *)&game_card.mem[registers.r15], kInstrSize);

  Instr::Instr instr_type;
  if (!get_instr_type(instr, instr_type)) {
    return false;
  }

  return process_instr(instr, instr_type, memory, *this);
}

[[nodiscard]] bool CPU::dispatch_B(uint32_t instr_) noexcept {
  const BranchInstr instr(instr_);
  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r15 += kInstrSize;
    return true;
  }
  registers.r15 += (instr.fields.offset << 2) + 8;
  return true;
}

[[nodiscard]] bool CPU::dispatch_BX(uint32_t instr_) noexcept {
  const BranchAndExchangeInstr instr(instr_);
  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r15 += kInstrSize;
    return true;
  }
  thumb_instr = instr.fields.rn & 1;
  uint32_t *rn;
  if (!get_reg(instr.fields.rn, registers, rn)) {
    return false;
  }
  registers.r15 = *rn & ~1;
  return true;
}

[[nodiscard]] bool op2_register(uint32_t op2, Registers &registers,
                                bool &carry_out, uint32_t &op2_val) {

  uint32_t *rm;
  if (!get_reg(op2 & 0b1111, registers, rm)) {
    return false;
  }

  uint32_t shift_type = (op2 >> 5) & 0b11;
  uint32_t shift_amount;
  if ((op2 >> 4) & 1) {
    uint32_t *rs;
    if (!get_reg(op2 >> 8, registers, rs)) {
      return false;
    }
    shift_amount = *rs & 0xFF;
  } else {
    shift_amount = op2 >> 7;
  }

  uint32_t sign;
  switch (shift_type) {
  case 0:
    carry_out = *rm >> (32 - shift_amount) & 1;
    op2_val = *rm << shift_amount;
    break;
  case 1:
    carry_out = *rm >> (shift_amount - 1) & 1;
    op2_val = *rm >> shift_amount;
    break;
  case 2:
    carry_out = *rm >> (shift_amount - 1) & 1;
    sign = (*rm >> 31) & 1;
    op2_val = (*rm >> shift_amount) |
              (((sign << shift_amount) - 1) << (32 - shift_amount));
    break;
  case 3:
    printf("No implemented");
    return false;
  default:
    printf("Not a recognized shift type");
    return false;
  }

  return true;
}

[[nodiscard]] bool CPU::dispatch_MOV(uint32_t instr_,
                                     const Memory::Memory &memory) noexcept {
  const DataProcessingInstr instr(instr_);

  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r15 += kInstrSize;
    return true;
  }

  uint32_t *r = nullptr;
  if (!get_reg(instr.fields.rd, registers, r)) {
    return false;
  }

  if (!instr.fields.i) {
    uint32_t op2;
    bool carry_out = 0;
    if (!op2_register(instr.fields.operand_2, registers, carry_out, op2)) {
      return false;
    }
    *r = op2;
  } else {
    const uint32_t rotate =
        ((instr.fields.operand_2 & generateMask(8, 11)) >> 8) * 2;
    const uint8_t lmm = instr.fields.operand_2 & generateMask(0, 7);
    *r = (lmm >> rotate) | (lmm << (8 - rotate));
  }
  registers.r15 += kInstrSize;
  return true;
}

[[nodiscard]] bool CPU::dispatch_ADD(uint32_t instr_) noexcept {
  const DataProcessingInstr instr(instr_);

  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r15 += kInstrSize;
    return true;
  }

  uint32_t op2;
  bool carry_out = 0;
  if (!instr.fields.i) {
    if (!op2_register(instr.fields.operand_2, registers, carry_out, op2)) {
      return false;
    }
  } else {
    const uint32_t rotate =
        ((instr.fields.operand_2 & generateMask(8, 11)) >> 8) * 2;
    const uint8_t lmm = instr.fields.operand_2 & generateMask(0, 7);
    op2 = (lmm >> rotate) | (lmm << (8 - rotate));
    carry_out = (lmm >> (rotate - 1)) & 1;
  }

  uint32_t *rn = nullptr;
  if (!get_reg(instr.fields.rn, registers, rn)) {
    return false;
  }

  uint32_t *rd = nullptr;
  if (!get_reg(instr.fields.rd, registers, rd)) {
    return false;
  }

  *rd = *rn + op2;

  if (instr.fields.s) {
    CPSR_Register tmp_cprs = registers.CPSR;
    tmp_cprs.bits.V = *rd < *rn || *rd < op2;
    tmp_cprs.bits.C = carry_out;
    tmp_cprs.bits.Z = *rd == 0;
    tmp_cprs.bits.N = (*rd >> 31) & 1;
    registers.CPSR = tmp_cprs;
  }

  registers.r15 += kInstrSize;
  return true;
}

[[nodiscard]] bool CPU::dispatch_MSR(uint32_t instr_) noexcept {
  const MSRInstr instr(instr_);

  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r15 += kInstrSize;
    return true;
  }

  uint32_t *rm = nullptr;
  if (!get_reg(instr.fields.rm, registers, rm)) {
    return false;
  }
  if (!instr.fields.dest_psr) {
    registers.CPSR = *rm;
  } else {
    uint32_t *rs = nullptr;
    if (!get_spsr_reg(registers.CPSR, registers, rs)) {
      return false;
    }
    *rs = *rm;
  }
  registers.r15 += kInstrSize;
  return true;
}

[[nodiscard]] bool CPU::dispatch_LDR(uint32_t instr_,
                                     const Memory::Memory &memory) noexcept {
  const SingleDataTransferInstr instr{instr_};
  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r15 += kInstrSize;
    return true;
  }

  uint32_t *rd = nullptr;
  if (!get_reg(instr.fields.rd, registers, rd)) {
    printf("Could not get rd");
    return false;
  }

  uint32_t *rn = nullptr;
  if (!get_reg(instr.fields.rn, registers, rn)) {
    printf("Could not get rn");
    return false;
  }

  uint32_t offset;
  if (instr.fields.i == 0) {
    offset = instr.fields.offset;
  } else {
    printf("Not implemented");
    return false;
  }

  const uint32_t old_rn = *rn;
  if (instr.fields.p) {
    *rn += (instr.fields.u ? 1 : -1) * offset;
    memcpy(rd, (void *)&memory.mem[*rn], (instr.fields.u ? 1 : 4));
  } else {
    memcpy(rd, (void *)&memory.mem[*rn], (instr.fields.u ? 1 : 4));
    *rn += (instr.fields.u ? 1 : -1) * offset;
  }

  if (!instr.fields.w) {
    *rn = old_rn;
  }

  registers.r15 += kInstrSize;
  return true;
}

[[nodiscard]] bool CPU::dispatch_STR(uint32_t instr_,
                                     const Memory::Memory &memory) noexcept {
  const SingleDataTransferInstr instr{instr_};
  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r15 += kInstrSize;
    return true;
  }

  uint32_t *rd = nullptr;
  if (!get_reg(instr.fields.rd, registers, rd)) {
    printf("Could not get rd");
    return false;
  }

  uint32_t *rn = nullptr;
  if (!get_reg(instr.fields.rn, registers, rn)) {
    printf("Could not get rn");
    return false;
  }

  uint32_t offset;
  if (instr.fields.i == 0) {
    offset = instr.fields.offset;
  } else {
    printf("Not implemented");
    return false;
  }

  const uint32_t old_rn = *rn;
  if (instr.fields.p) {
    *rn += (instr.fields.u ? 1 : -1) * offset;
    memcpy((void *)&memory.mem[*rd], rn, (instr.fields.u ? 1 : 4));
  } else {
    memcpy((void *)&memory.mem[*rd], rn, (instr.fields.u ? 1 : 4));
    *rn += (instr.fields.u ? 1 : -1) * offset;
  }

  if (!instr.fields.w) {
    *rn = old_rn;
  }

  registers.r15 += kInstrSize;
  return true;
}

} // namespace Emulator::Arm
