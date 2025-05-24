#pragma once

#include "datatypes.h"
#include "game_card_constants.h"

namespace Emulator::GameCard

{

struct GameCard {
  unsigned char mem[kSize];
};

} // namespace Emulator::GameCard