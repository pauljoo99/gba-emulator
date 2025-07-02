// DispatchLogger.cpp
#include "logger.h"
#include <fstream>

namespace Emulator::DispatchLogger {

DispatchLoggerT DispatchLogger;

void LOG_DISPATCH(U32 raw_instr, U32 addr, U32 opcode, bool thumb) {
  Opcode op;
  op.fields.opcode = opcode;
  op.fields.thumb = thumb;

  DispatchLogger.dispatch_logs[DispatchLogger.end_index_logs] =
      DispatchInfo{raw_instr, addr, op};
  DispatchLogger.end_index_logs =
      (DispatchLogger.end_index_logs + 1) % kMaxOpcodes;
}

void LOG_MEMORY_STORE(U32 raw_instr, U32 execute_addr, U32 addr, U32 value) {
  DispatchLogger.dispatch_mem_store[DispatchLogger.end_index_mem_store] =
      MemStoreInfo{.raw_instr = raw_instr,
                   .execute_addr = execute_addr,
                   .addr = addr,
                   .value = value};
  DispatchLogger.end_index_mem_store =
      (DispatchLogger.end_index_mem_store + 1) % kMaxOpcodes;
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
