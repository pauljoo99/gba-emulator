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
}