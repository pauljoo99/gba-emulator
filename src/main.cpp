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
    if (val == 160) {
      WriteHalfWordToGBAMemoryMock(*memory, 0x04000202, 0b1);
    }

    for (U32 i = 0; i < 18; ++i) {
      U32 dma = ReadWordFromGBAMemory(*memory, 0x40000b0 + i * 4);
      if (dma != 0) {
        printf("DMA: 0x%04x, 0x%04x\n", dma, 0x40000b0 + i * 4);
      }
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
