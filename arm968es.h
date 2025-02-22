#pragma once

#include "arm968es_constants.h"

namespace Emulator::Arm

{

struct ConditionFlags {
  bool z;
  bool c;
  bool n;
  bool v;
};

struct CPU {
  [[nodiscard]] bool dispatch();

  uint32_t pc = 0;
  ConditionFlags cflags = {};
};

} // namespace Emulator::Arm
