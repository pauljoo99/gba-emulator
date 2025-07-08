#pragma once

#include "datatypes.h"
#include <fstream>
#include <iostream>

namespace Emulator::DispatchLogger {

constexpr U32 kMaxLogs = 1024 * 1024;

enum class LogType : U8 { CONTEXT = 0, STORE = 1, MOV = 2, LOAD = 3 };

struct Context {
  U32 instr;
  U32 addr;
  U32 thumb;
  U32 opcode;
};
extern Context ctx;

struct Mov {
  U32 rd;
  U32 val;
};

struct Str {
  U32 addr;
  U32 val;
};

struct Ld {
  U32 addr;
  U32 val;
};

struct Logs {
  U32 log_type_end_idx = 0;
  LogType log_type[kMaxLogs];
  U8 raw_data[kMaxLogs * sizeof(Context)];
};
extern Logs Logger;

void SET_CONTEXT(U32 instr, U32 addr, U32 thumb, U32 opcode);
void LOG_STORE(U32 addr, U32 value);
void LOG_LOAD(U32 addr, U32 value);
void LOG_MOV(U32 rd, U32 value);
void DUMP_LOGS();

} // namespace Emulator::DispatchLogger