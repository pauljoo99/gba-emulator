#include <cassert>
#include <cstring>
#include <stdio.h>

#include "arm7tdmi.h"
#include "arm_extended_instructions.h"
#include "arm_instructions.h"
#include "bitutils.h"
#include "logging.h"
#include "snapshot.h"
#include "thumb_instructions.h"

namespace Emulator::Arm {

using namespace BitUtils;

ShifterOperandResult CPU::ShifterOperand(DataProcessingInstr instr) noexcept {
  if (instr.fields.i) {
    return ShifterOperandImmediate(instr.fields.operand_2);
  } else {
    U8 code = (instr >> 4) & 0b111;
    if (code == 0) {
      return ShifterOperandLogicalShiftLeftByImm(instr.fields.operand_2);
    } else if (code == 1) {
      return ShifterOperandLogicalShiftLeftByRegister(instr.fields.operand_2);
    } else if (code == 2) {
      return ShifterOperandLogicalShiftRightByImm(instr.fields.operand_2);
    } else if (code == 3) {
      return ShifterOperandLogicalShiftRightByRegister(instr.fields.operand_2);
    } else if (code == 4) {
      return ShifterOperandArithmeticShiftRightByImm(instr.fields.operand_2);
    } else if (code == 5) {
      return ShifterOperandArithmeticShiftRightByRegister(
          instr.fields.operand_2);
    } else if (code == 6) {
      return ShifterOperandRotateRightByImm(instr.fields.operand_2);
    } else if (code == 7) {
      return ShifterOperandRotateRightByRegister(instr.fields.operand_2);
    } else {
      LOG_ABORT("CouldNotDecodeShiftOperand");
    }
  }
}

ShifterOperandResult
CPU::ShifterOperandImmediate(DataProcessingInstrImmediate operand_2) noexcept {
  ShifterOperandResult result{};
  CPSR_Register cpsr(registers->CPSR);
  result.shifter_operand =
      RotateRight(operand_2.fields.immed_8, operand_2.fields.rotate_imm * 2);
  if (operand_2.fields.rotate_imm == 0) {
    result.shifter_carry_out = cpsr.bits.C;
  } else {
    result.shifter_carry_out = GetBit(result.shifter_operand, 31);
  }
  return result;
}

ShifterOperandResult CPU::ShifterOperandLogicalShiftLeftByImm(
    DataProcessingInstrShiftByImm operand_2) noexcept {
  ShifterOperandResult result{};
  CPSR_Register cpsr(registers->CPSR);
  if (operand_2.fields.shift_imm == 0) {
    result.shifter_carry_out = cpsr.bits.C;
  } else {
    result.shifter_carry_out = GetBit(registers->r[operand_2.fields.rm],
                                      32 - operand_2.fields.shift_imm);
  }
  result.shifter_operand = LogicalShiftLeft(registers->r[operand_2.fields.rm],
                                            operand_2.fields.shift_imm);
  return result;
}

ShifterOperandResult CPU::ShifterOperandLogicalShiftLeftByRegister(
    DataProcessingInstrShiftByRegister operand_2) noexcept {
  ShifterOperandResult result{};
  U8 rs(registers->r[operand_2.fields.rs]);
  U32 rm(registers->r[operand_2.fields.rm]);
  if (rs == 0) {
    result.shifter_operand = rm;
    result.shifter_carry_out = CPSR_Register(registers->CPSR).bits.C;
  } else if (rs < 32) {
    result.shifter_operand = LogicalShiftLeft(rm, rs);
    result.shifter_carry_out = GetBit(rm, 32 - rs);
  } else if (rs == 32) {
    result.shifter_operand = 0;
    result.shifter_carry_out = GetBit(rm, 0);
  } else {
    result.shifter_operand = 0;
    result.shifter_carry_out = 0;
  }
  return result;
}

ShifterOperandResult CPU::ShifterOperandLogicalShiftRightByImm(
    DataProcessingInstrShiftByImm operand_2) noexcept {
  ShifterOperandResult result{};
  CPSR_Register cpsr(registers->CPSR);
  if (operand_2.fields.shift_imm == 0) {
    result.shifter_operand = 0;
    result.shifter_carry_out = GetBit(registers->r[operand_2.fields.rm], 31);
  } else {
    result.shifter_operand = LogicalShiftRight(
        registers->r[operand_2.fields.rm], operand_2.fields.shift_imm);
    result.shifter_carry_out = GetBit(registers->r[operand_2.fields.rm],
                                      operand_2.fields.shift_imm - 1);
  }
  return result;
}

ShifterOperandResult CPU::ShifterOperandLogicalShiftRightByRegister(
    DataProcessingInstrShiftByRegister operand_2) noexcept {
  ShifterOperandResult result{};
  U8 rs(registers->r[operand_2.fields.rs]);
  U32 rm(registers->r[operand_2.fields.rm]);
  if (rs == 0) {
    result.shifter_operand = rm;
    result.shifter_carry_out = CPSR_Register(registers->CPSR).bits.C;
  } else if (rs < 32) {
    result.shifter_operand = LogicalShiftRight(rm, rs);
    result.shifter_carry_out = GetBit(rm, rs - 1);
  } else if (rs == 32) {
    result.shifter_operand = 0;
    result.shifter_carry_out = GetBit(rm, 31);
  } else {
    result.shifter_operand = 0;
    result.shifter_carry_out = 0;
  }
  return result;
}

ShifterOperandResult CPU::ShifterOperandArithmeticShiftRightByImm(
    DataProcessingInstrShiftByImm operand_2) noexcept {
  ShifterOperandResult result{};
  CPSR_Register cpsr(registers->CPSR);
  U32 rm(registers->r[operand_2.fields.rm]);
  if (operand_2.fields.shift_imm == 0) {
    if (GetBit(rm, 31) == 0) {
      result.shifter_operand = 0;
      result.shifter_carry_out = 0;
    } else {
      result.shifter_operand = 0xFFFFFFFF;
      result.shifter_carry_out = 1;
    }
  } else {
    result.shifter_operand =
        (U32)ArithmeticShiftRight((I32)rm, operand_2.fields.shift_imm);
    result.shifter_carry_out = GetBit(rm, operand_2.fields.shift_imm - 1);
  }
  return result;
}

ShifterOperandResult CPU::ShifterOperandArithmeticShiftRightByRegister(
    DataProcessingInstrShiftByRegister operand_2) noexcept {
  ShifterOperandResult result{};
  U8 rs(registers->r[operand_2.fields.rs]);
  U32 rm(registers->r[operand_2.fields.rm]);
  if (rs == 0) {
    result.shifter_operand = rm;
    result.shifter_carry_out = CPSR_Register(registers->CPSR).bits.C;
  } else if (rs < 32) {
    result.shifter_operand = ArithmeticShiftRight((I32)rm, rs);
    result.shifter_carry_out = GetBit(rm, rs - 1);
  } else if (rs >= 32) {
    if (rm == 0) {
      result.shifter_operand = 0;
      result.shifter_carry_out = 0;
    } else {
      result.shifter_operand = 0xFFFFFFFF;
      result.shifter_carry_out = 1;
    }
  }
  return result;
}

ShifterOperandResult CPU::ShifterOperandRotateRightByImm(
    DataProcessingInstrShiftByImm operand_2) noexcept {
  ShifterOperandResult result{};
  CPSR_Register cpsr(registers->CPSR);
  U32 rm(registers->r[operand_2.fields.rm]);
  if (operand_2.fields.shift_imm == 0) {
    result.shifter_operand =
        LogicalShiftLeft(cpsr.bits.C, 31) | LogicalShiftRight(rm, 1);
    result.shifter_carry_out = GetBit(rm, 0);
  } else {
    result.shifter_operand = RotateRight(rm, operand_2.fields.shift_imm);
    result.shifter_carry_out = GetBit(rm, operand_2.fields.shift_imm - 1);
  }
  return result;
}

ShifterOperandResult CPU::ShifterOperandRotateRightByRegister(
    DataProcessingInstrShiftByRegister operand_2) noexcept {
  ShifterOperandResult result{};
  U8 rs(registers->r[operand_2.fields.rs]);
  U32 rm(registers->r[operand_2.fields.rm]);
  if (rs == 0) {
    result.shifter_operand = rm;
    result.shifter_carry_out = CPSR_Register(registers->CPSR).bits.C;
  } else if (GetBitsInRange(rs, 0, 5) == 0) {
    result.shifter_operand = rm;
    result.shifter_carry_out = GetBit(rm, 31);
  } else {
    result.shifter_operand = RotateRight(rm, GetBitsInRange(rs, 0, 5));
    result.shifter_carry_out = GetBit(rm, GetBitsInRange(rs, 0, 5) - 1);
  }
  return result;
}

bool CPU::AdvancePipeline(U32 instr, U32 addr) noexcept {
  pipeline.execute = pipeline.decode;
  pipeline.decode = pipeline.fetch;
  pipeline.fetch = instr;

  pipeline.execute_addr = pipeline.decode_addr;
  pipeline.decode_addr = pipeline.fetch_addr;
  pipeline.fetch_addr = addr;
  return pipeline.execute != U32(-1);
}

void CPU::ClearPipeline() noexcept {
  pipeline.fetch = U32(-1);
  pipeline.decode = U32(-1);
  pipeline.execute = U32(-1);

  pipeline.execute_addr = U32(-1);
  pipeline.decode_addr = U32(-1);
  pipeline.fetch_addr = U32(-1);
}

[[nodiscard]] bool ProcessInstruction(U32 instr, Memory::Memory &memory,
                                      CPU &cpu) {

  ExtendedInstr extended_instr_opcode = GetExtendedArmOpcode(instr);
  assert(extended_instr_opcode != ExtendedInstr::UNDEFINED1 ||
         extended_instr_opcode != ExtendedInstr::UNDEFINED2 ||
         extended_instr_opcode != ExtendedInstr::NUM_OPCODES);

  Instr instr_opcode;
  if (extended_instr_opcode == ExtendedInstr::NONE) {
    instr_opcode = GetArmOpcode(instr);
    LOG("Dispatch %u - Instr: %s, Raw Instr: 0x%08X, PC: 0x%04X",
        cpu.dispatch_num, ToString(instr_opcode), instr,
        cpu.pipeline.execute_addr);

  } else {
    instr_opcode = ExtendedInstrToArmInstr[U32(extended_instr_opcode)];
    LOG("Dispatch %u - Extended Instr: %s, Raw Instr: 0x%08X, PC: 0x%04X",
        cpu.dispatch_num, ToString(instr_opcode), instr,
        cpu.pipeline.execute_addr);
  }

  switch (instr_opcode) {
  case Instr::B:
    return cpu.Dispatch_B(instr);
  case Instr::BL:
    return cpu.Dispatch_BL(instr);
  case Instr::BIC:
    return cpu.Dispatch_BIC(instr);
  case Instr::BX:
    return cpu.Dispatch_BX(instr);
  case Instr::MOV:
    return cpu.Dispatch_MOV(instr);
  case Instr::MSR:
    return cpu.Dispatch_MSR(instr);
  case Instr::LDR:
    return cpu.Dispatch_LDR(instr, memory);
  case Instr::LDRB:
    return cpu.Dispatch_LDRB(instr, memory);
  case Instr::LDRH:
    return cpu.Dispatch_LDRH(instr, memory);
  case Instr::CMP:
    return cpu.Dispatch_CMP(instr);
  case Instr::TEQ:
    return cpu.Dispatch_TEQ(instr);
  case Instr::TST:
    return cpu.Dispatch_TST(instr);
  case Instr::MRS:
    return cpu.Dispatch_MRS(instr);
  case Instr::ORR:
    return cpu.Dispatch_ORR(instr);
  case Instr::STM:
    return cpu.Dispatch_STM(instr, memory);
  case Instr::LDM:
    return cpu.Dispatch_LDM(instr, memory);
  case Instr::CMN:
    return cpu.Dispatch_CMN(instr);
  case Instr::SUB:
    return cpu.Dispatch_SUB(instr);
  case Instr::RSB:
    return cpu.Dispatch_RSB(instr);
  case Instr::STR:
    return cpu.Dispatch_STR(instr, memory);
  case Instr::ADD:
    return cpu.Dispatch_ADD(instr);
  case Instr::AND:
    return cpu.Dispatch_AND(instr);
  default:
    return false;
  }
}

[[nodiscard]] bool ProcessThumbInstruction(U16 instr, Memory::Memory &memory,
                                           CPU &cpu) noexcept {

  const Thumb::ThumbOpcode opcode = Thumb::GetThumbOpcode(instr);
  LOG("Dispatch %u - Raw Thumb Instr: 0x%04X, Opcode: %s, PC: 0x%04X",
      cpu.dispatch_num, instr, Thumb::ToString(opcode),
      cpu.pipeline.execute_addr);

  switch (opcode) {
  case (Thumb::ThumbOpcode::CMP1):
    return cpu.Dispatch_Thumb_CMP1(instr);
  case (Thumb::ThumbOpcode::CMP2):
    return cpu.Dispatch_Thumb_CMP2(instr);
  case (Thumb::ThumbOpcode::CMP3):
    return cpu.Dispatch_Thumb_CMP3(instr);
  case (Thumb::ThumbOpcode::MOV1):
    return cpu.Dispatch_Thumb_MOV1(instr);
  case (Thumb::ThumbOpcode::MOV3):
    return cpu.Dispatch_Thumb_MOV3(instr);
  case (Thumb::ThumbOpcode::MVN):
    return cpu.Dispatch_Thumb_MVN(instr);
  case (Thumb::ThumbOpcode::LDR1):
    return cpu.Dispatch_Thumb_LDR1(instr, memory);
  case (Thumb::ThumbOpcode::LDR2):
    return cpu.Dispatch_Thumb_LDR2(instr, memory);
  case (Thumb::ThumbOpcode::LDR3):
    return cpu.Dispatch_Thumb_LDR3(instr, memory);
  case (Thumb::ThumbOpcode::LDR4):
    return cpu.Dispatch_Thumb_LDR4(instr, memory);
  case (Thumb::ThumbOpcode::LDRB1):
    return cpu.Dispatch_Thumb_LDRB1(instr, memory);
  case (Thumb::ThumbOpcode::LDRH1):
    return cpu.Dispatch_Thumb_LDRH1(instr, memory);
  case (Thumb::ThumbOpcode::LDRH2):
    return cpu.Dispatch_Thumb_LDRH2(instr, memory);
  case (Thumb::ThumbOpcode::LDMIA):
    return cpu.Dispatch_Thumb_LDMIA(instr, memory);
  case (Thumb::ThumbOpcode::STRB1):
    return cpu.Dispatch_Thumb_STRB1(instr, memory);
  case (Thumb::ThumbOpcode::STR1):
    return cpu.Dispatch_Thumb_STR1(instr, memory);
  case (Thumb::ThumbOpcode::STR2):
    return cpu.Dispatch_Thumb_STR2(instr, memory);
  case (Thumb::ThumbOpcode::STR3):
    return cpu.Dispatch_Thumb_STR3(instr, memory);
  case (Thumb::ThumbOpcode::STMIA):
    return cpu.Dispatch_Thumb_STMIA(instr, memory);
  case (Thumb::ThumbOpcode::STRH1):
    return cpu.Dispatch_Thumb_STRH1(instr, memory);
  case (Thumb::ThumbOpcode::STRH2):
    return cpu.Dispatch_Thumb_STRH2(instr, memory);
  case (Thumb::ThumbOpcode::ORR):
    return cpu.Dispatch_Thumb_ORR(instr);
  case (Thumb::ThumbOpcode::EOR):
    return cpu.Dispatch_Thumb_EOR(instr);
  case (Thumb::ThumbOpcode::ADD1):
    return cpu.Dispatch_Thumb_ADD1(instr);
  case (Thumb::ThumbOpcode::ADD2):
    return cpu.Dispatch_Thumb_ADD2(instr);
  case (Thumb::ThumbOpcode::ADD3):
    return cpu.Dispatch_Thumb_ADD3(instr);
  case (Thumb::ThumbOpcode::ADD5):
    return cpu.Dispatch_Thumb_ADD5(instr);
  case (Thumb::ThumbOpcode::ADD6):
    return cpu.Dispatch_Thumb_ADD6(instr);
  case (Thumb::ThumbOpcode::ADD7):
    return cpu.Dispatch_Thumb_ADD7(instr);
  case (Thumb::ThumbOpcode::LSL1):
    return cpu.Dispatch_Thumb_LSL1(instr);
  case (Thumb::ThumbOpcode::LSL2):
    return cpu.Dispatch_Thumb_LSL2(instr);
  case (Thumb::ThumbOpcode::LSR1):
    return cpu.Dispatch_Thumb_LSR1(instr);
  case (Thumb::ThumbOpcode::ROR):
    return cpu.Dispatch_Thumb_ROR(instr);
  case (Thumb::ThumbOpcode::ASR1):
    return cpu.Dispatch_Thumb_ASR1(instr);
  case (Thumb::ThumbOpcode::B1):
    return cpu.Dispatch_Thumb_B1(instr);
  case (Thumb::ThumbOpcode::B2):
    return cpu.Dispatch_Thumb_B2(instr);
  case (Thumb::ThumbOpcode::BIC):
    return cpu.Dispatch_Thumb_BIC(instr);
  case (Thumb::ThumbOpcode::BL):
    return cpu.Dispatch_Thumb_BL(instr);
  case (Thumb::ThumbOpcode::BX):
    return cpu.Dispatch_Thumb_BX(instr);
  case (Thumb::ThumbOpcode::PUSH):
    return cpu.Dispatch_Thumb_PUSH(instr, memory);
  case (Thumb::ThumbOpcode::POP):
    return cpu.Dispatch_Thumb_POP(instr, memory);
  case (Thumb::ThumbOpcode::SUB1):
    return cpu.Dispatch_Thumb_SUB1(instr);
  case (Thumb::ThumbOpcode::SUB2):
    return cpu.Dispatch_Thumb_SUB2(instr);
  case (Thumb::ThumbOpcode::SUB3):
    return cpu.Dispatch_Thumb_SUB3(instr);
  case (Thumb::ThumbOpcode::SUB4):
    return cpu.Dispatch_Thumb_SUB4(instr);
  case (Thumb::ThumbOpcode::TST):
    return cpu.Dispatch_Thumb_TST(instr);
  case (Thumb::ThumbOpcode::MUL):
    return cpu.Dispatch_Thumb_MUL(instr);
  default:
    break;
  }

  return false;
}

bool EvaluateCondition(ConditionCode cond, U32 cpsr_) {
  CPSR_Register cpsr(cpsr_);
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

U32 CPU::LoadAndStoreWordOrByteImmAddr(U32 instr_) noexcept {
  const SingleDataTransferInstr instr{instr_};
  const LoadAndStoreWordOrByteImm encoding{instr.fields.offset};

  if (!EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    // Does not matter. The instruction will be skipped anyways.
    return 0;
  }
  U32 address;
  if (instr.fields.p == 1) {
    if (instr.fields.w == 1) {
      if (instr.fields.u == 1) {
        registers->r[instr.fields.rn] += encoding.fields.offset;
      } else {
        registers->r[instr.fields.rn] -= encoding.fields.offset;
      }
      address = registers->r[instr.fields.rn];
    } else {
      if (instr.fields.u == 1) {
        address = registers->r[instr.fields.rn] + encoding.fields.offset;
      } else {
        address = registers->r[instr.fields.rn] - encoding.fields.offset;
      }
    }
  }
  if (instr.fields.p == 0) {
    address = registers->r[instr.fields.rn];
    if (instr.fields.u == 1) {
      registers->r[instr.fields.rn] += encoding.fields.offset;
    } else {
      registers->r[instr.fields.rn] -= encoding.fields.offset;
    }
  }
  return address;
}

U32 CPU::LoadAndStoreWordOrByteRegAddr(U32 instr_) noexcept {
  const SingleDataTransferInstr instr{instr_};
  const LoadAndStoreWordOrByteReg encoding{instr.fields.offset};

  if (!EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    // Does not matter. The instruction will be skipped anyways.
    return 0;
  }

  U32 index;
  switch (encoding.fields.shift) {
  case (0b00):
    index = LogicalShiftLeft(registers->r[encoding.fields.rm],
                             encoding.fields.shift_imm);
    break;
  case (0b01):
    if (encoding.fields.shift_imm == 0) {
      index = 0;
    } else {
      index = LogicalShiftRight(registers->r[encoding.fields.rm],
                                encoding.fields.shift_imm);
    }
    break;
  case (0b10):
    if (encoding.fields.shift_imm == 0) {
      if (registers->r[encoding.fields.rm] == 1) {
        index = 0xFFFFFFFF;
      } else {
        index = 0;
      }
    } else {
      index = ArithmeticShiftRight(registers->r[encoding.fields.rm],
                                   encoding.fields.shift_imm);
    }
    break;
  case (0b11):
    CPSR_Register cpsr(registers->CPSR);
    if (encoding.fields.shift_imm == 0) {
      index = LogicalShiftLeft(cpsr.bits.C, 31) |
              LogicalShiftRight(registers->r[encoding.fields.rm], 1);
    } else {
      index = RotateRight(registers->r[encoding.fields.rm],
                          encoding.fields.shift_imm);
    }
    break;
  }

  if (instr.fields.p == 1 && instr.fields.w == 1) {
    if (instr.fields.u == 1) {
      registers->r[instr.fields.rn] += index;
    } else {
      registers->r[instr.fields.rn] -= index;
    }
    // TODO: Add w == 0 case which determines access privilege.
  }

  U32 address = registers->r[instr.fields.rn];

  if (instr.fields.p == 0) {
    if (instr.fields.u == 1) {
      registers->r[instr.fields.rn] += index;
    } else {
      registers->r[instr.fields.rn] -= index;
    }
  }
  return address;
}

U32 CPU::LoadAndStoreWordOrByteAddr(SingleDataTransferInstr instr) noexcept {
  if (instr.fields.i == 0) {
    return LoadAndStoreWordOrByteImmAddr(instr);
  } else {
    return LoadAndStoreWordOrByteRegAddr(instr);
  }
}

LoadAndStoreMultipleAddrResult
CPU::LoadAndStoreMultipleAddr(U32 instr_) noexcept {
  LoadAndStoreMultiple instr(instr_);

  if (!EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    return {};
  }

  LoadAndStoreMultipleAddrResult result;
  if (instr.fields.p == 0 && instr.fields.u == 1) {
    result.start_addr = registers->r[instr.fields.rn];
    result.end_addr = registers->r[instr.fields.rn] +
                      CountSetBits(instr.fields.register_list) * 4 - 4;
    if (instr.fields.w == 1) {
      registers->r[instr.fields.rn] = result.end_addr + 4;
    }
  } else if (instr.fields.p == 1 && instr.fields.u == 1) {
    result.start_addr = registers->r[instr.fields.rn] + 4;
    result.end_addr = registers->r[instr.fields.rn] +
                      CountSetBits(instr.fields.register_list) * 4;
    if (instr.fields.w == 1) {
      registers->r[instr.fields.rn] = result.end_addr;
    }
  } else if (instr.fields.p == 0 && instr.fields.u == 0) {
    result.start_addr = registers->r[instr.fields.rn] -
                        CountSetBits(instr.fields.register_list) * 4 + 4;
    result.end_addr = registers->r[instr.fields.rn];
    if (instr.fields.w == 1) {
      registers->r[instr.fields.rn] = result.start_addr - 4;
    }
  } else {
    result.start_addr = registers->r[instr.fields.rn] -
                        CountSetBits(instr.fields.register_list) * 4;
    result.end_addr = registers->r[instr.fields.rn] - 4;
    if (instr.fields.w == 1) {
      registers->r[instr.fields.rn] = result.start_addr;
    }
  }
  return result;
}

[[nodiscard]] bool CPU::Dispatch(Memory::Memory &memory) noexcept {
  ChangeRegistersOnMode();
  CPSR_Register cpsr(registers->CPSR);
  if (cpsr.bits.T) {
    U16 instr = ReadHalfWordFromGBAMemory(memory, registers->r[PC]);
    if (AdvancePipeline((U32)instr, registers->r[PC])) {
      if (!ProcessThumbInstruction((U16)pipeline.execute, memory, *this)) {
        return false;
      }
    } else {
      registers->r[PC] += 2;
    }
  } else {
    U32 instr = ReadWordFromGBAMemory(memory, registers->r[PC]);
    if (AdvancePipeline(instr, registers->r[PC])) {
      if (!ProcessInstruction(pipeline.execute, memory, *this)) {
        return false;
      }
    } else {
      registers->r[PC] += 4;
    }
  }
  dispatch_num++;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_B(U32 instr_) noexcept {
  const BranchInstr instr(instr_);
  if (!EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    registers->r[PC] += 4;
    return true;
  }
  registers->r[PC] = static_cast<I32>(registers->r[PC]) +
                     SignExtend(ConcatBits(instr.fields.offset, 0b00, 2), 26);
  ClearPipeline();
  return true;
}

[[nodiscard]] bool CPU::Dispatch_BL(U32 instr_) noexcept {
  const BranchInstr instr(instr_);
  if (!EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    registers->r[PC] += 4;
    return true;
  }
  registers->r[PC] = (I32)registers->r[PC] +
                     SignExtend(ConcatBits(instr.fields.offset, 0, 2), 26);
  registers->r[14] = pipeline.execute_addr + 4;
  ClearPipeline();
  return true;
}

[[nodiscard]] bool CPU::Dispatch_BIC(U32 instr_) noexcept {
  DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    registers->r[instr.fields.rd] =
        registers->r[instr.fields.rn] & ~shifter.shifter_operand;
    if (instr.fields.s && instr.fields.rd == PC) {
      registers->CPSR = U32(registers->SPRS);
      ClearPipeline();
      return true;
    } else if (instr.fields.s) {
      CPSR_SetN(GetBit(registers->r[instr.fields.rd], 31));
      CPSR_SetZ(instr.fields.rd == 0);
      CPSR_SetC(shifter.shifter_carry_out);
      registers->r[PC] += 4;
      return true;
    }
  }
  registers->r[PC] += 4;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_BX(U32 instr_) noexcept {
  const BranchAndExchangeInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    CPSR_SetT(GetBit(registers->r[instr.fields.rm], 0));
    registers->r[PC] = registers->r[instr.fields.rm] & 0xFFFFFFFE;
    ClearPipeline();
  } else {
    registers->r[PC] += 4;
  }
  return true;
}

[[nodiscard]] bool CPU::Dispatch_MOV(U32 instr_) noexcept {
  const DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    registers->r[instr.fields.rd] = shifter.shifter_operand;
    if (instr.fields.s && instr.fields.rd == PC) {
      registers->CPSR = U32(registers->SPRS);
    } else if (instr.fields.s) {
      CPSR_SetN(GetBit(registers->r[instr.fields.rd], 31));
      CPSR_SetZ(instr.fields.rd == 0);
      CPSR_SetC(shifter.shifter_carry_out);
    }
  }
  registers->r[PC] += 4;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_MSR(U32 instr_) noexcept {
  MSRImmInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    U32 operand = 0;
    if (GetBit(instr_, 25)) {
      MSRImmInstr instr(instr_);
      operand = RotateRight(instr.fields.imm, instr.fields.rotate_imm * 2);
    } else {
      MSRRegInstr instr(instr_);
      operand = registers->r[instr.fields.rm];
    }
    if (instr.fields.r == 0) {
      CPSR_Register cpsr(registers->CPSR);
      if (cpsr.bits.M != 0b10000) {
        if (GetBit(instr.fields.field_mask, 0)) {
          registers->CPSR =
              (registers->CPSR & 0xFFFFFF00) | (operand & 0x000000FF);
        }
        if (GetBit(instr.fields.field_mask, 1)) {
          registers->CPSR =
              (registers->CPSR & 0xFFFF00FF) | (operand & 0x0000FF00);
        }
        if (GetBit(instr.fields.field_mask, 2)) {
          registers->CPSR =
              (registers->CPSR & 0xFF00FFFF) | (operand & 0x00FF0000);
        }
      }
      if (GetBit(instr.fields.field_mask, 3)) {
        registers->CPSR =
            (registers->CPSR & 0x00FFFFFF) | (operand & 0xFF000000);
      }
    } else {
      Mode mode = GetMode();
      if (mode == Mode::SUPERVISOR || mode == Mode::ABORT ||
          mode == Mode::UNDEFINED || mode == Mode::INTERRUPT ||
          mode == Mode::FAST_INTERRUPT) {
        if (GetBit(instr.fields.field_mask, 0)) {
          registers->SPRS =
              (registers->SPRS & 0xFFFFFF00) | (operand & 0x000000FF);
        }
        if (GetBit(instr.fields.field_mask, 1)) {
          registers->SPRS =
              (registers->SPRS & 0xFFFF00FF) | (operand & 0x0000FF00);
        }
        if (GetBit(instr.fields.field_mask, 2)) {
          registers->SPRS =
              (registers->SPRS & 0xFF00FFFF) | (operand & 0x00FF0000);
        }
        if (GetBit(instr.fields.field_mask, 3)) {
          registers->SPRS =
              (registers->SPRS & 0x00FFFFFF) | (operand & 0xFF000000);
        }
      }
    }
  }
  registers->r[PC] += 4;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_MRS(U32 instr_) noexcept {
  MRSRegInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    if (instr.fields.r == 1) {
      registers->r[instr.fields.rd] = U32(registers->SPRS);
    } else {
      registers->r[instr.fields.rd] = U32(registers->CPSR);
    }
  }
  registers->r[PC] += 4;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_ORR(U32 instr_) noexcept {
  DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    registers->r[instr.fields.rd] =
        registers->r[instr.fields.rn] | shifter.shifter_operand;
    if (instr.fields.s == 1 && instr.fields.rn == PC) {
      registers->CPSR = U32(registers->SPRS);
    } else if (instr.fields.s == 1) {
      CPSR_SetN(GetBit(registers->r[instr.fields.rd], 31));
      CPSR_SetZ(registers->r[instr.fields.rd] == 0);
      CPSR_SetC(shifter.shifter_carry_out);
    }
  }
  registers->r[PC] += 4;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_LDRB(U32 instr_,
                                      const Memory::Memory &memory) noexcept {
  const SingleDataTransferInstr instr{instr_};
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    U32 address = LoadAndStoreWordOrByteAddr(instr);
    registers->r[instr.fields.rd] = ReadByteFromGBAMemory(memory, address);
  }
  registers->r[PC] += 4;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_LDRH(U32 instr_,
                                      const Memory::Memory &memory) noexcept {
  const SingleDataTransferInstr instr{instr_};
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    U32 address = LoadAndStoreWordOrByteAddr(instr);
    if ((address & 0b1) == 0) {
      registers->r[instr.fields.rd] =
          ReadHalfWordFromGBAMemory(memory, address);
    } else {
      LOG_ABORT("Bad address: 0x%04X", address);
    }
  }
  registers->r[PC] += 4;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_LDR(U32 instr_,
                                     const Memory::Memory &memory) noexcept {
  const SingleDataTransferInstr instr{instr_};
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {

    U32 address;
    if (instr.fields.i == 0) {
      address = LoadAndStoreWordOrByteImmAddr(instr_);
    } else {
      address = LoadAndStoreWordOrByteRegAddr(instr_);
    }

    U32 value;
    if (GetBitsInRange(address, 0, 2) == 0b00) {
      value = ReadWordFromGBAMemory(memory, address);
    } else if (GetBitsInRange(address, 0, 2) == 0b01) {
      value = ReadWordFromGBAMemory(memory, address);
      value = RotateRight(value, 8);
    } else if (GetBitsInRange(address, 0, 2) == 0b10) {
      value = ReadWordFromGBAMemory(memory, address);
      value = RotateRight(value, 16);
    } else if (GetBitsInRange(address, 0, 2) == 0b11) {
      value = ReadWordFromGBAMemory(memory, address);
      value = RotateRight(value, 24);
    } else {
      value = 0;
    }

    if (instr.fields.rd == PC) {
      registers->r[instr.fields.rd] = value & 0xFFFFFFFC;
      ClearPipeline();
      return true;
    } else {
      registers->r[instr.fields.rd] = value;
    }
  }

  registers->r[PC] += 4;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_STR(U32 instr_,
                                     Memory::Memory &memory) noexcept {
  const SingleDataTransferInstr instr{instr_};
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    U32 address;
    if (instr.fields.i == 0) {
      address = LoadAndStoreWordOrByteImmAddr(instr_);
    } else {
      address = LoadAndStoreWordOrByteRegAddr(instr_);
    }
    WriteWordFromGBAMemory(memory, address, registers->r[instr.fields.rd]);
  }
  registers->r[PC] += 4;
  return true;
}

[[nodiscard]] bool CPU::Dispatch_CMP(U32 instr_) noexcept {
  const DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    U32 alu_out = registers->r[instr.fields.rn] - shifter.shifter_operand;
    CPSR_SetN(GetBit(alu_out, 31));
    CPSR_SetZ(alu_out == 0);
    CPSR_SetC(!UnsignedSubBorrow(registers->r[instr.fields.rn],
                                 shifter.shifter_operand));
    CPSR_SetV(SignedSubOverflow(registers->r[instr.fields.rn],
                                shifter.shifter_operand));
  }
  registers->r[PC] += 4;
  return true;
}

bool CPU::Dispatch_TEQ(U32 instr_) noexcept {
  const DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    U32 alu_out = registers->r[instr.fields.rn] ^ shifter.shifter_operand;

    CPSR_SetN(GetBit(alu_out, 31));
    CPSR_SetZ(alu_out == 0);
    CPSR_SetC(shifter.shifter_carry_out);
  }
  registers->r[PC] += 4;
  return true;
}

