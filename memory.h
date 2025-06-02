#pragma once

#include "datatypes.h"
#include "memory_constants.h"
#include <assert.h>

namespace Emulator::Memory

{

#include <stdint.h>

typedef struct {
  // General Internal Memory
  U8 BIOS[0x4000];     // 00000000-00003FFF   BIOS - System ROM (16 KB)
  U8 _pad0[0x1FC0000]; // 00004000-01FFFFFF   Not used

  U8 WRAM_OnBoard[0x40000]; // 02000000-0203FFFF   On-board Work RAM (256 KB)
  U8 _pad1[0x2FC0000];      // 02040000-02FFFFFF   Not used

  U8 WRAM_OnChip[0x8000]; // 03000000-03007FFF   On-chip Work RAM (32 KB)
  U8 _pad2[0x3F8000];     // 03008000-03FFFFFF   Not used

  U8 IO_Registers[0x1FF]; // 04000000-040003FE   I/O Registers (read/write
                          // as 16-bit)
  U8 _pad3[0xFBFC02];     // 04000400-04FFFFFF   Not used

  // Internal Display Memory
  U8 PaletteRAM[0x200]; // 05000000-050003FF   BG/OBJ Palette RAM (1 KB)
  U8 _pad4[0xFBFC00];   // 05000400-05FFFFFF   Not used

  U8 VRAM[0x18000 / 2]; // 06000000-06017FFF   VRAM (96 KB)
  U8 _pad5[0xFEE800];   // 06018000-06FFFFFF   Not used

  U8 OAM[0x200];      // 07000000-070003FF   OBJ Attributes (1 KB)
  U8 _pad6[0xFBFC00]; // 07000400-07FFFFFF   Not used

  // External Memory (Game Pak)
  U8 GamePak_WS0[0x2000000]; // 08000000-09FFFFFF   Game Pak ROM/Flash -
                             // Wait State 0
  U8 GamePak_WS1[0x2000000]; // 0A000000-0BFFFFFF   Game Pak ROM/Flash -
                             // Wait State 1
  U8 GamePak_WS2[0x2000000]; // 0C000000-0DFFFFFF   Game Pak ROM/Flash -
                             // Wait State 2

  U8 GamePak_SRAM[0x10000]; // 0E000000-0E00FFFF   Game Pak SRAM (64 KB)
  U8 _pad7[0x1FF0000];      // 0E010000-0FFFFFFF   Not used

  // Unused Memory Area
  U8 _pad8[0xF0000000]; // 10000000-FFFFFFFF   Not used
} Memory;

inline const U8 *GetPhysicalMemoryReadOnly(const Memory &mem,
                                           U32 address) noexcept {
  if (address < 0x00004000) {
    return &mem.BIOS[address - 0x00000000];
  } else if (address >= 0x02000000 && address < 0x02040000) {
    return &mem.WRAM_OnBoard[address - 0x02000000];
  } else if (address >= 0x03000000 && address < 0x03008000) {
    return &mem.WRAM_OnChip[address - 0x03000000];
  } else if (address >= 0x04000000 && address < 0x040003FE) {
    return &((U8 *)mem.IO_Registers)[address - 0x04000000];
  } else if (address >= 0x05000000 && address < 0x05000400) {
    return &((U8 *)mem.PaletteRAM)[address - 0x05000000];
  } else if (address >= 0x06000000 && address < 0x06018000) {
    return &((U8 *)mem.VRAM)[address - 0x06000000];
  } else if (address >= 0x07000000 && address < 0x07000400) {
    return &((U8 *)mem.OAM)[address - 0x07000000];
  } else if (address >= 0x08000000 && address < 0x0A000000) {
    return &mem.GamePak_WS0[address - 0x08000000];
  } else if (address >= 0x0A000000 && address < 0x0C000000) {
    return &mem.GamePak_WS1[address - 0x0A000000];
  } else if (address >= 0x0C000000 && address < 0x0E000000) {
    return &mem.GamePak_WS2[address - 0x0C000000];
  } else if (address >= 0x0E000000 && address < 0x0E010000) {
    return &mem.GamePak_SRAM[address - 0x0E000000];
  } else {
    // Out of bounds or unused memory
    return nullptr;
  }
}

inline U8 *GetPhysicalMemoryReadWrite(Memory &mem, U32 address) noexcept {
  if (address < 0x00004000) {
    return &mem.BIOS[address - 0x00000000];
  } else if (address >= 0x02000000 && address < 0x02040000) {
    return &mem.WRAM_OnBoard[address - 0x02000000];
  } else if (address >= 0x03000000 && address < 0x03008000) {
    return &mem.WRAM_OnChip[address - 0x03000000];
  } else if (address >= 0x04000000 && address < 0x040003FE) {
    return &((U8 *)mem.IO_Registers)[address - 0x04000000];
  } else if (address >= 0x05000000 && address < 0x05000400) {
    return &((U8 *)mem.PaletteRAM)[address - 0x05000000];
  } else if (address >= 0x06000000 && address < 0x06018000) {
    return &((U8 *)mem.VRAM)[address - 0x06000000];
  } else if (address >= 0x07000000 && address < 0x07000400) {
    return &((U8 *)mem.OAM)[address - 0x07000000];
  } else if (address >= 0x08000000 && address < 0x0A000000) {
    return &mem.GamePak_WS0[address - 0x08000000];
  } else if (address >= 0x0A000000 && address < 0x0C000000) {
    return &mem.GamePak_WS1[address - 0x0A000000];
  } else if (address >= 0x0C000000 && address < 0x0E000000) {
    return &mem.GamePak_WS2[address - 0x0C000000];
  } else if (address >= 0x0E000000 && address < 0x0E010000) {
    return &mem.GamePak_SRAM[address - 0x0E000000];
  } else {
    // Out of bounds or unused memory
    return nullptr;
  }
}

inline U8 ReadByteFromGBAMemory(const Memory &mem, U32 address) noexcept {
  U8 value;
  memcpy(&value, GetPhysicalMemoryReadOnly(mem, address), sizeof(value));
  return value;
}

inline U32 ReadWordFromGBAMemory(const Memory &mem, U32 address) noexcept {
  U32 value;
  memcpy(&value, GetPhysicalMemoryReadOnly(mem, address), sizeof(value));
  return value;
}

inline void WriteByteFromGBAMemory(Memory &mem, U32 address,
                                   U8 value) noexcept {
  memcpy(GetPhysicalMemoryReadWrite(mem, address), &value, sizeof(value));
}

inline void WriteWordFromGBAMemory(Memory &mem, U32 address,
                                   U32 value) noexcept {
  memcpy(GetPhysicalMemoryReadWrite(mem, address), &value, sizeof(value));
}

inline void Reset(Memory &mem) {
  // POSTFLG is set to 0 on reset and 1 after bootup.
  WriteWordFromGBAMemory(mem, 0x4000300, 0x00);
}

} // namespace Emulator::Memory