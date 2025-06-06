#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "arm7tdmi.h"
#include "logging.h"
#include "memory.h"

using namespace Emulator;

[[nodiscard]] bool load_file(char *file_name, char *bytes) {
  FILE *file_pointer;
  U64 size;

  file_pointer = fopen(file_name, "rb");
  if (file_pointer == NULL) {
    perror("Error opening file");
    return false;
  }

  // Determine file size
  fseek(file_pointer, 0, SEEK_END);
  size = ftell(file_pointer);
  rewind(file_pointer);

  // Read data from file
  size_t bytes_read = fread(bytes, sizeof(char), size, file_pointer);
  if (bytes_read != size) {
    perror("Error reading file");
    fclose(file_pointer);
    return false;
  }

  // Close the file
  fclose(file_pointer);

  return true;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <name>" << std::endl;
    return 1;
  }
  char *file_name = argv[1];

  std::unique_ptr<Arm::CPU> cpu = std::make_unique<Arm::CPU>();
  std::unique_ptr<Memory::Memory> memory = std::make_unique<Memory::Memory>();

  // Load BIOS
  if (!load_file(file_name, (char *)memory->BIOS)) {
    LOG("Could not load bios!");
    return 1;
  }

  cpu->reset();
  Reset(*memory);
  while (cpu->Dispatch(*memory)) {
  }

  LOG("Dispatch failed!");
  return 1;
}
