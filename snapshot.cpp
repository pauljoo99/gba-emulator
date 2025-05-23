#include <fstream>

#include "snapshot.h"

namespace Emulator::Arm::Debug {

void write_registers(const Registers &registers) {
  // Open file in binary mode
  std::ofstream outFile(registers_bin, std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&registers), sizeof(registers));
  outFile.close();
}

void write_memory(const Emulator::Memory::Memory &mem) {
  // Open file in binary mode
  std::ofstream outFile(memory_bin, std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&mem.mem), sizeof(mem.mem));
  outFile.close();
}

void write_program(const Emulator::GameCard::GameCard &card) {
  // Open file in binary mode
  std::ofstream outFile(program_bin, std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&card.mem), sizeof(card.mem));
  outFile.close();
}

} // namespace Emulator::Arm::Debug