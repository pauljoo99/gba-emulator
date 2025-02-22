#pragma once

#include "arm968es_constants.h"
#include "game_card.h"

namespace Emulator::Arm

{

struct ConditionFlags {
  bool z;
  bool c;
  bool n;
  bool v;
};

struct CPU {
  [[nodiscard]] bool dispatch(const GameCard::GameCard &game_card) noexcept;

  uint32_t pc = 0;
  ConditionFlags cflags = {};
};

} // namespace Emulator::Arm
