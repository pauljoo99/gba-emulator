#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "src/datatypes.h"
#include "src/logger.h"
#include "src/logging.h"

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

  char filename[] = "/tmp/gba_log_dumper";
  if (!load_file(filename, (char *)&Emulator::DispatchLogger::DispatchLogger)) {
    return 1;
  }

  I32 end_arg = std::stoi(argv[1]);
  U32 num_steps_lookback = std::stoi(argv[2]);

  if (num_steps_lookback > Emulator::DispatchLogger::kMaxOpcodes) {
    LOG("Bad num_steps_lookback parameter");
  }

  U32 end = end_arg;
  if (end_arg == -1) {
    end = Emulator::DispatchLogger::DispatchLogger.end_index - 1;
  }

  for (U32 i = 0; i < num_steps_lookback; ++i) {
    I32 index_t = end - i;
    U32 index = index_t;
    if (index_t < 0) {
      index = Emulator::DispatchLogger::kMaxOpcodes - index_t;
    }
    Emulator::DispatchLogger::DispatchInfo info =
        Emulator::DispatchLogger::DispatchLogger.dispatch_logs[index];
    LOG("Step %u, Raw Instr: 0x%04x, Addr: 0x%04x", i, info.instr,
        info.execute_addr);
    LOG("%u", Emulator::DispatchLogger::DispatchLogger.end_index);
  }
  return 0;
}