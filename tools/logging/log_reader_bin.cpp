#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "src/arm_instructions.h"
#include "src/datatypes.h"
#include "src/logger.h"
#include "src/logging.h"
#include "src/thumb_instructions.h"

using namespace Emulator::DispatchLogger;

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

int emit_logs(int argc, char *argv[]) {
  char filename[] = "/tmp/gba_log_dumper";
  if (!load_file(filename, (char *)&Logger)) {
    return 1;
  }

  U32 log_idx = Logger.log_type_end_idx - 1;
  for (U32 i = 0; i < std::stoi(argv[1]); ++i) {
    LogType log_type = Logger.log_type[log_idx];

    switch (log_type) {
    case LogType::CONTEXT: {

      auto &ctx = *(Context *)(&Logger.raw_data[log_idx * sizeof(Context)]);
      char opcode[100];
      strcpy(opcode,
             ctx.thumb == 1
                 ? Emulator::Thumb::ToString(
                       (Emulator::Thumb::ThumbOpcode)ctx.opcode)
                 : Emulator::Arm::ToString((Emulator::Arm::Instr)ctx.opcode));
      printf("Context: instr=0x%04x, addr=0x%04x, thumb=%d, opcode=%s\n",
             ctx.instr, ctx.addr, ctx.thumb, opcode);
      break;
    }
    case LogType::STORE: {
      auto &store = *(Str *)(&Logger.raw_data[log_idx * sizeof(Context)]);
      printf("Store: addr=0x%04x, value=0x%04x\n", store.addr, store.val);
      break;
    }
    case LogType::MOV: {
      auto &mov = *(Mov *)(&Logger.raw_data[log_idx * sizeof(Context)]);
      printf("Move: rd=%u, value=0x%04x\n", mov.rd, mov.val);
      break;
    }
    }
    if (log_idx == 0) {
      log_idx = kMaxLogs;
    }
    log_idx--;
  }

  return 0;
}

int main(int argc, char *argv[]) { return emit_logs(argc, argv); }