#pragma once

#include "arm7tdmi_constants.h"
#include "arm_instructions.h"
#include "bitutils.h"
#include "datatypes.h"
#include "logging.h"
#include "memory.h"
#include <cstdlib>

namespace Emulator::Arm

{

constexpr U32 LR = 14;
constexpr U32 SP = 13;
constexpr U32 PC = 15;

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

enum Mode {
  USER,
  SYSTEM,
  SUPERVISOR,
  ABORT,
  UNDEFINED,
  INTERRUPT,
  FAST_INTERRUPT
};

struct AllRegisters {
  U32 r[16];

  U32 r8_fiq;
  U32 r9_fiq;
  U32 r10_fiq;
  U32 r11_fiq;
  U32 r12_fiq;
  U32 r13_fiq;
  U32 r14_fiq;
  U32 SPRS_fiq;

  U32 r13_svc;
  U32 r14_svc;
  U32 SPRS_svc;

  U32 r13_abt;
  U32 r14_abt;
  U32 SPRS_abt;

  U32 r13_irq;
  U32 r14_irq;
  U32 SPRS_irq;

  U32 r13_und;
  U32 r14_und;
  U32 SPRS_und;

  U32 CPSR;
};

struct Registers {
  U32Ref r[16];
  U32Ref SPRS;
  U32Ref CPSR;
};

struct Pipeline {
  U32 fetch = U32(-1);
  U32 decode = U32(-1);
  U32 execute = U32(-1);

  U32 fetch_addr = U32(-1);
  U32 decode_addr = U32(-1);
  U32 execute_addr = U32(-1);
};

/* Based on ARM DDI 0100E */
struct CPU {

  void reset() noexcept;

  [[nodiscard]] bool dispatch(Memory::Memory &memory) noexcept;

