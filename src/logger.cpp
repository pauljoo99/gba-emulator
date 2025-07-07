// DispatchLogger.cpp
#include "logger.h"
#include <fstream>

namespace Emulator::DispatchLogger {

DispatchLoggerT DispatchLogger;
Context ctx;

void SET_CONTEXT(U32 dispatch_num, U32 raw_instr, U32 addr) {
  ctx.dispatch_num = dispatch_num;
  ctx.raw_instr = raw_instr;
  ctx.execute_addr = addr;
}

void LOG_DISPATCH(U32 opcode, bool thumb) {
  Opcode op;
  op.fields.opcode = opcode;
  op.fields.thumb = thumb;

  DispatchLogger.dispatch_logs[DispatchLogger.end_index_logs] =
      DispatchInfo{ctx.dispatch_num, ctx.raw_instr, ctx.execute_addr, op};
  DispatchLogger.end_index_logs =
      (DispatchLogger.end_index_logs + 1) % kMaxOpcodes;
}

void LOG_STORE(U32 addr, U32 value) {
  DispatchLogger.dispatch_mem_store[DispatchLogger.end_index_mem_store] =
      MemStoreInfo{.dispatch_num = ctx.dispatch_num,
                   .raw_instr = ctx.raw_instr,
                   .execute_addr = ctx.execute_addr,
                   .addr = addr,
                   .value = value};
  DispatchLogger.end_index_mem_store =
      (DispatchLogger.end_index_mem_store + 1) % kMaxOpcodes;
}

void LOG_MOV(U32 rd, U32 value) {
  DispatchLogger.dispatch_mov[DispatchLogger.end_index_mov] =
      MovInfo{.dispatch_num = ctx.dispatch_num,
              .raw_instr = ctx.raw_instr,
              .execute_addr = ctx.execute_addr,
              .rd = rd,
              .value = value};
  DispatchLogger.end_index_mov =
      (DispatchLogger.end_index_mov + 1) % kMaxOpcodes;
}

void LOG_LOAD(U32 addr, U32 value) {
  DispatchLogger.dispatch_load[DispatchLogger.end_index_load] =
      LoadInfo{.dispatch_num = ctx.dispatch_num,
               .raw_instr = ctx.raw_instr,
               .execute_addr = ctx.execute_addr,
               .address = addr,
               .value = value};
  DispatchLogger.end_index_load =
      (DispatchLogger.end_index_load + 1) % kMaxOpcodes;
}

void DUMP_LOGS() {
  std::ofstream outFile("/tmp/gba_log_dumper", std::ios::binary);
  if (!outFile)
    return;
  outFile.write(reinterpret_cast<const char *>(&DispatchLogger),
                sizeof(DispatchLogger));
  outFile.close();
}

} // namespace Emulator::DispatchLogger
