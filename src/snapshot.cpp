#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>

#include "logging.h"
#include "snapshot.h"

namespace Emulator::Arm::Debug {

void write_registers(const AllRegisters &registers, const char *path) {
  // Open file in binary mode
  char file[MAX_PATH_LENGHT];
  strcpy(file, path);
  strcat(file, registers_bin);
  std::ofstream outFile(file, std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&registers), sizeof(registers));
  outFile.close();
}

void write_memory(const Emulator::Memory::Memory &mem, const char *path) {
  // Open file in binary mode
  char file[MAX_PATH_LENGHT];
  strcpy(file, path);
  strcat(file, memory_bin);
  std::ofstream outFile(file, std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&mem), sizeof(mem));
  outFile.close();
}

void write_pipeline(const Pipeline &pipeline, const char *path) {
  // Open file in binary mode
  char file[MAX_PATH_LENGHT];
  strcpy(file, path);
  strcat(file, pipeline_bin);
  std::ofstream outFile(file, std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&pipeline), sizeof(pipeline));
  outFile.close();
}

void debug_snapshot(const AllRegisters &registers,
                    const Emulator::Memory::Memory &mem,
                    const Pipeline &pipeline, const char *path) {
  static U32 snapshot_num = 0;

  LOG("Writing Snapshot: %u", snapshot_num);

  char snapshot_path[MAX_PATH_LENGHT];
  snprintf(snapshot_path, sizeof(snapshot_path), "%s%s_%u/", path, snapshot_dir,
           snapshot_num++);

  if (mkdir(snapshot_path, 0777) != -1 && errno != EEXIST) {
    perror("mkdir");
  }

  write_registers(registers, snapshot_path);
  write_memory(mem, snapshot_path);
  write_pipeline(pipeline, snapshot_path);
}

} // namespace Emulator::Arm::Debug