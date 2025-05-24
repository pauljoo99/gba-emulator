#include "arm968es.h"
#include "arm_instructions.h"
#include "snapshot.h"
#include "thumb2_instructions.h"
#include "thumb_instructions.h"
#include <cstring>
#include <stdio.h>

namespace Emulator::Arm

{

Thumb::ThumbOpcode get_thumb_instruction(U16 instr) {
  switch (instr >> 13) {
  case (0):
    switch ((instr >> 11) & 0b11) {
    case (0b00):
      return Thumb::ThumbOpcode::LSL;
    case (0b01):
      return Thumb::ThumbOpcode::LSR;
    case (0b10):
      return Thumb::ThumbOpcode::ASR;
    case (0b11):
      if (instr >> 10 == 0) {
        return Thumb::ThumbOpcode::ADD;
      } else {
        return Thumb::ThumbOpcode::SUB;
      }
    }

  case (1):
    switch ((instr >> 11) & 0b11) {
    case (0b00):
      return Thumb::ThumbOpcode::MOV;
    case (0b01):
      return Thumb::ThumbOpcode::CMP;
    case (0b10):
      return Thumb::ThumbOpcode::ADD;
    case (0b11):
      return Thumb::ThumbOpcode::SUB;
    }

  case (2):
    switch ((instr >> 10) & 0b111) {
    case (0b000):
      switch ((instr >> 6) & 0b1111) {
      case (0b0000):
        return Thumb::ThumbOpcode::AND;
      case (0b0001):
        return Thumb::ThumbOpcode::EOR;
      case (0b0010):
        return Thumb::ThumbOpcode::LSL;
      case (0b0011):
        return Thumb::ThumbOpcode::LSR;
      case (0b0100):
        return Thumb::ThumbOpcode::ASR;
      case (0b0101):
        return Thumb::ThumbOpcode::ADC;
      case (0b0110):
        return Thumb::ThumbOpcode::SBC;
      case (0b0111):
        return Thumb::ThumbOpcode::ROR;
      case (0b1000):
        return Thumb::ThumbOpcode::TST;
      case (0b1001):
        return Thumb::ThumbOpcode::NEG;
      case (0b1010):
        return Thumb::ThumbOpcode::CMP;
      case (0b1011):
        return Thumb::ThumbOpcode::CMN;
      case (0b1100):
        return Thumb::ThumbOpcode::ORR;
      case (0b1101):
        return Thumb::ThumbOpcode::MUL;
      case (0b1110):
        return Thumb::ThumbOpcode::BIC;
      case (0b1111):
        return Thumb::ThumbOpcode::MVN;
      }
    case (0b001):
      switch ((instr >> 8) & 0b11) {
      case (0b00):
        return Thumb::ThumbOpcode::ADD;
      case (0b01):
        return Thumb::ThumbOpcode::CMP;
      case (0b10):
        return Thumb::ThumbOpcode::MOV;
      case (0b11):
        return Thumb::ThumbOpcode::BX;
      }
    case (0b010):
    case (0b011):
      return Thumb::ThumbOpcode::LDR;
    case (0b100):
    case (0b101):
    case (0b110):
    case (0b111):
      switch ((instr >> 9) & 0b111) {
      case (0b000):
        return Thumb::ThumbOpcode::STR;
      case (0b010):
        return Thumb::ThumbOpcode::STRB;
      case (0b100):
        return Thumb::ThumbOpcode::LDR;
      case (0b110):
        return Thumb::ThumbOpcode::LDRB;
      case (0b001):
        return Thumb::ThumbOpcode::STRH;
      case (0b011):
        return Thumb::ThumbOpcode::LDRH;
      case (0b101):
        return Thumb::ThumbOpcode::LDSB;
      case (0b111):
        return Thumb::ThumbOpcode::LDSH;
      }
    }

  case (3):
    switch ((instr >> 11) & 0b11) {
    case (0b00):
      return Thumb::ThumbOpcode::STR;
    case (0b10):
      return Thumb::ThumbOpcode::LDR;
    case (0b01):
      return Thumb::ThumbOpcode::STRB;
    case (0b11):
      return Thumb::ThumbOpcode::LDRB;
    }

  case (4):
    switch ((instr >> 11) & 0b11) {
    case (0b00):
      return Thumb::ThumbOpcode::STRH;
    case (0b01):
      return Thumb::ThumbOpcode::LDRH;
    case (0b10):
      return Thumb::ThumbOpcode::STR;
    case (0b11):
      return Thumb::ThumbOpcode::LDR;
    }

  case (5):
    switch ((instr >> 11) & 0b11) {
    case (0b00):
      return Thumb::ThumbOpcode::ADD;
    case (0b01):
      return Thumb::ThumbOpcode::ADD;
    case (0b10):
      if (((instr >> 8) & 0b1111) == 0) {
        return Thumb::ThumbOpcode::ADD;
      } else if (((instr >> 8) & 0b1111) == 0b1010) {
        return Thumb::ThumbOpcode::PUSH;
      } else if (((instr >> 8) & 0b1111) == 0b1110) {
        return Thumb::ThumbOpcode::POP;
      }
    default:
      break;
    }

  case (6):
    switch ((instr >> 11) & 0b11) {
    case (0b00):
      return Thumb::ThumbOpcode::STMIA;
    case (0b01):
      return Thumb::ThumbOpcode::LDMIA;
    case (0b10):
    case (0b11):
      if (((instr >> 8) & 0b11111) == 0b11111) {
        return Thumb::ThumbOpcode::SWI;
      }
      return Thumb::ThumbOpcode::BXX;
    default:
      break;
    }

  case (7):
    switch ((instr >> 11) & 0b11) {
    case (0b00):
      return Thumb::ThumbOpcode::B;
    case (0b01):
      return Thumb::ThumbOpcode::BLX;
    case (0b10):
    case (0b11):
      return Thumb::ThumbOpcode::BL;
    default:
      break;
    }
  default:
    break;
  }
  return Thumb::ThumbOpcode::UNDEFINED;
}

inline U32 generateMask(U8 a, U8 b) { return ((1U << (b - a + 1)) - 1) << a; }

struct CPSR_Flags {
  U32 M : 5;
  U32 T : 1;
  U32 F : 1;
  U32 I : 1;
  U32 reserve : 20;
  U32 V : 1;
  U32 C : 1;
  U32 Z : 1;
  U32 N : 1;
};

union CPSR_Register {
  U32 value;
  CPSR_Flags bits;

