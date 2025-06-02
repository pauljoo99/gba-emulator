#pragma once

#include "arm7tdmi_constants.h"
#include "arm_instructions.h"
#include "datatypes.h"
#include "game_card.h"
#include "logging.h"
#include "memory.h"
#include <cstdlib>

namespace Emulator::Arm

{

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
};

struct PipelineThumb {
  U16 fetch = U16(-1);
  U16 decode = U16(-1);
  U16 execute = U16(-1);
};

/* Based on ARM DDI 0100E */
struct CPU {

  inline CPU() { SetupRegisters(); }

  void reset() noexcept;

  [[nodiscard]] bool dispatch(const GameCard::GameCard &game_card,
                              const Memory::Memory &memory) noexcept;

  [[nodiscard]] bool dispatch_B(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_BX(U32 instr) noexcept;

  [[nodiscard]] bool dispatch_CMP(U32 instr) noexcept;

  [[nodiscard]] bool dispatch_MOV(U32 instr,
                                  const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_MSR(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_LDR(U32 instr,
                                  const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_STR(U32 instr_,
                                  const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_TEQ(U32 instr_) noexcept;

  [[nodiscard]] bool dispatch_thumb_LSL(U16 instr) noexcept;
  [[nodiscard]] bool dispatch_thumb_BLX(U16 instr) noexcept;

  [[nodiscard]] bool advance_pipeline(U32 instr) noexcept;
  [[nodiscard]] bool advance_pipeline(U16 instr) noexcept;

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

  void clearPipeline() noexcept;
  void clearPipelineThumb() noexcept;

  AllRegisters all_registers;
  Registers user_registers;
  Registers system_registers;
  Registers supervisor_registers;
  Registers abort_registers;
  Registers undefined_registers;
  Registers interrupt_registers;
  Registers fast_interrupt_registers;

  Registers *registers;

  Pipeline pipeline;
  PipelineThumb pipeline_thumb;
  bool thumb_instr = false;

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
      std::abort();
      return Mode::USER;
    }
  }

  inline void SetupRegisters() {
    // User registers
    for (int i = 0; i < 16; ++i) {
      user_registers.r[i] = &all_registers.r[i];
    }
    user_registers.CPSR = &all_registers.CPSR;
    user_registers.SPRS = nullptr;

    // System registers
    for (int i = 0; i < 16; ++i) {
      system_registers.r[i] = &all_registers.r[i];
    }
    system_registers.CPSR = &all_registers.CPSR;
    system_registers.SPRS = nullptr;

    // Supervisor registers
    for (int i = 0; i < 13; ++i) {
      supervisor_registers.r[i] = &all_registers.r[i];
    }
    supervisor_registers.r[13] = &all_registers.r13_svc;
    supervisor_registers.r[14] = &all_registers.r14_svc;
    supervisor_registers.r[15] = &all_registers.r[15];
    supervisor_registers.CPSR = &all_registers.CPSR;
    supervisor_registers.SPRS = &all_registers.SPRS_svc;

    // Abort registers
    for (int i = 0; i < 13; ++i) {
      abort_registers.r[i] = &all_registers.r[i];
    }
    abort_registers.r[13] = &all_registers.r13_abt;
    abort_registers.r[14] = &all_registers.r14_abt;
    abort_registers.r[15] = &all_registers.r[15];
    abort_registers.CPSR = &all_registers.CPSR;
    abort_registers.SPRS = &all_registers.SPRS_abt;

    // Undefined registers
    for (int i = 0; i < 13; ++i) {
      undefined_registers.r[i] = &all_registers.r[i];
    }
    undefined_registers.r[13] = &all_registers.r13_und;
    undefined_registers.r[14] = &all_registers.r14_und;
    undefined_registers.r[15] = &all_registers.r[15];
    undefined_registers.CPSR = &all_registers.CPSR;
    undefined_registers.SPRS = &all_registers.SPRS_und;

    // Interrupt registers
    for (int i = 0; i < 13; ++i) {
      interrupt_registers.r[i] = &all_registers.r[i];
    }
    interrupt_registers.r[13] = &all_registers.r13_irq;
    interrupt_registers.r[14] = &all_registers.r14_irq;
    interrupt_registers.r[15] = &all_registers.r[15];
    interrupt_registers.CPSR = &all_registers.CPSR;
    interrupt_registers.SPRS = &all_registers.SPRS_irq;

    // Fast interrupt registers
    for (int i = 0; i < 8; ++i) {
      fast_interrupt_registers.r[i] = &all_registers.r[i];
    }
    fast_interrupt_registers.r[8] = &all_registers.r8_fiq;
    fast_interrupt_registers.r[9] = &all_registers.r9_fiq;
    fast_interrupt_registers.r[10] = &all_registers.r10_fiq;
    fast_interrupt_registers.r[11] = &all_registers.r11_fiq;
    fast_interrupt_registers.r[12] = &all_registers.r12_fiq;
    fast_interrupt_registers.r[13] = &all_registers.r13_fiq;
    fast_interrupt_registers.r[14] = &all_registers.r14_fiq;
    fast_interrupt_registers.r[15] = &all_registers.r[15];
    fast_interrupt_registers.CPSR = &all_registers.CPSR;
    fast_interrupt_registers.SPRS = &all_registers.SPRS_fiq;
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
};

} // namespace Emulator::Arm
