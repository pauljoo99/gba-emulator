#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "src/arm_instructions.h"
#include "src/datatypes.h"
#include "src/logger.h"
#include "src/logging.h"
#include "src/thumb_instructions.h"

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

int log_reader(int argc, char *argv[]) {
  char filename[] = "/tmp/gba_log_dumper";
  if (!load_file(filename, (char *)&Emulator::DispatchLogger::DispatchLogger)) {
    return 1;
  }

  I32 end_arg = std::stoi(argv[2]);
  U32 num_steps_lookback = std::stoi(argv[3]);

  if (num_steps_lookback > Emulator::DispatchLogger::kMaxOpcodes) {
    LOG("Bad num_steps_lookback parameter");
  }

  U32 end = end_arg;
  if (end_arg == -1) {
    end = Emulator::DispatchLogger::DispatchLogger.end_index_logs - 1;
  }

  for (U32 i = 0; i < num_steps_lookback; ++i) {
    I32 index_t = end - i;
    U32 index = index_t;
    if (index_t < 0) {
      index = Emulator::DispatchLogger::kMaxOpcodes - index_t;
    }
    Emulator::DispatchLogger::DispatchInfo info =
        Emulator::DispatchLogger::DispatchLogger.dispatch_logs[index];
    char instr_type[100];
    char instr_opcode[100];
    Emulator::DispatchLogger::Opcode opcode(info.opcode);
    if (opcode.fields.thumb) {
      strcpy(instr_type, "Thumb");
      strcpy(instr_opcode,
             Emulator::Thumb::ToString(
                 Emulator::Thumb::ThumbOpcode(opcode.fields.opcode)));
    } else {
      strcpy(instr_type, "Normal");
      strcpy(instr_opcode, Emulator::Arm::ToString(
                               Emulator::Arm::Instr(opcode.fields.opcode)));
    }
    LOG("Step %u, Dispatch num: %u, Instr Type %s, Opcode %s, Raw Instr: "
        "0x%04x, Addr: 0x%04x",
        i, info.dispatch_num, instr_type, instr_opcode, info.instr,
        info.execute_addr);
  }
  return 0;
}

int store_reader(int argc, char *argv[]) {
  char filename[] = "/tmp/gba_log_dumper";
  if (!load_file(filename, (char *)&Emulator::DispatchLogger::DispatchLogger)) {
    return 1;
  }

  I32 end_arg = std::stoi(argv[2]);
  U32 num_steps_lookback = std::stoi(argv[3]);

  if (num_steps_lookback > Emulator::DispatchLogger::kMaxOpcodes) {
    LOG("Bad num_steps_lookback parameter");
  }

  U32 end = end_arg;
  if (end_arg == -1) {
    end = Emulator::DispatchLogger::DispatchLogger.end_index_mem_store - 1;
  }

  for (U32 i = 0; i < num_steps_lookback; ++i) {
    I32 index_t = end - i;
    U32 index = index_t;
    if (index_t < 0) {
      index = Emulator::DispatchLogger::kMaxOpcodes - index_t;
    }
    Emulator::DispatchLogger::MemStoreInfo info =
        Emulator::DispatchLogger::DispatchLogger.dispatch_mem_store[index];
    LOG("Store %u, Dispatch num: %u, Execute Addr: 0x%04x, Raw Instr: 0x%04x, "
        "Addr: 0x%04x, "
        "Value: "
        "0x%04x",
        i, info.dispatch_num, info.execute_addr, info.raw_instr, info.addr,
        info.value);
  }
  return 0;
}

int main(int argc, char *argv[]) {

  if (!strcmp(argv[1], "mem")) {
    return store_reader(argc, argv);
  } else {
    return log_reader(argc, argv);
  }
}