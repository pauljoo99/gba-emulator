#pragma once

#include "datatypes.h"
#include "logging.h"

namespace Emulator::BitUtils {

inline U8 GetBit(U32 imm, U8 bit) { return (imm >> bit) & 0b1; }

inline U32 GetBitsInRange(U32 imm, U8 start, U8 end) {
  if (end == 32) {
    return imm >> start;
  }

  U32 mask = (1 << end) - 1;
  return (imm & mask) >> start;
}

inline U32 ConcatBits(U32 left, U32 right, U8 num_right_bits) {
  return (left << num_right_bits) | right;
}

inline I64 SignExtend(U32 imm, U8 num_bits) {
  U32 mask = ((1 << (32 - num_bits)) - GetBit(imm, num_bits - 1)) << num_bits;
  return static_cast<I64>(mask | imm);
}

inline U32 RotateRight(U32 imm, U8 num_bits) {
  U32 dropped_right = ((1 << num_bits) - 1) & imm;
  imm = imm >> num_bits;
  dropped_right = dropped_right << (32 - num_bits);
  return imm | dropped_right;
}

inline U32 LogicalShiftLeft(U32 imm, U8 num_bits) { return imm << num_bits; }
inline U32 LogicalShiftRight(U32 imm, U8 num_bits) { return imm >> num_bits; }

inline I32 ArithmeticShiftRight(I32 imm, U8 num_bits) {
  return imm >> num_bits;
}

} // namespace Emulator::BitUtils