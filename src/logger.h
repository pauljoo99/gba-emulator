#pragma once

#include "datatypes.h"
#include <fstream>
#include <iostream>

namespace Emulator::DispatchLogger {

constexpr U32 kMaxOpcodes = 1024 * 1024;

struct OpcodeFields {
  U32 opcode : 8;
  U32 thumb : 1;
};

struct Opcode {
  U32 value;
  OpcodeFields fields;
  Opcode(U32 val = 0) : value(val) {}
  operator U32() const { return value; } // Implicit conversion
};

struct DispatchInfo {
  U32 instr;
  U32 execute_addr;
  U32 opcode;
};

struct DispatchLoggerT {
  U32 start_index = 0;
  U32 num_elements_added = 0;
  DispatchInfo dispatch_logs[kMaxOpcodes];
};
static DispatchLoggerT DispatchLogger;

inline void LOG_DISPATCH(U32 raw_instr, U32 addr, U32 opcode, bool thumb) {
  Opcode op;
  op.fields.opcode = opcode;
  op.fields.thumb = thumb;

  if (DispatchLogger.num_elements_added < kMaxOpcodes) {
    DispatchLogger.dispatch_logs[DispatchLogger.num_elements_added].instr =
        raw_instr;
    DispatchLogger.dispatch_logs[DispatchLogger.num_elements_added]
        .execute_addr = addr;
    DispatchLogger.dispatch_logs[DispatchLogger.num_elements_added].opcode = op;
    DispatchLogger.num_elements_added++;
  } else {
    DispatchLogger.dispatch_logs[DispatchLogger.start_index].instr = raw_instr;
    DispatchLogger.dispatch_logs[DispatchLogger.start_index].execute_addr =
        addr;
    DispatchLogger.dispatch_logs[DispatchLogger.start_index].opcode = op;
    DispatchLogger.start_index++;
  }
  if (DispatchLogger.start_index == kMaxOpcodes) {
    DispatchLogger.start_index = 0;
  }
}

inline void DUMP_LOGS() {
  // Open file in binary mode
  char file[] = "/tmp/gba_log_dumper";
  std::ofstream outFile(file, std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&DispatchLogger),
                sizeof(DispatchLogger));
  outFile.close();
}

} // namespace Emulator::DispatchLogger