#include "arm968es.h"
#include "memory.h"

#pragma once

namespace Emulator::Arm::Debug {

static constexpr char registers_bin[] = "tools/visual/data/registers.bin";
static constexpr char memory_bin[] = "tools/visual/data/memory.bin";
static constexpr char program_bin[] = "tools/visual/data/program.bin";

void write_registers(const Registers &registers);

void write_memory(const Emulator::Memory::Memory &mem);

void write_program(const Emulator::GameCard::GameCard &card);

inline void debug_snapshot(const Registers &registers,
                           const Emulator::Memory::Memory &mem,
                           const Emulator::GameCard::GameCard &card) {
  write_registers(registers);
  write_memory(mem);
  write_program(card);
}

} // namespace Emulator::Arm::Debug