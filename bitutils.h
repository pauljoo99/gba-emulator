#pragma once

#include "datatypes.h"
#include "logging.h"

namespace Emulator::BitUtils {

inline U8 GetBit(U32 imm, U8 bit) { return (imm >> bit) & 0b1; }

inline U32 ConcatBits(U32 left, U32 right, U8 num_right_bits) {
  return (left << num_right_bits) | right;
}

inline I64 SignExtend(U32 imm, U8 num_bits) {
  U32 mask = ((1 << (32 - num_bits)) - GetBit(imm, num_bits - 1)) << num_bits;
  LOG("%u", imm);
  return static_cast<I64>(mask | imm);
}

} // namespace Emulator::BitUtils