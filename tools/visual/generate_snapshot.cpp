#include <cstdint>
#include <fstream>

struct Registers {
  U32 r[16];

  U32 r8_fiq;
  U32 r9_fiq;
  U32 r10_fiq;
  U32 r11_fiq;
  U32 r12_fiq;
  U32 r13_fiq;
  U32 r14_fiq;
  U32 SPSR_fiq;

  U32 r13_svc;
  U32 r14_svc;
  U32 SPSR_svc;

  U32 r13_abt;
  U32 r14_abt;
  U32 SPSR_abt;

  U32 r13_irq;
  U32 r14_irq;
  U32 SPSR_irq;

  U32 r13_und;
  U32 r14_und;
  U32 SPSR_und;

  U32 CPSR;
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
  U32 mem[] = {1, 2, 3, 4, 5, 6, 7};

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
  U32 program[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

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