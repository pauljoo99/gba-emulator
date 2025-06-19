#include "include/cpu_runner.h"
#include "memory.h"
#include <chrono>
#include <memory>
#include <thread>

void MockDisplayLoop(Emulator::Memory::Memory *memory) {
  while (true) {
    // Write to VCOUNT
    U8 val = ReadByteFromGBAMemory(*memory, 0x04000006);
    WriteByteToGBAMemory(*memory, 0x04000006, val + 1);

    // Trigger VCOUNT interrupt
    U32 IRQ_ENABLED = ReadWordFromGBAMemory(*memory, 0x04000200);
    if (((IRQ_ENABLED & 0b1) == 1) && val == 160) {
      WriteHalfWordToGBAMemoryMock(*memory, 0x04000202, 0b1);
    }

    // Runs a 60Hz. 1/60 seconds per screen, 228 scanlines. Each scanline takes
    // 73 microsconds.
    std::this_thread::sleep_for(std::chrono::microseconds(73));
  }
}

int main(int argc, char *argv[]) {

  std::unique_ptr<CpuRunner::CpuRunner> cpu_runner =
      std::make_unique<CpuRunner::CpuRunner>();

  cpu_runner->Init(argc, argv);

  Emulator::Memory::Memory *memory =
      (Emulator::Memory::Memory *)cpu_runner->Memory_;
  std::thread MockDisplay(MockDisplayLoop, memory);

  cpu_runner->Run();

  return 1;
}
