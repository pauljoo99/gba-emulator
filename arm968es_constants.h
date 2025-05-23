#pragma once

#include <cstdint>

namespace Emulator::Arm

{

// Instruction size in bytes
constexpr uint8_t kInstrSize = 4;

// Instruction size in bytes
constexpr uint8_t kThumbInstrSize = 2;

// Instruction size in bytes
constexpr uint8_t kThumb2InstrSize = 4;

} // namespace Emulator::Arm