  CPSR_Register(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

[[nodiscard]] bool process_instr(U32 instr, Instr::Instr instr_type,
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

[[nodiscard]] bool process_thumb(U16 instr, const Memory::Memory &memory,
                                 CPU &cpu) {

  printf("Raw Instr: 0x%04X, ", instr);

  Thumb::ThumbOpcode opcode = get_thumb_instruction(instr);

  printf("Instr: %s", toString(opcode));
  printf("\n");

  switch (opcode) {
  case (Thumb::ThumbOpcode::LSL):
    return cpu.dispatch_thumb_LSL(instr);
  case (Thumb::ThumbOpcode::BLX):
    return cpu.dispatch_thumb_BLX(instr);
  default:
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
                                U32 *&reg) {
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

  Debug::debug_snapshot(registers, memory, game_card, "tools/visual/data/");

  if (registers.r[15] > sizeof(game_card.mem) / sizeof(game_card.mem[0])) {
    return false;
  }

  if (thumb_instr) {
    U16 instr;
    memcpy(&instr, (void *)&game_card.mem[registers.r[15]], kThumbInstrSize);
    return process_thumb(instr, memory, *this);
  } else {
    U32 instr;
    memcpy(&instr, (void *)&game_card.mem[registers.r[15]], kInstrSize);
    Instr::Instr instr_type;
    if (!get_instr_type(instr, instr_type)) {
      return false;
    }
    return process_instr(instr, instr_type, memory, *this);
  }
}

[[nodiscard]] bool CPU::dispatch_B(U32 instr_) noexcept {
  const BranchInstr instr(instr_);
  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r[15] += kInstrSize;
    return true;
  }
  registers.r[15] += (instr.fields.offset << 2) + 8;
  return true;
}

[[nodiscard]] bool CPU::dispatch_BX(U32 instr_) noexcept {
  const BranchAndExchangeInstr instr(instr_);
  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r[15] += kInstrSize;
    return true;
  }
  thumb_instr = instr.fields.rn & 1;
  U32 &rn = registers.r[instr.fields.rn];
  registers.r[15] = rn & ~1;
  return true;
}

[[nodiscard]] bool op2_register(U32 op2, Registers &registers, bool &carry_out,
                                U32 &op2_val) {

  U32 &rm = registers.r[op2 & 0b1111];

  U32 shift_type = (op2 >> 5) & 0b11;
  U32 shift_amount;
  if ((op2 >> 4) & 1) {
    U32 &rs = registers.r[op2 >> 8];
    shift_amount = rs & 0xFF;
  } else {
    shift_amount = op2 >> 7;
  }

  U32 sign;
  switch (shift_type) {
  case 0:
    carry_out = rm >> (32 - shift_amount) & 1;
    op2_val = rm << shift_amount;
    break;
  case 1:
    carry_out = rm >> (shift_amount - 1) & 1;
    op2_val = rm >> shift_amount;
    break;
  case 2:
    carry_out = rm >> (shift_amount - 1) & 1;
    sign = (rm >> 31) & 1;
    op2_val = (rm >> shift_amount) |
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

[[nodiscard]] bool CPU::dispatch_MOV(U32 instr_,
                                     const Memory::Memory &memory) noexcept {
  const DataProcessingInstr instr(instr_);

  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r[15] += kInstrSize;
    return true;
  }

  U32 &r = registers.r[instr.fields.rd];
  if (!instr.fields.i) {
    U32 op2;
    bool carry_out = 0;
    if (!op2_register(instr.fields.operand_2, registers, carry_out, op2)) {
      return false;
    }
    r = op2;
  } else {
    const U32 rotate =
        ((instr.fields.operand_2 & generateMask(8, 11)) >> 8) * 2;
    const U8 lmm = instr.fields.operand_2 & generateMask(0, 7);
    r = (lmm >> rotate) | (lmm << (8 - rotate));
  }
  registers.r[15] += kInstrSize;
  return true;
}

[[nodiscard]] bool CPU::dispatch_ADD(U32 instr_) noexcept {
  const DataProcessingInstr instr(instr_);

  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r[15] += kInstrSize;
    return true;
  }

  U32 op2;
  bool carry_out = 0;
  if (!instr.fields.i) {
    if (!op2_register(instr.fields.operand_2, registers, carry_out, op2)) {
      return false;
    }
  } else {
    const U32 rotate =
        ((instr.fields.operand_2 & generateMask(8, 11)) >> 8) * 2;
    const U8 lmm = instr.fields.operand_2 & generateMask(0, 7);
    op2 = (lmm >> rotate) | (lmm << (8 - rotate));
    carry_out = (lmm >> (rotate - 1)) & 1;
  }

  U32 &rn = registers.r[instr.fields.rn];
  U32 &rd = registers.r[instr.fields.rd];

  rd = rn + op2;

  if (instr.fields.s) {
    CPSR_Register tmp_cprs = registers.CPSR;
    tmp_cprs.bits.V = rd < rn || rd < op2;
    tmp_cprs.bits.C = carry_out;
    tmp_cprs.bits.Z = rd == 0;
    tmp_cprs.bits.N = (rd >> 31) & 1;
    registers.CPSR = tmp_cprs;
  }

  registers.r[15] += kInstrSize;
  return true;
}

[[nodiscard]] bool CPU::dispatch_MSR(U32 instr_) noexcept {
  const MSRInstr instr(instr_);

  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r[15] += kInstrSize;
    return true;
  }

  U32 &rm = registers.r[instr.fields.rm];
  if (!instr.fields.dest_psr) {
    registers.CPSR = rm;
  } else {
    U32 *rs = nullptr;
    if (!get_spsr_reg(registers.CPSR, registers, rs)) {
      return false;
    }
    *rs = rm;
  }
  registers.r[15] += kInstrSize;
  return true;
}

[[nodiscard]] bool CPU::dispatch_LDR(U32 instr_,
                                     const Memory::Memory &memory) noexcept {
  const SingleDataTransferInstr instr{instr_};
  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r[15] += kInstrSize;
    return true;
  }

