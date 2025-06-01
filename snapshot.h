#include "arm7tdmi.h"
#include "datatypes.h"
#include "memory.h"

#pragma once

namespace Emulator::Arm::Debug {

static constexpr U32 MAX_PATH_LENGHT = 100;

static constexpr char snapshot_dir[] = "snapshot";

static constexpr char registers_bin[] = "registers.bin";
static constexpr char memory_bin[] = "memory.bin";
static constexpr char program_bin[] = "program.bin";

void write_registers(const AllRegisters &registers, const char *path);

void write_memory(const Emulator::Memory::Memory &mem, const char *path);

void write_program(const Emulator::GameCard::GameCard &card, const char *path);

void debug_snapshot(const AllRegisters &registers,
                    const Emulator::Memory::Memory &mem,
                    const Emulator::GameCard::GameCard &card, const char *path);

} // namespace Emulator::Arm::Debug