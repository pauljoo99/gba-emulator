#pragma once

#include "arm968es_constants.h"
#include "game_card.h"
#include "memory.h"

namespace Emulator::Arm

{

struct Registers {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t r10;
  uint32_t r11;
  uint32_t r12;
  uint32_t r13;
  uint32_t r14;
  uint32_t r15;

  uint32_t r8_fiq;
  uint32_t r9_fiq;
  uint32_t r10_fiq;
  uint32_t r11_fiq;
  uint32_t r12_fiq;
  uint32_t r13_fiq;
  uint32_t r14_fiq;
  uint32_t SPSR_fiq;

  uint32_t r13_svc;
  uint32_t r14_svc;
  uint32_t SPSR_svc;

  uint32_t r13_abt;
  uint32_t r14_abt;
  uint32_t SPSR_abt;

  uint32_t r13_irq;
  uint32_t r14_irq;
  uint32_t SPSR_irq;

  uint32_t r13_und;
  uint32_t r14_und;
  uint32_t SPSR_und;

  uint32_t CPSR;
};

struct CPU {

  [[nodiscard]] bool dispatch(const GameCard::GameCard &game_card,
                              const Memory::Memory &memory) noexcept;

  [[nodiscard]] bool dispatch_B(uint32_t instr) noexcept;
  [[nodiscard]] bool dispatch_MOV(uint32_t instr,
                                  const Memory::Memory &memory) noexcept;
  [[nodiscard]] bool dispatch_MSR(uint32_t instr,
                                  const Memory::Memory &memory) noexcept;

  Registers registers;
};

} // namespace Emulator::Arm
