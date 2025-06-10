#pragma once

#include "datatypes.h"

namespace Emulator::Thumb {

namespace ThumbMask {
constexpr U16 ADC = 0xFFC0;
constexpr U16 ADD1 = 0xFE00;
constexpr U16 ADD2 = 0xF800;
constexpr U16 ADD3 = 0xFE00;
constexpr U16 ADD4 = 0xFF00;
constexpr U16 ADD5 = 0xF800;
constexpr U16 ADD6 = 0xF800;
constexpr U16 ADD7 = 0xFF80;
constexpr U16 AND = 0xFFC0;
constexpr U16 ASR1 = 0xF800;
constexpr U16 ASR2 = 0xFFC0;
constexpr U16 B1 = 0xF000;
constexpr U16 B2 = 0xF800;
constexpr U16 BIC = 0xFFC0;
constexpr U16 BKPT = 0xFF00;
constexpr U16 BL = 0xF000;
constexpr U16 BX = 0xFF80;
constexpr U16 CMN = 0xFFC0;
constexpr U16 CMP1 = 0xF800;
constexpr U16 CMP2 = 0xFFC0;
constexpr U16 CMP3 = 0xFF00;
constexpr U16 EOR = 0xFFC0;
constexpr U16 LDMIA = 0xF800;
constexpr U16 LDR1 = 0xF800;
constexpr U16 LDR2 = 0xFE00;
constexpr U16 LDR3 = 0xF800;
constexpr U16 LDR4 = 0xF800;
constexpr U16 LDRB1 = 0xF800;
constexpr U16 LDRB2 = 0xFE00;
constexpr U16 LDRH1 = 0xF800;
constexpr U16 LDRH2 = 0xFE00;
constexpr U16 LDRSB = 0xFE00;
constexpr U16 LDRSH = 0xFE00;
constexpr U16 LSL1 = 0xF800;
constexpr U16 LSL2 = 0xFFC0;
constexpr U16 LSR1 = 0xF800;
constexpr U16 LSR2 = 0xFFC0;
constexpr U16 MOV1 = 0xF800;
constexpr U16 MOV2 = 0xFFC0;
constexpr U16 MOV3 = 0xFF00;
constexpr U16 MUL = 0xFFC0;
constexpr U16 MVN = 0xFFC0;
constexpr U16 NEG = 0xFFC0;
constexpr U16 ORR = 0xFFC0;
constexpr U16 POP = 0xFE00;
constexpr U16 PUSH = 0xFE00;
constexpr U16 ROR = 0xFFC0;
constexpr U16 SBC = 0xFFC0;
constexpr U16 STMIA = 0xF800;
constexpr U16 STR1 = 0xF800;
constexpr U16 STR2 = 0xFE00;
constexpr U16 STR3 = 0xF800;
constexpr U16 STRB1 = 0xF800;
constexpr U16 STRB2 = 0xFE00;
constexpr U16 STRH1 = 0xF800;
constexpr U16 STRH2 = 0xFE00;
constexpr U16 SUB1 = 0xFE00;
constexpr U16 SUB2 = 0xF800;
constexpr U16 SUB3 = 0xFE00;
constexpr U16 SUB4 = 0xFF80;
constexpr U16 SWI = 0xFF00;
constexpr U16 TST = 0xFFC0;
} // namespace ThumbMask

namespace ThumbEncoding {
constexpr U16 ADC = 0x4180;
constexpr U16 ADD1 = 0x1C00;
constexpr U16 ADD2 = 0x3000;
constexpr U16 ADD3 = 0x1800;
constexpr U16 ADD4 = 0x4400;
constexpr U16 ADD5 = 0xA000;
constexpr U16 ADD6 = 0xA800;
constexpr U16 ADD7 = 0xB000;
constexpr U16 AND = 0x4000;
constexpr U16 ASR1 = 0x1000;
constexpr U16 ASR2 = 0x4100;
constexpr U16 B1 = 0xD000;
constexpr U16 B2 = 0xE000;
constexpr U16 BIC = 0x4380;
constexpr U16 BKPT = 0xBE00;
constexpr U16 BL = 0xF000;
constexpr U16 BX = 0x4700;
constexpr U16 CMN = 0x42C0;
constexpr U16 CMP1 = 0x2800;
constexpr U16 CMP2 = 0x4280;
constexpr U16 CMP3 = 0x4500;
constexpr U16 EOR = 0x4040;
constexpr U16 LDMIA = 0xC800;
constexpr U16 LDR1 = 0x6800;
constexpr U16 LDR2 = 0x5800;
constexpr U16 LDR3 = 0x4800;
constexpr U16 LDR4 = 0x9800;
constexpr U16 LDRB1 = 0x7800;
constexpr U16 LDRB2 = 0x5C00;
constexpr U16 LDRH1 = 0x8800;
constexpr U16 LDRH2 = 0x5A00;
constexpr U16 LDRSB = 0x5600;
constexpr U16 LDRSH = 0x5D00;
constexpr U16 LSL1 = 0x0000;
constexpr U16 LSL2 = 0x4080;
constexpr U16 LSR1 = 0x0800;
constexpr U16 LSR2 = 0x40C0;
constexpr U16 MOV1 = 0x2000;
constexpr U16 MOV2 = 0x1C00;
constexpr U16 MOV3 = 0x4600;
constexpr U16 MUL = 0x4340;
constexpr U16 MVN = 0x43C0;
constexpr U16 NEG = 0x4240;
constexpr U16 ORR = 0x4300;
constexpr U16 POP = 0xBC00;
constexpr U16 PUSH = 0xB400;
constexpr U16 ROR = 0x41C0;
constexpr U16 SBC = 0x4180;
constexpr U16 STMIA = 0xC000;
constexpr U16 STR1 = 0x6000;
constexpr U16 STR2 = 0x5000;
constexpr U16 STR3 = 0x9000;
constexpr U16 STRB1 = 0x7000;
constexpr U16 STRB2 = 0x5400;
constexpr U16 STRH1 = 0x8000;
constexpr U16 STRH2 = 0x5200;
constexpr U16 SUB1 = 0x1E00;
constexpr U16 SUB2 = 0x3800;
constexpr U16 SUB3 = 0x1A00;
constexpr U16 SUB4 = 0xB080;
constexpr U16 SWI = 0xDF00;
constexpr U16 TST = 0x4200;
} // namespace ThumbEncoding

constexpr U16 ThumbMaskList[] = {
    ThumbMask::ADC,   ThumbMask::ADD1,  ThumbMask::ADD2,  ThumbMask::ADD3,
    ThumbMask::ADD4,  ThumbMask::ADD5,  ThumbMask::ADD6,  ThumbMask::ADD7,
    ThumbMask::AND,   ThumbMask::ASR1,  ThumbMask::ASR2,  ThumbMask::B1,
    ThumbMask::B2,    ThumbMask::BIC,   ThumbMask::BKPT,  ThumbMask::BL,
    ThumbMask::BX,    ThumbMask::CMN,   ThumbMask::CMP1,  ThumbMask::CMP2,
    ThumbMask::CMP3,  ThumbMask::EOR,   ThumbMask::LDMIA, ThumbMask::LDR1,
    ThumbMask::LDR2,  ThumbMask::LDR3,  ThumbMask::LDR4,  ThumbMask::LDRB1,
    ThumbMask::LDRB2, ThumbMask::LDRH1, ThumbMask::LDRH2, ThumbMask::LDRSB,
    ThumbMask::LDRSH, ThumbMask::LSL1,  ThumbMask::LSL2,  ThumbMask::LSR1,
    ThumbMask::LSR2,  ThumbMask::MOV1,  ThumbMask::MOV2,  ThumbMask::MOV3,
    ThumbMask::MUL,   ThumbMask::MVN,   ThumbMask::NEG,   ThumbMask::ORR,
    ThumbMask::POP,   ThumbMask::PUSH,  ThumbMask::ROR,   ThumbMask::SBC,
    ThumbMask::STMIA, ThumbMask::STR1,  ThumbMask::STR2,  ThumbMask::STR3,
    ThumbMask::STRB1, ThumbMask::STRB2, ThumbMask::STRH1, ThumbMask::STRH2,
    ThumbMask::SUB1,  ThumbMask::SUB2,  ThumbMask::SUB3,  ThumbMask::SUB4,
    ThumbMask::SWI,   ThumbMask::TST,
};

constexpr U16 ThumbEncodingList[] = {
    ThumbEncoding::ADC,   ThumbEncoding::ADD1,  ThumbEncoding::ADD2,
    ThumbEncoding::ADD3,  ThumbEncoding::ADD4,  ThumbEncoding::ADD5,
    ThumbEncoding::ADD6,  ThumbEncoding::ADD7,  ThumbEncoding::AND,
    ThumbEncoding::ASR1,  ThumbEncoding::ASR2,  ThumbEncoding::B1,
    ThumbEncoding::B2,    ThumbEncoding::BIC,   ThumbEncoding::BKPT,
    ThumbEncoding::BL,    ThumbEncoding::BX,    ThumbEncoding::CMN,
    ThumbEncoding::CMP1,  ThumbEncoding::CMP2,  ThumbEncoding::CMP3,
    ThumbEncoding::EOR,   ThumbEncoding::LDMIA, ThumbEncoding::LDR1,
    ThumbEncoding::LDR2,  ThumbEncoding::LDR3,  ThumbEncoding::LDR4,
    ThumbEncoding::LDRB1, ThumbEncoding::LDRB2, ThumbEncoding::LDRH1,
    ThumbEncoding::LDRH2, ThumbEncoding::LDRSB, ThumbEncoding::LDRSH,
    ThumbEncoding::LSL1,  ThumbEncoding::LSL2,  ThumbEncoding::LSR1,
    ThumbEncoding::LSR2,  ThumbEncoding::MOV1,  ThumbEncoding::MOV2,
    ThumbEncoding::MOV3,  ThumbEncoding::MUL,   ThumbEncoding::MVN,
    ThumbEncoding::NEG,   ThumbEncoding::ORR,   ThumbEncoding::POP,
    ThumbEncoding::PUSH,  ThumbEncoding::ROR,   ThumbEncoding::SBC,
    ThumbEncoding::STMIA, ThumbEncoding::STR1,  ThumbEncoding::STR2,
    ThumbEncoding::STR3,  ThumbEncoding::STRB1, ThumbEncoding::STRB2,
    ThumbEncoding::STRH1, ThumbEncoding::STRH2, ThumbEncoding::SUB1,
    ThumbEncoding::SUB2,  ThumbEncoding::SUB3,  ThumbEncoding::SUB4,
    ThumbEncoding::SWI,   ThumbEncoding::TST,
};

enum ThumbOpcode {
  ADC,
  ADD1,
  ADD2,
  ADD3,
  ADD4,
  ADD5,
  ADD6,
  ADD7,
  AND,
  ASR1,
  ASR2,
  B1,
  B2,
  BIC,
  BKPT,
  BL,
  BX,
  CMN,
  CMP1,
  CMP2,
  CMP3,
  EOR,
  LDMIA,
  LDR1,
  LDR2,
  LDR3,
  LDR4,
  LDRB1,
  LDRB2,
  LDRH1,
  LDRH2,
  LDRSB,
  LDRSH,
  LSL1,
  LSL2,
  LSR1,
  LSR2,
  MOV1,
  MOV2,
  MOV3,
  MUL,
  MVN,
  NEG,
  ORR,
  POP,
  PUSH,
  ROR,
  SBC,
  STMIA,
  STR1,
  STR2,
  STR3,
  STRB1,
  STRB2,
  STRH1,
  STRH2,
  SUB1,
  SUB2,
  SUB3,
  SUB4,
  SWI,
  TST,
  NUM_OPCODES, // Must be last enum
};

const char *ToString(const ThumbOpcode opcode) {
  switch (opcode) {
  case ADC:
    return "ADC";
  case ADD1:
    return "ADD1";
  case ADD2:
    return "ADD2";
  case ADD3:
    return "ADD3";
  case ADD4:
    return "ADD4";
  case ADD5:
    return "ADD5";
  case ADD6:
    return "ADD6";
  case ADD7:
    return "ADD7";
  case AND:
    return "AND";
  case ASR1:
    return "ASR1";
  case ASR2:
    return "ASR2";
  case B1:
    return "B1";
  case B2:
    return "B2";
  case BIC:
    return "BIC";
  case BKPT:
    return "BKPT";
  case BL:
    return "BL";
  case BX:
    return "BX";
  case CMN:
    return "CMN";
  case CMP1:
    return "CMP1";
  case CMP2:
    return "CMP2";
  case CMP3:
    return "CMP3";
  case EOR:
    return "EOR";
  case LDMIA:
    return "LDMIA";
  case LDR1:
    return "LDR1";
  case LDR2:
    return "LDR2";
  case LDR3:
    return "LDR3";
  case LDR4:
    return "LDR4";
  case LDRB1:
    return "LDRB1";
  case LDRB2:
    return "LDRB2";
  case LDRH1:
    return "LDRH1";
  case LDRH2:
    return "LDRH2";
  case LDRSB:
    return "LDRSB";
  case LDRSH:
    return "LDRSH";
  case LSL1:
    return "LSL1";
  case LSL2:
    return "LSL2";
  case LSR1:
    return "LSR1";
  case LSR2:
    return "LSR2";
  case MOV1:
    return "MOV1";
  case MOV2:
    return "MOV2";
  case MOV3:
    return "MOV3";
  case MUL:
    return "MUL";
  case MVN:
    return "MVN";
  case NEG:
    return "NEG";
  case ORR:
    return "ORR";
  case POP:
    return "POP";
  case PUSH:
    return "PUSH";
  case ROR:
    return "ROR";
  case SBC:
    return "SBC";
  case STMIA:
    return "STMIA";
  case STR1:
    return "STR1";
  case STR2:
    return "STR2";
  case STR3:
    return "STR3";
  case STRB1:
    return "STRB1";
  case STRB2:
    return "STRB2";
  case STRH1:
    return "STRH1";
  case STRH2:
    return "STRH2";
  case SUB1:
    return "SUB1";
  case SUB2:
    return "SUB2";
  case SUB3:
    return "SUB3";
  case SUB4:
    return "SUB4";
  case SWI:
    return "SWI";
  case TST:
    return "TST";
  case NUM_OPCODES:
    return "NUM_OPCODES";
  default:
    return "UNKNOWN";
  }
}

static_assert(sizeof(ThumbEncodingList) / sizeof(ThumbEncodingList[0]) ==
              NUM_OPCODES);
static_assert(sizeof(ThumbMaskList) / sizeof(ThumbMaskList[0]) == NUM_OPCODES);

inline ThumbOpcode GetThumbOpcode(U16 instr) {
  for (U32 i = 0; i < U32(NUM_OPCODES); ++i) {
    if ((instr & ThumbMaskList[i]) == ThumbEncodingList[i]) {
      return ThumbOpcode(i);
    }
  }
  ABORT("Could not find thumbcode for 0x%04X", instr);
}

} // namespace Emulator::Thumb