#include <stdint.h>
#include <stdio.h>

#include "src/datatypes.h"
#include "src/display_utils.h"
#include "src/logging.h"
#include "src/memory.h"

int main(int argc, char *argv[]) {

  char *memory_bin = argv[1];

  FILE *file_pointer;
  U64 size;

  file_pointer = fopen(memory_bin, "rb");
  if (file_pointer == NULL) {
    perror("Error opening file");
    return 0;
  }

  // Determine file size
  fseek(file_pointer, 0, SEEK_END);
  size = ftell(file_pointer);
  rewind(file_pointer);

  // Read data from file
  Emulator::Memory::Memory *memory_ptr = new Emulator::Memory::Memory();
  Emulator::Memory::Memory &memory = *memory_ptr;
  size_t bytes_read = fread(memory_ptr, sizeof(char), size, file_pointer);
  if (bytes_read != size) {
    perror("Error reading file");
    fclose(file_pointer);
    return 0;
  }

  // Close the file
  fclose(file_pointer);

  U16 DISPCNT = ReadHalfWordFromGBAMemory(memory, Emulator::DISPCNT_ADDR);
  LOG("DISPCNT: 0x%04x", DISPCNT);

  return 0;
}
