#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "arm968es.h"
#include "game_card.h"
#include "memory.h"

using namespace Emulator;

static char game_file[] = "games/Pokemon - Emerald Version (U).gba";

[[nodiscard]] bool load_file(GameCard::GameCard &game_card) {
  FILE *file_pointer;
  long file_size;

  file_pointer = fopen(game_file, "rb");
  if (file_pointer == NULL) {
    perror("Error opening file");
    return false;
  }

  // Determine file size
  fseek(file_pointer, 0, SEEK_END);
  file_size = ftell(file_pointer);
  rewind(file_pointer);
  if (file_size > GameCard::kSize) {
    printf("File is too large %li", file_size);
    fclose(file_pointer);
    return false;
  }

  // Read data from file
  size_t bytes_read =
      fread(game_card.mem, sizeof(char), file_size, file_pointer);
  if (bytes_read != file_size) {
    perror("Error reading file");
    fclose(file_pointer);
    return false;
  }

  // Close the file
  fclose(file_pointer);

  return true;
}

int main() {

  std::unique_ptr<GameCard::GameCard> game_card =
      std::make_unique<GameCard::GameCard>();
  std::unique_ptr<Arm::CPU> cpu = std::make_unique<Arm::CPU>();
  std::unique_ptr<Memory::Memory> memory = std::make_unique<Memory::Memory>();

  if (!load_file(*game_card)) {
    printf("Could not load game!");
    return 1;
  }

  printf("Success");

  return 0;
}
