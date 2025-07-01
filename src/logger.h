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
  U32 end_index = 0;
  DispatchInfo dispatch_logs[kMaxOpcodes];
};
extern DispatchLoggerT DispatchLogger;

void LOG_DISPATCH(U32 raw_instr, U32 addr, U32 opcode, bool thumb);
void DUMP_LOGS();

} // namespace Emulator::DispatchLogger