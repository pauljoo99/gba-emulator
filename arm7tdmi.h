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
constexpr U32 SL = 10;
constexpr U32 FP = 11;
constexpr U32 IP = 12;

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

  [[nodiscard]] bool Dispatch(Memory::Memory &memory) noexcept;

  [[nodiscard]] bool Dispatch_B(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_BL(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_BIC(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_BX(U32 instr) noexcept;

  [[nodiscard]] bool Dispatch_CMP(U32 instr) noexcept;

  [[nodiscard]] bool Dispatch_MOV(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_MSR(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_LDR(U32 instr,
                                  const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_LDRB(U32 instr,
                                   const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_LDRSB(U32 instr,
                                    const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_LDRH(U32 instr,
                                   const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_LDRSH(U32 instr,
                                    const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_LDM(U32 instr,
                                  const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_TEQ(U32 instr_) noexcept;
  [[nodiscard]] bool Dispatch_TST(U32 instr_) noexcept;
  [[nodiscard]] bool Dispatch_MRS(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_ORR(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_EOR(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_CMN(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_SUB(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_RSB(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_ADD(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_MUL(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_ADC(U32 instr) noexcept;
  [[nodiscard]] bool Dispatch_AND(U32 instr) noexcept;

  [[nodiscard]] bool AdvancePipeline(U32 instr, U32 addr) noexcept;

  [[nodiscard]] bool Dispatch_STM(U32 instr, Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_STR(U32 instr, Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_STRB(U32 instr, Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_STRH(U32 instr, Memory::Memory &memory) noexcept;

  [[nodiscard]] bool Dispatch_Thumb_CMN(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_CMP1(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_CMP2(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_CMP3(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_MOV1(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_MOV3(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_MVN(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_AND(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_ADD1(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_ADD2(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_ADD3(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_ADD5(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_ADD6(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_ADD7(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_NEG(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_ORR(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_EOR(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_SUB1(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_SUB2(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_SUB3(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_SUB4(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_LSL1(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_LSL2(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_LSR1(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_ROR(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_LDR1(U16 instr,
                                         const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_LDR2(U16 instr,
                                         const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_LDR3(U16 instr,
                                         const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_LDR4(U16 instr,
                                         const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool
  Dispatch_Thumb_LDRB1(U16 instr, const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_ASR1(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_B1(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_B2(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_BIC(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_BL(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_BX(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_TST(U16 instr) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_MUL(U16 instr) noexcept;

  [[nodiscard]] bool
  Dispatch_Thumb_LDRH1(U16 instr, const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool
  Dispatch_Thumb_LDRH2(U16 instr, const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool
  Dispatch_Thumb_LDMIA(U16 instr, const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_STRB1(U16 instr,
                                          Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_STR1(U16 instr,
                                         Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_STR2(U16 instr,
                                         Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_STR3(U16 instr,
                                         Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_STMIA(U16 instr,
                                          Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_STRH1(U16 instr,
                                          Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_STRH2(U16 instr,
                                          Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_PUSH(U16 instr,
                                         Memory::Memory &memory) noexcept;
  [[nodiscard]] bool Dispatch_Thumb_POP(U16 instr,
                                        const Memory::Memory &memory) noexcept;

  void EnterException_IRQ() noexcept;

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
  U32 LoadAndStoreWordOrByteAddr(SingleDataTransferInstr instr) noexcept;

  U32 LoadAndStoreMiscImmAddr(U32 instr_) noexcept;
  U32 LoadAndStoreMiscRegAddr(U32 instr_) noexcept;
  U32 LoadAndStoreMiscAddr(SingleDataTransferInstr instr) noexcept;

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

  U32 dispatch_num = 0;

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
      ABORT("Undefined Mode %u", cpsr.bits.M);
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

  inline void CPSR_SetI(bool I) noexcept {
    CPSR_Register cpsr;
    cpsr.bits.I = I;
    CPSR_Register mask;
    mask.bits.I = 1;
    registers->CPSR = BitUtils::SetBitsInMask(registers->CPSR, cpsr, mask);
  }

  inline void CPSR_SetT(bool T) noexcept {
    CPSR_Register cpsr;
    cpsr.bits.T = T;
    CPSR_Register mask;
    mask.bits.T = 1;
    registers->CPSR = BitUtils::SetBitsInMask(registers->CPSR, cpsr, mask);
  }

  inline void CPSR_SetM(U8 M) noexcept {
    CPSR_Register cpsr;
    cpsr.bits.M = M;
    CPSR_Register mask;
    mask.bits.M = 0b11111;
    registers->CPSR = BitUtils::SetBitsInMask(registers->CPSR, cpsr, mask);
  }
};

} // namespace Emulator::Arm