bool CPU::Dispatch_TST(U32 instr_) noexcept {
  const DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    U32 alu_out = registers->r[instr.fields.rn] & shifter.shifter_operand;

    CPSR_SetN(GetBit(alu_out, 31));
    CPSR_SetZ(alu_out == 0);
    CPSR_SetC(shifter.shifter_carry_out);
  }
  registers->r[PC] += 4;
  return true;
}

bool CPU::Dispatch_STM(U32 instr_, Memory::Memory &memory) noexcept {
  LoadAndStoreMultiple instr(instr_);
  if (!EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    registers->r[PC] += 4;
    return true;
  }

  LoadAndStoreMultipleAddrResult addr = LoadAndStoreMultipleAddr(instr_);
  U32 address = addr.start_addr;
  for (U32 i = 0; i < 15; ++i) {
    // If s == 0, then STM1, else STM2.
    U32 val = instr.fields.s == 1 ? user_registers.r[i] : registers->r[i];
    if (GetBit(instr.fields.register_list, i) == 1) {
      WriteWordFromGBAMemory(memory, address, val);
      address += 4;
    }
  }
  assert(addr.end_addr == address - 4);
  registers->r[PC] += 4;
  return true;
}

bool CPU::Dispatch_LDM(U32 instr_, const Memory::Memory &memory) noexcept {
  LoadAndStoreMultiple instr(instr_);
  // LDM1
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    if (instr.fields.s == 0) {
      LoadAndStoreMultipleAddrResult addr = LoadAndStoreMultipleAddr(instr_);
      U32 address = addr.start_addr;
      for (U32 i = 0; i < 15; ++i) {
        if (GetBit(instr.fields.register_list, i) == 1) {
          registers->r[i] = ReadWordFromGBAMemory(memory, address);
          address += 4;
        }
      }
      if (GetBit(instr.fields.register_list, 15) == 1) {
        U32 value = ReadWordFromGBAMemory(memory, address);
        registers->r[PC] = value & 0xFFFFFFFC;
        ClearPipeline();
      } else {
        registers->r[PC] += 4;
      }
      assert(addr.end_addr == address - 4);
      return true;
    } else {
      // LDM2
      if (GetBit(instr.fields.register_list, 15) == 0) {
        LoadAndStoreMultipleAddrResult addr = LoadAndStoreMultipleAddr(instr_);
        U32 address = addr.start_addr;
        for (U32 i = 0; i < 15; ++i) {
          if (GetBit(instr.fields.register_list, i) == 1) {
            user_registers.r[i] = ReadWordFromGBAMemory(memory, address);
            address += 4;
          }
        }
        assert(addr.end_addr == address - 4);
        registers->r[PC] += 4;
        return true;
      } else {
        // LDM3
        LoadAndStoreMultipleAddrResult addr = LoadAndStoreMultipleAddr(instr_);
        U32 address = addr.start_addr;
        for (U32 i = 0; i < 15; ++i) {
          if (GetBit(instr.fields.register_list, i) == 1) {
            registers->r[i] = ReadWordFromGBAMemory(memory, address);
            address += 4;
          }
        }
        registers->CPSR = U32(registers->SPRS);
        U32 value = ReadWordFromGBAMemory(memory, address);

        CPSR_Register cpsr(registers->CPSR);
        if (cpsr.bits.T == 1) {
          registers->r[PC] = value & 0xFFFFFFFE;
          ClearPipeline();
        }
        address += 4;

        assert(addr.end_addr == address - 4);
        return true;
      }
    }
  } else {
    registers->r[PC] += 4;
    return true;
  }
}

