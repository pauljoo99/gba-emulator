#pragma once

#include "arm7tdmi_constants.h"
#include "datatypes.h"
#include "game_card.h"
#include "memory.h"

namespace Emulator::Arm

{

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

  Registers registers;
  bool thumb_instr = 0;
};

} // namespace Emulator::Arm
