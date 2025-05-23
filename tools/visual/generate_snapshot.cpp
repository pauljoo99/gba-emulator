#include <cstdint>
#include <fstream>

struct Registers {
  uint32_t r[16];

  uint32_t r8_fiq;
  uint32_t r9_fiq;
  uint32_t r10_fiq;
  uint32_t r11_fiq;
  uint32_t r12_fiq;
  uint32_t r13_fiq;
  uint32_t r14_fiq;
  uint32_t SPSR_fiq;

  uint32_t r13_svc;
  uint32_t r14_svc;
  uint32_t SPSR_svc;

  uint32_t r13_abt;
  uint32_t r14_abt;
  uint32_t SPSR_abt;

  uint32_t r13_irq;
  uint32_t r14_irq;
  uint32_t SPSR_irq;

  uint32_t r13_und;
  uint32_t r14_und;
  uint32_t SPSR_und;

  uint32_t CPSR;
};

void write_registers() {
  Registers registers = {
      .r = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
      .r8_fiq = 1,
      .r9_fiq = 2,
      .r10_fiq = 3,
      .r11_fiq = 4,
      .r12_fiq = 5,
      .r13_fiq = 6,
      .r14_fiq = 7,
      .SPSR_fiq = 8,
      .r13_svc = 9,
      .r14_svc = 10,
      .SPSR_svc = 11,
      .r13_abt = 12,
      .r14_abt = 13,
      .SPSR_abt = 14,
      .r13_irq = 15,
      .r14_irq = 16,
      .SPSR_irq = 17,
      .r13_und = 18,
      .r14_und = 19,
      .SPSR_und = 20,
      .CPSR = 21,
  };

  // Open file in binary mode
  std::ofstream outFile("data/registers.bin", std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&registers), sizeof(registers));
  outFile.close();
}

void write_memory() {
  uint32_t mem[] = {1, 2, 3, 4, 5, 6, 7};

  // Open file in binary mode
  std::ofstream outFile("data/memory.bin", std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&mem), sizeof(mem));
  outFile.close();
}

void write_program() {
  uint32_t program[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // Open file in binary mode
  std::ofstream outFile("data/program.bin", std::ios::binary);

  if (!outFile) {
    // Handle error
    return;
  }

  // Write bytes to file
  outFile.write(reinterpret_cast<const char *>(&program), sizeof(program));
  outFile.close();
}

int main() {
  write_registers();
  write_memory();
  write_program();
}