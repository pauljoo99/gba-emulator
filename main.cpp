#include "include/cpu_runner.h"
#include "memory.h"
#include <chrono>
#include <memory>
#include <thread>

void MockDisplayLoop(Emulator::Memory::Memory *memory) {
  U16 dispcnt = 0;
  while (true) {
    // Write to VCOUNT
    U8 val = ReadByteFromGBAMemory(*memory, 0x04000006);
    WriteByteToGBAMemory(*memory, 0x04000006, val + 1);
    // Runs a 60Hz. 1/60 seconds per screen, 228 scanlines. Each scanline takes
    // 73 microsconds.
    std::this_thread::sleep_for(std::chrono::microseconds(73));

    U16 dispcnt_new = ReadHalfWordFromGBAMemory(*memory, 0x04000000);
    if (dispcnt != dispcnt_new) {
      LOG("dispcnt: 0x%04x", dispcnt_new);
      dispcnt = dispcnt_new;
    }
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
