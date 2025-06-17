#include "include/cpu_runner.h"
#include <stdio.h>

namespace CpuRunner {

void CpuRunner::Run() {
  VramBuffer[0] = 1;
  VramBuffer[1] = 2;
  printf("Hello world, %i", VramBuffer[0]);
};

} // namespace CpuRunner