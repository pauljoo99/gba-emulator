#include <cassert>

#include "bitutils.h"
#include "logging.h"

using namespace Emulator::BitUtils;

int main() {
  assert(GetBit(0x000F, 0) == 1);
  assert(GetBit(0x000F, 1) == 1);
  assert(GetBit(0x000F, 2) == 1);
  assert(GetBit(0x000F, 3) == 1);
  assert(GetBit(0x002F, 4) == 0);
  assert(GetBit(0x002F, 5) == 1);

  assert(ConcatBits(0x00C0, 0x000F, 4) == 0x0C0F);
  assert(ConcatBits(0x0001, 0x0000, 1) == 0x0002);

  assert(SignExtend(0x0000, 1) == 0x0000);
  assert(SignExtend(0x0001, 1) == 0xFFFFFFFF);
  assert(SignExtend(0x0002, 3) == 0x0002);
  assert(SignExtend(0x0002, 2) == 0xFFFFFFFE);

  assert(RotateRight(0x12345678, 0) == 0x12345678);
  assert(RotateRight(0x12345678, 32) == 0x12345678);
  assert(RotateRight(0x12345678, 4) == 0x81234567);
  assert(RotateRight(0xFF00FF00, 8) == 0x00FF00FF);
  assert(RotateRight(0xA5A5A5A5, 16) == 0xA5A5A5A5);
  assert(RotateRight(0xFFFFFFFF, 13) == 0xFFFFFFFF);
  assert(RotateRight(0x80000000, 1) == 0x40000000);
  assert(RotateRight(0x80000000, 31) == 0x00000001);

  // Get bit 4 (single bit range)
  assert(GetBitsInRange(0b00010000, 4, 5) == 1); // bit 4 is 1
  assert(GetBitsInRange(0b00000000, 4, 5) == 0); // bit 4 is 0

  // Get bits 2 to 6 (not including 6)
  assert(GetBitsInRange(0b10110100, 2, 6) == 0b1101); // bits 5–2 → 1101

  // Lower nibble: bits 0–4 (excludes bit 4)
  assert(GetBitsInRange(0xAB, 0, 4) == 0xB); // bits 3–0

  // Upper nibble: bits 4–8 (excludes 8)
  assert(GetBitsInRange(0xAB, 4, 8) == 0xA); // bits 7–4

  // All 8 bits
  assert(GetBitsInRange(0xAB, 0, 8) == 0xAB);

  // Bits 28–32 from full 32-bit value
  assert(GetBitsInRange(0xF0F0F0F0, 28, 32) == 0xF); // top 4 bits

  // Zero-width range
  assert(GetBitsInRange(0xFFFF0000, 5, 5) == 0); // empty range → 0
}