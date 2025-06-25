#include "include/cpu_runner.h"

#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

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
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <name>" << std::endl;
    return false;
  }
  char *bios_name = argv[1];
  char *game_name = argv[2];

  Arm::CPU *cpu = new Arm::CPU();
  Memory::Memory *memory = new Memory::Memory();

  Cpu_ = (void *)cpu;
  Memory_ = (void *)memory;

  // Load BIOS
  if (!load_file(bios_name, (char *)memory->BIOS)) {
    LOG_VERBOSE("Could not load bios!");
    return false;
  }

  // Load Game Cartridge
  if (!load_file(game_name, (char *)memory->GamePak_WS0)) {
    LOG_VERBOSE("Could not load game!");
    return false;
  }

  cpu->reset();
  Reset(*memory);

  initialized = true;
  return true;
};

void CpuRunner::Run() {
  Arm::CPU *cpu = (Arm::CPU *)Cpu_;
  Memory::Memory *memory = (Memory::Memory *)Memory_;
  if (!initialized) {
    LOG("CpuRunner was not initialized!");
  } else {
    while (cpu->Dispatch(*memory)) {
      // Clock time of GBA is 16.57 MHz.
      std::this_thread::sleep_for(std::chrono::nanoseconds(59));
    }
    LOG("CpuRunner stopped running!");
  }
  free(cpu);
  free(memory);
  return;
};

} // namespace CpuRunner