  U32 &rd = registers.r[instr.fields.rd];
  U32 &rn = registers.r[instr.fields.rn];

  U32 offset;
  if (instr.fields.i == 0) {
    offset = instr.fields.offset;
  } else {
    printf("Not implemented");
    return false;
  }

  const U32 old_rn = rn;
  if (instr.fields.p) {
    rn += (instr.fields.u ? 1 : -1) * offset;
    memcpy(&rd, (void *)&memory.mem[rn], (instr.fields.u ? 1 : 4));
  } else {
    memcpy(&rd, (void *)&memory.mem[rn], (instr.fields.u ? 1 : 4));
    rn += (instr.fields.u ? 1 : -1) * offset;
  }

  if (!instr.fields.w) {
    rn = old_rn;
  }

  registers.r[15] += kInstrSize;
  return true;
}

[[nodiscard]] bool CPU::dispatch_STR(U32 instr_,
                                     const Memory::Memory &memory) noexcept {
  const SingleDataTransferInstr instr{instr_};
  if (!evaluate_cond(ConditionCode(instr.fields.cond), registers.CPSR)) {
    registers.r[15] += kInstrSize;
    return true;
  }

  U32 &rd = registers.r[instr.fields.rd];
  U32 &rn = registers.r[instr.fields.rn];

  U32 offset;
  if (instr.fields.i == 0) {
    offset = instr.fields.offset;
  } else {
    printf("Not implemented");
    return false;
  }

  const U32 old_rn = rn;
  if (instr.fields.p) {
    rn += (instr.fields.u ? 1 : -1) * offset;
    memcpy((void *)&memory.mem[rd], &rn, (instr.fields.u ? 1 : 4));
  } else {
    memcpy((void *)&memory.mem[rd], &rn, (instr.fields.u ? 1 : 4));
    rn += (instr.fields.u ? 1 : -1) * offset;
  }

  if (!instr.fields.w) {
    rn = old_rn;
  }

  registers.r[15] += kInstrSize;
  return true;
}

[[nodiscard]] bool CPU::dispatch_thumb_LSL(U16 instr_) noexcept {
  if (((instr_ >> 13) & 0b111) == 0b000) {
    const Thumb::MoveShiftedRegister instr{instr_};
    registers.r[instr.fields.rd] = instr.fields.rs << instr.fields.offset5;
    registers.r[15] += kThumbInstrSize;
    return true;
  }
  return false;
}

[[nodiscard]] bool CPU::dispatch_thumb_BLX(U16 instr_) noexcept {
  const Thumb::UnconditionalBranch instr(instr_);
  int32_t offset = static_cast<int32_t>(U32(instr.fields.offset11)
                                        << 12); // TODO: Sign Extend

  return false;
}

} // namespace Emulator::Arm
