// DispatchLogger.cpp
#include "logger.h"
#include <fstream>

namespace Emulator::DispatchLogger {

Logs Logger;
Context ctx;

void SET_CONTEXT(U32 instr, U32 addr, U32 thumb, U32 opcode) {
  ctx.instr = instr;
  ctx.addr = addr;
  ctx.thumb = thumb;
  ctx.opcode = opcode;

  Logger.log_type[Logger.log_type_end_idx] = LogType::CONTEXT;
  *(Context *)(&Logger.raw_data[Logger.log_type_end_idx * sizeof(Context)]) =
      ctx;
  Logger.log_type_end_idx = (Logger.log_type_end_idx + 1) % kMaxLogs;
}

void LOG_STORE(U32 addr, U32 value) {
  Logger.log_type[Logger.log_type_end_idx] = LogType::STORE;
  *(Str *)(&Logger.raw_data[Logger.log_type_end_idx * sizeof(Context)]) =
      Str{.addr = addr, .val = value};
  Logger.log_type_end_idx = (Logger.log_type_end_idx + 1) % kMaxLogs;
}

void LOG_MOV(U32 rd, U32 value) {
  Logger.log_type[Logger.log_type_end_idx] = LogType::MOV;
  *(Mov *)(&Logger.raw_data[Logger.log_type_end_idx * sizeof(Context)]) =
      Mov{.rd = rd, .val = value};
  Logger.log_type_end_idx = (Logger.log_type_end_idx + 1) % kMaxLogs;
}

void DUMP_LOGS() {
  std::ofstream outFile("/tmp/gba_log_dumper", std::ios::binary);
  if (!outFile)
    return;
  outFile.write(reinterpret_cast<const char *>(&Logger), sizeof(Logs));
  outFile.close();
}

} // namespace Emulator::DispatchLogger