bool CPU::Dispatch_CMN(U32 instr_) noexcept {
  DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    U32 alu_out = registers->r[instr.fields.rn] + shifter.shifter_operand;

    CPSR_SetN(GetBit(alu_out, 31));
    CPSR_SetZ(alu_out == 0);
    CPSR_SetC(UnsignedAddCarry(registers->r[instr.fields.rn],
                               shifter.shifter_operand));
    CPSR_SetV(SignedAddOverflow(registers->r[instr.fields.rn],
                                shifter.shifter_operand));
  }
  registers->r[PC] += 4;
  return true;
}

bool CPU::Dispatch_SUB(U32 instr_) noexcept {
  DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    registers->r[instr.fields.rd] =
        registers->r[instr.fields.rn] - shifter.shifter_operand;
    if (instr.fields.s == 1 && instr.fields.rd == PC) {
      registers->CPSR = U32(registers->SPRS);
      // Return early since pc is being updated.
      ClearPipeline();
      return true;
    } else if (instr.fields.s == 1) {
      CPSR_SetN(GetBit(registers->r[instr.fields.rd], 31));
      CPSR_SetZ(registers->r[instr.fields.rd] == 0);
      CPSR_SetC(!UnsignedSubBorrow(registers->r[instr.fields.rn],
                                   shifter.shifter_operand));
      CPSR_SetV(SignedSubOverflow(registers->r[instr.fields.rn],
                                  shifter.shifter_operand));
    }
  }
  registers->r[PC] += 4;
  return true;
}

