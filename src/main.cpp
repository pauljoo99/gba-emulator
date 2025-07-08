#include "include/cpu_runner.h"
#include "logging.h"
#include "memory.h"
#include <chrono>
#include <csignal>
#include <memory>
#include <thread>

std::atomic<bool> keepRunning(true);

void signalHandler(int signal) {
  if (signal == SIGINT) {
    ABORT("Exiting gracefully");
    keepRunning = false;
  }
}

void MockDisplayLoop(Emulator::Memory::Memory *memory) {
  while (true) {
    // Write to VCOUNT
    U8 val = ReadByteFromGBAMemory(*memory, 0x04000006);
    WriteByteToGBAMemory(*memory, 0x04000006, val + 1);

    // Trigger VCOUNT interrupt
    if (val == 160) {
      WriteHalfWordToGBAMemoryMock(*memory, 0x04000202, 0b1);
    }

    // Runs a 60Hz. 1/60 seconds per screen, 228 scanlines. Each scanline takes
    // 73 microsconds.
    std::this_thread::sleep_for(std::chrono::microseconds(73));
  }
}

int main(int argc, char *argv[]) {

  std::signal(SIGINT, signalHandler);

  std::unique_ptr<CpuRunner::CpuRunner> cpu_runner =
      std::make_unique<CpuRunner::CpuRunner>();

  cpu_runner->Init(argc, argv);

  Emulator::Memory::Memory *memory =
      (Emulator::Memory::Memory *)cpu_runner->Memory_;
  std::thread MockDisplay(MockDisplayLoop, memory);

  cpu_runner->Run();

  return 1;
}