  [[nodiscard]] bool dispatch_B(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_BL(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_BX(U32 instr) noexcept;

  [[nodiscard]] bool dispatch_CMP(U32 instr) noexcept;

  [[nodiscard]] bool dispatch_MOV(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_MSR(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_LDR(U32 instr,
                                  const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_TEQ(U32 instr_) noexcept;
  [[nodiscard]] bool dispatch_MRS(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_ORR(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_CMN(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_SUB(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_ADD(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_AND(U32 instr) noexcept;

  [[nodiscard]] bool AdvancePipeline(U32 instr, U32 addr) noexcept;

  [[nodiscard]] bool dispatch_STM(U32 instr, Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_STR(U32 instr, Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_LDM(U32 instr,
                                  const Memory::Memory &memory) noexcept;

  [[nodiscard]] bool dispatch_thumb_CMP1(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_MOV1(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_MOV3(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_MVN(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_ADD1(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_ADD2(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_ADD3(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_ORR(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_SUB1(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_SUB3(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_SUB4(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_LSL1(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_LSL2(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_LSR1(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_ASR1(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_B1(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_B2(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_BL(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_BX(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_TST(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_LDR3(U16 instr,
                                         const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool
  dispatch_thumb_LDRH1(U16 instr, const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool
  dispatch_thumb_LDRH2(U16 instr, const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_thumb_STR2(U16 instr,
                                         Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_thumb_STR3(U16 instr,
                                         Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_thumb_STRH1(U16 instr,
                                          Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_thumb_STRH2(U16 instr,
                                          Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_thumb_PUSH(U16 instr,
                                         Memory::Memory &memory) noexcept;

  ShifterOperandResult ShifterOperand(DataProcessingInstr instr) noexcept;

  ShifterOperandResult
  ShifterOperandImmediate(DataProcessingInstrImmediate operand_2) noexcept;
  ShifterOperandResult ShifterOperandLogicalShiftLeftByImm(
      DataProcessingInstrShiftByImm operand_2) noexcept;
  ShifterOperandResult ShifterOperandLogicalShiftLeftByRegister(
      DataProcessingInstrShiftByRegister operand_2) noexcept;
  ShifterOperandResult ShifterOperandLogicalShiftRightByImm(
      DataProcessingInstrShiftByImm operand_2) noexcept;
  ShifterOperandResult ShifterOperandLogicalShiftRightByRegister(
      DataProcessingInstrShiftByRegister operand_2) noexcept;
  ShifterOperandResult ShifterOperandArithmeticShiftRightByImm(
      DataProcessingInstrShiftByImm operand_2) noexcept;
  ShifterOperandResult ShifterOperandArithmeticShiftRightByRegister(
      DataProcessingInstrShiftByRegister operand_2) noexcept;
  ShifterOperandResult ShifterOperandRotateRightByImm(
      DataProcessingInstrShiftByImm operand_2) noexcept;
  ShifterOperandResult ShifterOperandRotateRightByRegister(
      DataProcessingInstrShiftByRegister operand_2) noexcept;
  ShifterOperandResult ShifterOperandRotateRightByExtend(
      DataProcessingInstrShiftByRegister operand_2) noexcept;

  U32 LoadAndStoreWordOrByteImmAddr(U32 instr_) noexcept;
  U32 LoadAndStoreWordOrByteRegAddr(U32 instr_) noexcept;

  LoadAndStoreMultipleAddrResult LoadAndStoreMultipleAddr(U32 instr_) noexcept;

  void ClearPipeline() noexcept;

  AllRegisters all_registers;
  Registers user_registers{
      .r = {&all_registers.r[0], &all_registers.r[1], &all_registers.r[2],
            &all_registers.r[3], &all_registers.r[4], &all_registers.r[5],
            &all_registers.r[6], &all_registers.r[7], &all_registers.r[8],
            &all_registers.r[9], &all_registers.r[10], &all_registers.r[11],
            &all_registers.r[12], &all_registers.r[13], &all_registers.r[14],
            &all_registers.r[15]},
      .CPSR = &all_registers.CPSR,
      .SPRS = nullptr};
  Registers system_registers{
      .r = {&all_registers.r[0], &all_registers.r[1], &all_registers.r[2],
            &all_registers.r[3], &all_registers.r[4], &all_registers.r[5],
            &all_registers.r[6], &all_registers.r[7], &all_registers.r[8],
            &all_registers.r[9], &all_registers.r[10], &all_registers.r[11],
            &all_registers.r[12], &all_registers.r[13], &all_registers.r[14],
            &all_registers.r[15]},
      .CPSR = &all_registers.CPSR,
      .SPRS = nullptr};
  Registers supervisor_registers{
      .r = {&all_registers.r[0], &all_registers.r[1], &all_registers.r[2],
            &all_registers.r[3], &all_registers.r[4], &all_registers.r[5],
            &all_registers.r[6], &all_registers.r[7], &all_registers.r[8],
            &all_registers.r[9], &all_registers.r[10], &all_registers.r[11],
            &all_registers.r[12], &all_registers.r13_svc,
            &all_registers.r14_svc, &all_registers.r[15]},
      .CPSR = &all_registers.CPSR,
      .SPRS = &all_registers.SPRS_svc};
  Registers abort_registers{
      .r = {&all_registers.r[0], &all_registers.r[1], &all_registers.r[2],
            &all_registers.r[3], &all_registers.r[4], &all_registers.r[5],
            &all_registers.r[6], &all_registers.r[7], &all_registers.r[8],
            &all_registers.r[9], &all_registers.r[10], &all_registers.r[11],
            &all_registers.r[12], &all_registers.r13_abt,
            &all_registers.r14_abt, &all_registers.r[15]},
      .CPSR = &all_registers.CPSR,
      .SPRS = &all_registers.SPRS_abt};
  Registers undefined_registers{
      .r = {&all_registers.r[0], &all_registers.r[1], &all_registers.r[2],
            &all_registers.r[3], &all_registers.r[4], &all_registers.r[5],
            &all_registers.r[6], &all_registers.r[7], &all_registers.r[8],
            &all_registers.r[9], &all_registers.r[10], &all_registers.r[11],
            &all_registers.r[12], &all_registers.r13_und,
            &all_registers.r14_und, &all_registers.r[15]},
      .CPSR = &all_registers.CPSR,
      .SPRS = &all_registers.SPRS_und};
  Registers interrupt_registers{
      .r = {&all_registers.r[0], &all_registers.r[1], &all_registers.r[2],
            &all_registers.r[3], &all_registers.r[4], &all_registers.r[5],
            &all_registers.r[6], &all_registers.r[7], &all_registers.r[8],
            &all_registers.r[9], &all_registers.r[10], &all_registers.r[11],
            &all_registers.r[12], &all_registers.r13_irq,
            &all_registers.r14_irq, &all_registers.r[15]},
      .CPSR = &all_registers.CPSR,
      .SPRS = &all_registers.SPRS_irq};
  Registers fast_interrupt_registers{
      .r = {&all_registers.r[0], &all_registers.r[1], &all_registers.r[2],
            &all_registers.r[3], &all_registers.r[4], &all_registers.r[5],
            &all_registers.r[6], &all_registers.r[7], &all_registers.r8_fiq,
            &all_registers.r9_fiq, &all_registers.r10_fiq,
            &all_registers.r11_fiq, &all_registers.r12_fiq,
            &all_registers.r13_fiq, &all_registers.r13_fiq,
            &all_registers.r[15]},
      .CPSR = &all_registers.CPSR,
      .SPRS = &all_registers.SPRS_fiq};

  Registers *registers;
  Pipeline pipeline;

  inline Mode GetMode() {
    CPSR_Register cpsr(registers->CPSR);
    switch (cpsr.bits.M) {
    case 0b10000:
      return Mode::USER;
    case 0b10001:
      return Mode::FAST_INTERRUPT;
    case 0b10010:
      return Mode::INTERRUPT;
    case 0b10011:
      return Mode::SUPERVISOR;
    case 0b10111:
      return Mode::ABORT;
    case 0b11011:
      return Mode::UNDEFINED;
    case 0b11111:
      return Mode::SYSTEM;
    default:
      LOG_ABORT("Undefined Mode %u", cpsr.bits.M);
    }
  }

  inline void ChangeRegistersOnMode() {
    switch (GetMode()) {
    case Mode::USER:
      registers = &user_registers;
      break;
    case Mode::FAST_INTERRUPT:
      registers = &fast_interrupt_registers;
      break;
    case Mode::INTERRUPT:
      registers = &interrupt_registers;
      break;
    case Mode::SUPERVISOR:
      registers = &supervisor_registers;
      break;
    case Mode::ABORT:
      registers = &abort_registers;
      break;
    case Mode::UNDEFINED:
      registers = &undefined_registers;
      break;
    case Mode::SYSTEM:
      registers = &system_registers;
      break;
    }
  }

  inline void CPSR_SetC(bool C) noexcept {
    CPSR_Register cpsr;
    cpsr.bits.C = C;
    CPSR_Register mask;
    mask.bits.C = 1;
    registers->CPSR = BitUtils::SetBitsInMask(registers->CPSR, cpsr, mask);
  }

  inline void CPSR_SetV(bool V) noexcept {
    CPSR_Register cpsr;
    cpsr.bits.V = V;
    CPSR_Register mask;
    mask.bits.V = 1;
    registers->CPSR = BitUtils::SetBitsInMask(registers->CPSR, cpsr, mask);
  }

  inline void CPSR_SetZ(bool Z) noexcept {
    CPSR_Register cpsr;
    cpsr.bits.Z = Z;
    CPSR_Register mask;
    mask.bits.Z = 1;
    registers->CPSR = BitUtils::SetBitsInMask(registers->CPSR, cpsr, mask);
  }

  inline void CPSR_SetN(bool N) noexcept {
    CPSR_Register cpsr;
    cpsr.bits.N = N;
    CPSR_Register mask;
    mask.bits.N = 1;
    registers->CPSR = BitUtils::SetBitsInMask(registers->CPSR, cpsr, mask);
  }
};

} // namespace Emulator::Arm