bool CPU::Dispatch_RSB(U32 instr_) noexcept {
  DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    registers->r[instr.fields.rd] =
        shifter.shifter_operand - registers->r[instr.fields.rn];
    if (instr.fields.s == 1 && instr.fields.rd == PC) {
      registers->CPSR = U32(registers->SPRS);
      // Return early since pc is being updated.
      ClearPipeline();
      return true;
    } else if (instr.fields.s == 1) {
      CPSR_SetN(GetBit(registers->r[instr.fields.rd], 31));
      CPSR_SetZ(registers->r[instr.fields.rd] == 0);
      CPSR_SetC(!UnsignedSubBorrow(shifter.shifter_operand,
                                   registers->r[instr.fields.rn]));
      CPSR_SetV(SignedSubOverflow(shifter.shifter_operand,
                                  registers->r[instr.fields.rn]));
    }
  }
  registers->r[PC] += 4;
  return true;
}

bool CPU::Dispatch_ADD(U32 instr_) noexcept {
  DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    registers->r[instr.fields.rd] =
        registers->r[instr.fields.rn] + shifter.shifter_operand;

    if (instr.fields.s == 1 && instr.fields.rd == PC) {
      registers->CPSR = U32(registers->SPRS);
      // Return early since pc is being updated.
      ClearPipeline();
      return true;
    } else if (instr.fields.s == 1) {

      CPSR_SetN(GetBit(registers->r[instr.fields.rd], 31));
      CPSR_SetZ(registers->r[instr.fields.rd] == 0);
      CPSR_SetC(UnsignedAddCarry(registers->r[instr.fields.rn],
                                 shifter.shifter_operand));
      CPSR_SetV(SignedAddOverflow(registers->r[instr.fields.rn],
                                  shifter.shifter_operand));
    }
  }
  registers->r[PC] += 4;
  return true;
}

