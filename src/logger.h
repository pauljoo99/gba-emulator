#pragma once

#include "datatypes.h"
#include <fstream>
#include <iostream>

namespace Emulator::DispatchLogger {

constexpr U32 kMaxOpcodes = 1024 * 1024;

struct Context {
  U32 dispatch_num;
  U32 raw_instr;
  U32 execute_addr;
};
extern Context ctx;

struct OpcodeFields {
  U32 opcode : 8;
  U32 thumb : 1;
};

union Opcode {
  U32 value;
  OpcodeFields fields;
  Opcode(U32 val = 0) : value(val) {}
  operator U32() const { return value; } // Implicit conversion
};

struct DispatchInfo {
  U32 dispatch_num;
  U32 instr;
  U32 execute_addr;
  U32 opcode;
};

struct MemStoreInfo {
  U32 dispatch_num;
  U32 raw_instr;
  U32 execute_addr;
  U32 addr;
  U32 value;
};

struct MovInfo {
  U32 dispatch_num;
  U32 raw_instr;
  U32 execute_addr;
  U32 rd;
  U32 value;
};

struct LoadInfo {
  U32 dispatch_num;
  U32 raw_instr;
  U32 execute_addr;
  U32 address;
  U32 value;
};

struct DispatchLoggerT {
  U32 end_index_logs = 0;
  DispatchInfo dispatch_logs[kMaxOpcodes];

  U32 end_index_mem_store = 0;
  MemStoreInfo dispatch_mem_store[kMaxOpcodes];

  U32 end_index_mov = 0;
  MovInfo dispatch_mov[kMaxOpcodes];

  U32 end_index_load = 0;
  LoadInfo dispatch_load[kMaxOpcodes];
};
extern DispatchLoggerT DispatchLogger;

void SET_CONTEXT(U32 dispatch_num, U32 raw_instr, U32 addr);
void LOG_DISPATCH(U32 opcode, bool thumb);
void LOG_STORE(U32 addr, U32 value);
void LOG_MOV(U32 rd, U32 value);
void LOG_LOAD(U32 addr, U32 value);
void DUMP_LOGS();

} // namespace Emulator::DispatchLogger