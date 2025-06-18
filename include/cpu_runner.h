#pragma once

namespace CpuRunner {

struct CpuRunner {
  bool Init(int argc, char *argv[]);
  void Run();
  void *Memory_;
  void *Cpu_;
  bool initialized = false;
};

} // namespace CpuRunner