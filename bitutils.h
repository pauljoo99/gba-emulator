#pragma once

#include "datatypes.h"
#include "logging.h"
#include <limits.h>

namespace Emulator::BitUtils {

inline U8 GetBit(U32 imm, U8 bit) { return (imm >> bit) & 0b1; }

// Get bits from [start, end)
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

inline U32 SetBitsInMask(U32 bits, U32 to_set, U32 mask) {
  return (bits & ~mask) | to_set;
}

inline I32 SignExtend(U32 imm, U8 num_bits) {
  U32 mask = ((1 << (32 - num_bits)) - GetBit(imm, num_bits - 1)) << num_bits;
  return static_cast<I32>(mask | imm);
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

inline U32 CountSetBits(U32 n) {
  U32 count = 0;
  while (n) {
    n &= (n - 1);
    count++;
  }
  return count;
}

inline bool UnsignedAddCarry(U32 a, U32 b) {
  U32 result = a + b;
  return result < a; // carry out occurred
}

inline bool UnsignedSubBorrow(U32 a, U32 b) {
  return a < b; // borrow occurred
}

inline bool SignedAddOverflow(I32 a, I32 b) {
  if (b > 0 && a > INT32_MAX - b)
    return true;
  if (b < 0 && a < INT32_MIN - b)
    return true;
  return false;
}

inline bool SignedSubOverflow(I32 a, I32 b) {
  if (b < 0 && a > INT32_MAX + b)
    return true;
  if (b > 0 && a < INT32_MIN + b)
    return true;
  return false;
}

} // namespace Emulator::BitUtils