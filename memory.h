#pragma once

#include "memory_constants.h"

namespace Emulator::Memory

{

struct Memory {
  unsigned char mem[kSize];
};

} // namespace Emulator::Memory