bool CPU::Dispatch_AND(U32 instr_) noexcept {
  DataProcessingInstr instr(instr_);
  if (EvaluateCondition(ConditionCode(instr.fields.cond), registers->CPSR)) {
    ShifterOperandResult shifter = ShifterOperand(instr);
    registers->r[instr.fields.rd] =
        registers->r[instr.fields.rn] & shifter.shifter_operand;
    if (instr.fields.s == 1 && instr.fields.rd == PC) {
      registers->CPSR = U32(registers->SPRS);
      ClearPipeline();
      return true;
    } else if (instr.fields.s == 1) {
      CPSR_SetN(GetBit(registers->r[instr.fields.rd], 31));
      CPSR_SetZ(registers->r[instr.fields.rd] == 0);
      CPSR_SetC(shifter.shifter_carry_out);
    }
  }
  registers->r[PC] += 4;
  return true;
}

bool CPU::Dispatch_Thumb_MOV1(U16 instr) noexcept {
  U32 immed_8 = GetBitsInRange(instr, 0, 8);
  U32 rd = GetBitsInRange(instr, 8, 11);

  registers->r[rd] = immed_8;

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_MOV3(U16 instr) noexcept {
  U32 rd = ConcatBits(GetBit(instr, 7), GetBitsInRange(instr, 0, 3), 3);
  U32 rm = ConcatBits(GetBit(instr, 6), GetBitsInRange(instr, 3, 6), 3);
  registers->r[rd] = U32(registers->r[rm]);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_CMP1(U16 instr) noexcept {
  U32 immed_8 = GetBitsInRange(instr, 0, 8);
  U32 rn = GetBitsInRange(instr, 8, 11);
  U32 alu_out = registers->r[rn] - immed_8;

  CPSR_SetN(GetBit(alu_out, 31));
  CPSR_SetZ(alu_out == 0);
  CPSR_SetC(!UnsignedSubBorrow(registers->r[rn], immed_8));
  CPSR_SetV(SignedSubOverflow(registers->r[rn], immed_8));

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_CMP2(U16 instr) noexcept {
  U32 rn = GetBitsInRange(instr, 0, 3);
  U32 rm = GetBitsInRange(instr, 3, 6);
  U32 alu_out = U32(registers->r[rn]) - U32(registers->r[rm]);

  CPSR_SetN(GetBit(alu_out, 31));
  CPSR_SetZ(alu_out == 0);
  CPSR_SetC(!UnsignedSubBorrow(registers->r[rn], registers->r[rm]));
  CPSR_SetV(SignedSubOverflow(registers->r[rn], registers->r[rm]));

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_CMP3(U16 instr) noexcept {
  U32 rn = ConcatBits(GetBit(instr, 7), GetBitsInRange(instr, 0, 3), 3);
  U32 rm = ConcatBits(GetBit(instr, 6), GetBitsInRange(instr, 3, 6), 3);
  U32 alu_out = U32(registers->r[rn]) - U32(registers->r[rm]);

  CPSR_SetN(GetBit(alu_out, 31));
  CPSR_SetZ(alu_out == 0);
  CPSR_SetC(!UnsignedSubBorrow(registers->r[rn], registers->r[rm]));
  CPSR_SetV(SignedSubOverflow(registers->r[rn], registers->r[rm]));

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_MVN(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rm = GetBitsInRange(instr, 3, 6);

  registers->r[rd] = ~(registers->r[rm]);

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_ORR(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rm = GetBitsInRange(instr, 3, 6);

  registers->r[rd] = registers->r[rd] | registers->r[rm];

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_EOR(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rm = GetBitsInRange(instr, 3, 6);

  registers->r[rd] = registers->r[rd] ^ registers->r[rm];

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_ADD1(U16 instr) noexcept {
  U32 immed_3 = GetBitsInRange(instr, 6, 9);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 rd = GetBitsInRange(instr, 0, 3);

  registers->r[rd] = registers->r[rn] + immed_3;

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);
  CPSR_SetC(UnsignedAddCarry(registers->r[rn], immed_3));
  CPSR_SetV(SignedAddOverflow(registers->r[rn], immed_3));

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_ADD2(U16 instr) noexcept {
  U32 immed_8 = GetBitsInRange(instr, 0, 8);
  U32 rd = GetBitsInRange(instr, 8, 11);

  U32 original_rd = registers->r[rd];
  registers->r[rd] = original_rd + immed_8;

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);
  CPSR_SetC(UnsignedAddCarry(original_rd, immed_8));
  CPSR_SetV(SignedAddOverflow(original_rd, immed_8));

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_ADD3(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 rm = GetBitsInRange(instr, 6, 9);

  registers->r[rd] = registers->r[rn] + registers->r[rm];

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);
  CPSR_SetC(UnsignedAddCarry(registers->r[rn], registers->r[rm]));
  CPSR_SetV(SignedAddOverflow(registers->r[rn], registers->r[rm]));

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_ADD5(U16 instr) noexcept {
  U32 immed_8 = GetBitsInRange(instr, 0, 8);
  U32 rd = GetBitsInRange(instr, 8, 11);
  registers->r[rd] = (registers->r[PC] & 0xFFFFFFFC) + (immed_8 << 2);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_ADD6(U16 instr) noexcept {
  U32 immed_8 = GetBitsInRange(instr, 0, 8);
  U32 rd = GetBitsInRange(instr, 8, 11);
  registers->r[rd] = registers->r[SP] + (immed_8 << 2);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_ADD7(U16 instr) noexcept {
  U32 immed_7 = GetBitsInRange(instr, 0, 7);
  registers->r[SP] += (immed_7 << 2);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LSL1(U16 instr) noexcept {
  U32 immed_5 = GetBitsInRange(instr, 6, 11);
  U32 rm = GetBitsInRange(instr, 3, 6);
  U32 rd = GetBitsInRange(instr, 0, 3);

  if (immed_5 == 0) {
    registers->r[rd] = U32(registers->r[rm]);
  } else {
    CPSR_SetC(GetBit(registers->r[rm], 32 - immed_5));
    registers->r[rd] = LogicalShiftLeft(registers->r[rm], immed_5);
  }

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LSL2(U16 instr) noexcept {
  U32 rs = GetBitsInRange(instr, 3, 6);
  U32 rd = GetBitsInRange(instr, 0, 3);

  U32 rs_byte = GetBitsInRange(registers->r[rs], 0, 8);
  if (rs_byte == 0) {
  } else if (rs_byte < 32) {
    CPSR_SetC(GetBit(registers->r[rd], rs_byte - 1));
    registers->r[rd] = LogicalShiftRight(registers->r[rd], rs_byte);
  } else if (rs_byte == 32) {
    CPSR_SetC(GetBit(registers->r[rd], 31));
    registers->r[rd] = 0;
  } else {
    CPSR_SetC(0);
    registers->r[rd] = 0;
  }

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LSR1(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rm = GetBitsInRange(instr, 3, 6);
  U32 immed_5 = GetBitsInRange(instr, 6, 11);

  if (immed_5 == 0) {
    CPSR_SetC(GetBit(registers->r[rd], 31));
    registers->r[rd] = 0;
  } else {
    CPSR_SetC(GetBit(registers->r[rd], immed_5 - 1));
    registers->r[rd] = LogicalShiftRight(registers->r[rm], immed_5);
  }
  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_ROR(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rs = GetBitsInRange(instr, 3, 6);

  if (GetBitsInRange(registers->r[rs], 0, 8) == 0) {
  } else if (GetBitsInRange(registers->r[rs], 0, 5) == 0) {
    CPSR_SetC(GetBit(registers->r[rd], 31));
  } else {
    CPSR_SetC(
        GetBit(registers->r[rd], GetBitsInRange(registers->r[rs], 0, 5) - 1));
    registers->r[rd] =
        RotateRight(registers->r[rd], GetBitsInRange(registers->r[rs], 0, 5));
  }
  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_ASR1(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rm = GetBitsInRange(instr, 3, 6);
  U32 immed_5 = GetBitsInRange(instr, 6, 11);

  if (immed_5 == 0) {
    CPSR_SetC(GetBit(registers->r[rm], 31));
    if (GetBit(registers->r[rm], 31) == 0) {
      registers->r[rd] = 0;
    } else {
      registers->r[rd] = 0xFFFFFFFF;
    }
  } else {
    CPSR_SetC(GetBit(registers->r[rd], immed_5 - 1));
    registers->r[rd] = ArithmeticShiftRight(registers->r[rm], immed_5);
  }

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_SUB1(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 immed_3 = GetBitsInRange(instr, 6, 9);

  registers->r[rd] = registers->r[rn] - immed_3;

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);
  CPSR_SetC(!UnsignedSubBorrow(registers->r[rn], immed_3));
  CPSR_SetV(SignedSubOverflow(registers->r[rn], immed_3));

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_SUB2(U16 instr) noexcept {
  U32 immed_8 = GetBitsInRange(instr, 0, 8);
  U32 rd = GetBitsInRange(instr, 8, 11);

  U32 original_rd = registers->r[rd];
  registers->r[rd] = original_rd - immed_8;

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);
  CPSR_SetC(!UnsignedSubBorrow(original_rd, immed_8));
  CPSR_SetV(SignedSubOverflow(original_rd, immed_8));

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_SUB3(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 rm = GetBitsInRange(instr, 6, 9);

  registers->r[rd] = U32(registers->r[rn]) - U32(registers->r[rm]);

  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);
  CPSR_SetC(!UnsignedSubBorrow(registers->r[rn], registers->r[rm]));
  CPSR_SetV(SignedSubOverflow(registers->r[rn], registers->r[rm]));

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_SUB4(U16 instr) noexcept {
  U32 immed_7 = GetBitsInRange(instr, 0, 7);
  registers->r[SP] -= (immed_7 << 2);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_TST(U16 instr) noexcept {
  U32 rn = GetBitsInRange(instr, 0, 3);
  U32 rm = GetBitsInRange(instr, 3, 6);
  U32 alu_out = registers->r[rn] & registers->r[rm];
  CPSR_SetN(GetBit(alu_out, 31));
  CPSR_SetZ(alu_out == 0);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_MUL(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rm = GetBitsInRange(instr, 3, 6);
  registers->r[rd] = registers->r[rd] * registers->r[rm];
  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_B1(U16 instr) noexcept {
  I32 immed_8 = SignExtend(GetBitsInRange(instr, 0, 8), 8);
  U32 cond = GetBitsInRange(instr, 8, 12);

  if (EvaluateCondition(ConditionCode(cond), registers->CPSR)) {
    registers->r[PC] = registers->r[PC] + (immed_8 << 1);
    ClearPipeline();
  } else {
    registers->r[PC] += 2;
  }
  return true;
}

bool CPU::Dispatch_Thumb_B2(U16 instr) noexcept {
  U32 immed_11 = GetBitsInRange(instr, 0, 11);
  registers->r[PC] += (SignExtend(immed_11, 11) << 1);
  ClearPipeline();
  return true;
}

bool CPU::Dispatch_Thumb_BIC(U16 instr) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rm = GetBitsInRange(instr, 3, 6);
  registers->r[rd] = registers->r[rd] & ~registers->r[rm];
  CPSR_SetN(GetBit(registers->r[rd], 31));
  CPSR_SetZ(registers->r[rd] == 0);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_BL(U16 instr) noexcept {
  U32 h = GetBitsInRange(instr, 11, 13);
  U32 offset_11 = GetBitsInRange(instr, 0, 11);

  if (h == 0b10) {
    registers->r[LR] = registers->r[PC] + (SignExtend(offset_11, 11) << 12);
    registers->r[PC] += 2;
  } else if (h == 0b11) {
    registers->r[PC] = registers->r[LR] + (offset_11 << 1);
    registers->r[LR] = (pipeline.execute_addr + 2) | 0b1;
    ClearPipeline();
  }

  return true;
}

bool CPU::Dispatch_Thumb_BX(U16 instr) noexcept {
  U32 rm = GetBitsInRange(instr, 3, 7);
  CPSR_SetT(GetBit(registers->r[rm], 0));
  registers->r[PC] = GetBitsInRange(registers->r[rm], 1, 32) << 1;
  ClearPipeline();
  return true;
}

bool CPU::Dispatch_Thumb_LDR1(U16 instr,
                              const Memory::Memory &memory) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 immed_5 = GetBitsInRange(instr, 6, 11);
  U32 address = registers->r[rn] + (immed_5 * 4);
  if (GetBitsInRange(address, 0, 2) == 0b00) {
    registers->r[rd] = ReadWordFromGBAMemory(memory, address);
  } else {
    LOG_ABORT("Bad address: 0x%04X", address);
  }
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LDR2(U16 instr,
                              const Memory::Memory &memory) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 rm = GetBitsInRange(instr, 6, 9);
  U32 address = registers->r[rn] + registers->r[rm];
  if (GetBitsInRange(address, 0, 2) == 0b00) {
    registers->r[rd] = ReadWordFromGBAMemory(memory, address);
  } else {
    LOG_ABORT("Bad address: 0x%04X", address);
  }
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LDR3(U16 instr,
                              const Memory::Memory &memory) noexcept {
  U32 immed_8 = GetBitsInRange(instr, 0, 8);
  U32 rd = GetBitsInRange(instr, 8, 11);
  U32 address = (GetBitsInRange(registers->r[PC], 2, 32) << 2) + immed_8 * 4;
  registers->r[rd] = ReadWordFromGBAMemory(memory, address);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LDR4(U16 instr,
                              const Memory::Memory &memory) noexcept {
  U32 rd = GetBitsInRange(instr, 8, 11);
  U32 immed_8 = GetBitsInRange(instr, 0, 8);
  U32 address = registers->r[SP] + (immed_8 * 4);
  if (GetBitsInRange(address, 0, 2) == 0b00) {
    registers->r[rd] = ReadWordFromGBAMemory(memory, address);
  } else {
    LOG_ABORT("Bad address: 0x%04X", address);
  }
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LDRB1(U16 instr,
                               const Memory::Memory &memory) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 immed_5 = GetBitsInRange(instr, 6, 11);
  U32 address = registers->r[rn] + immed_5;
  registers->r[rd] = ReadByteFromGBAMemory(memory, address);
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LDRH1(U16 instr,
                               const Memory::Memory &memory) noexcept {
  U32 immed_5 = GetBitsInRange(instr, 6, 11);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 address = registers->r[rn] + immed_5 * 2;
  U16 data;
  if ((address & 0b1) == 0) {
    data = ReadHalfWordFromGBAMemory(memory, address);
  } else {
    LOG_ABORT("Unpredictable");
  }
  registers->r[rd] = data;
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LDRH2(U16 instr,
                               const Memory::Memory &memory) noexcept {
  U32 rm = GetBitsInRange(instr, 6, 9);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 address = registers->r[rn] + registers->r[rm];
  U16 data;
  if ((address & 0b1) == 0) {
    data = ReadHalfWordFromGBAMemory(memory, address);
  } else {
    LOG_ABORT("Unpredictable");
  }
  registers->r[rd] = data;
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_LDMIA(U16 instr,
                               const Memory::Memory &memory) noexcept {
  U32 rn = GetBitsInRange(instr, 8, 11);
  U32 register_list = GetBitsInRange(instr, 0, 8);

  U32 start_address = registers->r[rn];
  U32 end_address = registers->r[rn] + (CountSetBits(register_list) * 4) - 4;
  U32 address = start_address;

  for (U32 i = 0; i < 8; ++i) {
    if (GetBit(register_list, i) == 1) {
      registers->r[i] = ReadWordFromGBAMemory(memory, address);
      address += 4;
    }
  }
  assert(end_address == address - 4);
  registers->r[rn] += (CountSetBits(register_list) * 4);

  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_PUSH(U16 instr, Memory::Memory &memory) noexcept {
  U32 register_list = GetBitsInRange(instr, 0, 8);
  U32 include_lr = GetBit(instr, 8);

  U32 start_address =
      registers->r[SP] - 4 * (include_lr + CountSetBits(register_list));
  U32 end_address = registers->r[SP] - 4;
  U32 address = start_address;
  for (U32 i = 0; i < 8; ++i) {
    if (GetBit(register_list, i) == 1) {
      WriteWordFromGBAMemory(memory, address, registers->r[i]);
      address += 4;
    }
  }
  if (include_lr == 1) {
    WriteWordFromGBAMemory(memory, address, registers->r[LR]);
    address += 4;
  }

  assert(end_address == address - 4);
  registers->r[SP] -= 4 * (include_lr + CountSetBits(register_list));

  registers->r[PC] += 2;

  return true;
}

bool CPU::Dispatch_Thumb_POP(U16 instr, const Memory::Memory &memory) noexcept {
  U32 register_list = GetBitsInRange(instr, 0, 8);
  U32 include_lr = GetBit(instr, 8);

  U32 start_address = registers->r[SP];
  U32 end_address =
      registers->r[SP] + 4 * (include_lr + CountSetBits(register_list));
  U32 address = start_address;
  for (U32 i = 0; i < 8; ++i) {
    if (GetBit(register_list, i) == 1) {
      registers->r[i] = ReadWordFromGBAMemory(memory, address);
      address += 4;
    }
  }
  if (include_lr == 1) {
    U32 value = ReadWordFromGBAMemory(memory, address);
    registers->r[PC] = value & 0xFFFFFFFE;
    ClearPipeline();
    address += 4;
  }

  assert(end_address == address);
  registers->r[SP] = end_address;

  if (include_lr == 0) {
    registers->r[PC] += 2;
  }
  return true;
}

bool CPU::Dispatch_Thumb_STRB1(U16 instr, Memory::Memory &memory) noexcept {
  U32 immed_5 = GetBitsInRange(instr, 6, 11);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 address = registers->r[rn] + immed_5;
  WriteByteFromGBAMemory(memory, address, U8(registers->r[rd]));
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_STR1(U16 instr, Memory::Memory &memory) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 immed_5 = GetBitsInRange(instr, 6, 11);
  U32 address = registers->r[rn] + immed_5 * 4;
  if (GetBitsInRange(address, 0, 2) == 0b00) {
    WriteWordFromGBAMemory(memory, address, registers->r[rd]);
  } else {
    LOG_ABORT("Unpredictable");
  }
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_STR2(U16 instr, Memory::Memory &memory) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 rm = GetBitsInRange(instr, 6, 9);
  U32 address = registers->r[rn] + registers->r[rm];
  if (GetBitsInRange(address, 0, 2) == 0b00) {
    WriteWordFromGBAMemory(memory, address, registers->r[rd]);
  } else {
    LOG_ABORT("Unpredictable");
  }
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_STR3(U16 instr, Memory::Memory &memory) noexcept {
  U32 immed_8 = GetBitsInRange(instr, 0, 8);
  U32 rd = GetBitsInRange(instr, 8, 11);
  U32 address = registers->r[SP] + (immed_8 * 4);
  if (GetBitsInRange(address, 0, 2) == 0b00) {
    WriteWordFromGBAMemory(memory, address, registers->r[rd]);
  } else {
    LOG_ABORT("Unpredicatable: Bad address");
  }
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_STMIA(U16 instr, Memory::Memory &memory) noexcept {
  U32 register_list = GetBitsInRange(instr, 0, 8);
  U32 rn = GetBitsInRange(instr, 8, 11);
  U32 start_address = registers->r[rn];
  U32 end_address = registers->r[rn] + (CountSetBits(register_list) * 4) - 4;
  U32 address = start_address;
  for (U32 i = 0; i < 8; ++i) {
    if (GetBit(register_list, i) == 1) {
      WriteWordFromGBAMemory(memory, address, registers->r[i]);
      address += 4;
    }
  }
  assert(end_address == address - 4);
  registers->r[rn] += CountSetBits(register_list) * 4;
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_STRH1(U16 instr, Memory::Memory &memory) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 immed_5 = GetBitsInRange(instr, 6, 11);

  U32 address = registers->r[rn] + (immed_5 * 4);
  if ((address & 0b1) == 0) {
    WriteHalfWordFromGBAMemory(memory, address, U16(registers->r[rd]));
  } else {
    LOG_ABORT("Unpredicatable: Bad address 0x%04X", address);
  }
  registers->r[PC] += 2;
  return true;
}

bool CPU::Dispatch_Thumb_STRH2(U16 instr, Memory::Memory &memory) noexcept {
  U32 rd = GetBitsInRange(instr, 0, 3);
  U32 rn = GetBitsInRange(instr, 3, 6);
  U32 rm = GetBitsInRange(instr, 6, 9);

  U32 address = registers->r[rn] + registers->r[rm];
  if ((address & 0b1) == 0) {
    WriteHalfWordFromGBAMemory(memory, address, U16(registers->r[rd]));
  } else {
    LOG_ABORT("Unpredicatable: Bad address 0x%04X", address);
  }
  registers->r[PC] += 2;
  return true;
}

void CPU::reset() noexcept {
  // On reset, start at SVC mode
  registers = &supervisor_registers;
  registers->CPSR = 0b10011;

  // link register is undefined at bootup. Hardcode to random value for
  // determinism.
  registers->r[14] = 9U;
}

} // namespace Emulator::Arm
