#include "include/cpu_runner.h"

#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "arm7tdmi.h"
#include "logging.h"
#include "memory.h"

namespace CpuRunner {

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

bool CpuRunner::Init(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <name>" << std::endl;
    return false;
  }
  char *file_name = argv[1];

  Arm::CPU *cpu = new Arm::CPU();
  Memory::Memory *memory = new Memory::Memory();

  Cpu_ = (void *)cpu;
  Memory_ = (void *)memory;

  // Load BIOS
  if (!load_file(file_name, (char *)memory->BIOS)) {
    LOG("Could not load bios!");
    return false;
  }

  cpu->reset();
  Reset(*memory);

  initialized = true;
  return true;
};

void CpuRunner::Run() {
  if (!initialized) {
    LOG_DEBUG("CpuRunner was not initialized!");
    return;
  }

  Arm::CPU *cpu = (Arm::CPU *)Cpu_;
  Memory::Memory *memory = (Memory::Memory *)Memory_;
  while (cpu->Dispatch(*memory)) {
  }
  LOG_DEBUG("CpuRunner stopped running!");
  return;
};

} // namespace CpuRunner