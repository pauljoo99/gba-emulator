#include "include/cpu_runner.h"
#include <memory>

int main(int argc, char *argv[]) {

  std::unique_ptr<CpuRunner::CpuRunner> cpu_runner =
      std::make_unique<CpuRunner::CpuRunner>();

  cpu_runner->Init(argc, argv);
  cpu_runner->Run();

  return 1;
}
