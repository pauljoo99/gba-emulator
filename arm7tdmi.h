#pragma once

#include "arm7tdmi_constants.h"
#include "arm_instructions.h"
#include "datatypes.h"
#include "game_card.h"
#include "memory.h"

namespace Emulator::Arm

{

enum Mode {
  USER,
  SYSTEM,
  SUPERVISOR,
  ABORT,
  UNDEFINED,
  INTERRUPT,
  FAST_INTERRUPT
};

struct Registers {
  U32 r[16];

  U32 r8_fiq;
  U32 r9_fiq;
  U32 r10_fiq;
  U32 r11_fiq;
  U32 r12_fiq;
  U32 r13_fiq;
  U32 r14_fiq;
  U32 SPSR_fiq;

  U32 r13_svc;
  U32 r14_svc;
  U32 SPSR_svc;

  U32 r13_abt;
  U32 r14_abt;
  U32 SPSR_abt;

  U32 r13_irq;
  U32 r14_irq;
  U32 SPSR_irq;

  U32 r13_und;
  U32 r14_und;
  U32 SPSR_und;

  U32 CPSR;
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

  [[nodiscard]] bool dispatch(const GameCard::GameCard &game_card,
                              const Memory::Memory &memory) noexcept;

  [[nodiscard]] bool dispatch_B(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_BX(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_MOV(U32 instr,
                                  const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_MSR(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_LDR(U32 instr,
                                  const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_ADD(U32 instr) noexcept;
  [[nodiscard]] bool dispatch_STR(U32 instr_,
                                  const Memory::Memory &memory) noexcept;

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

  void clearPipeline() noexcept;
  void clearPipelineThumb() noexcept;

  Registers registers;
  Pipeline pipeline;
  PipelineThumb pipeline_thumb;
  bool thumb_instr = false;

  Mode mode;

  inline U32 *GetSPRS() {
    switch (mode) {
    case USER:
      return nullptr;
    case SYSTEM:
      return nullptr;
    case SUPERVISOR:
      return &registers.SPSR_svc;
    case ABORT:
      return &registers.SPSR_abt;
    case UNDEFINED:
      return &registers.SPSR_und;
    case INTERRUPT:
      return &registers.SPSR_irq;
    case FAST_INTERRUPT:
      return &registers.SPSR_fiq;
    }
  }
};

} // namespace Emulator::Arm
