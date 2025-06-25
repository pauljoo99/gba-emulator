#pragma once

#include "datatypes.h"

namespace Emulator {
constexpr U32 DISPCNT_ADDR = 0x04000000;
constexpr U32 GREEN_SWAP_ADDR = 0x04000002; // undocumented
constexpr U32 DISPSTAT_ADDR = 0x04000004;
constexpr U32 VCOUNT_ADDR = 0x04000006;

constexpr U32 BG0CNT_ADDR = 0x04000008;
constexpr U32 BG1CNT_ADDR = 0x0400000A;
constexpr U32 BG2CNT_ADDR = 0x0400000C;
constexpr U32 BG3CNT_ADDR = 0x0400000E;

constexpr U32 BG0HOFS_ADDR = 0x04000010;
constexpr U32 BG0VOFS_ADDR = 0x04000012;
constexpr U32 BG1HOFS_ADDR = 0x04000014;
constexpr U32 BG1VOFS_ADDR = 0x04000016;
constexpr U32 BG2HOFS_ADDR = 0x04000018;
constexpr U32 BG2VOFS_ADDR = 0x0400001A;
constexpr U32 BG3HOFS_ADDR = 0x0400001C;
constexpr U32 BG3VOFS_ADDR = 0x0400001E;

constexpr U32 BG2PA_ADDR = 0x04000020;
constexpr U32 BG2PB_ADDR = 0x04000022;
constexpr U32 BG2PC_ADDR = 0x04000024;
constexpr U32 BG2PD_ADDR = 0x04000026;
constexpr U32 BG2X_ADDR = 0x04000028;
constexpr U32 BG2Y_ADDR = 0x0400002C;

constexpr U32 BG3PA_ADDR = 0x04000030;
constexpr U32 BG3PB_ADDR = 0x04000032;
constexpr U32 BG3PC_ADDR = 0x04000034;
constexpr U32 BG3PD_ADDR = 0x04000036;
constexpr U32 BG3X_ADDR = 0x04000038;
constexpr U32 BG3Y_ADDR = 0x0400003C;

constexpr U32 WIN0H_ADDR = 0x04000040;
constexpr U32 WIN1H_ADDR = 0x04000042;
constexpr U32 WIN0V_ADDR = 0x04000044;
constexpr U32 WIN1V_ADDR = 0x04000046;

constexpr U32 WININ_ADDR = 0x04000048;
constexpr U32 WINOUT_ADDR = 0x0400004A;

constexpr U32 MOSAIC_ADDR = 0x0400004C;
// 0x0400004E unused

constexpr U32 BLDCNT_ADDR = 0x04000050;
constexpr U32 BLDALPHA_ADDR = 0x04000052;
constexpr U32 BLDY_ADDR = 0x04000054;
// 0x04000056 unused

struct DISPCNT_Fields {
  U16 mode : 3;
  U16 gb : 1;
  U16 ps : 1;
  U16 hb : 1;
  U16 om : 1;
  U16 fb : 1;
  U16 bg0 : 1;
  U16 bg1 : 1;
  U16 bg2 : 1;
  U16 bg3 : 1;
  U16 obj : 1;
  U16 w0 : 1;
  U16 w1 : 1;
  U16 ow : 1;
};

union DISPCNT_t {
  U16 value;
  DISPCNT_Fields fields;

  DISPCNT_t(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

} // namespace Emulator