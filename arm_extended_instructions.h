#pragma once

#include "arm_instructions.h"
#include "datatypes.h"
#include "logging.h"

namespace Emulator::Arm {

namespace ExtendedInstrMask {
constexpr U32 UNDEFINED1 = 0b0000'1110'0000'0000'0000'0000'0001'0000;
constexpr U32 UNDEFINED2 = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 MUL = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 MULS = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 MLA = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 MLAS = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 UMULL = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 UMULLS = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 UMLAL = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 UMLALS = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 SMULL = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 SMULLS = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 SMLAL = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 SMLALS = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 MRS = 0b0000'1111'1011'1000'0000'0000'1111'0000;
constexpr U32 MSR = 0b0000'1111'1011'1000'0000'0000'1111'0000;
constexpr U32 BX = 0b0000'1111'1111'1000'0000'0000'1111'0000;
constexpr U32 MSR_IMM = 0b0000'1111'1011'1000'0000'0000'0000'0000;
constexpr U32 SWP = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 SWPB = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 STRH = 0b0000'1110'0001'0000'0000'0000'1111'0000;
constexpr U32 LDRH = 0b0000'1110'0001'0000'0000'0000'1111'0000;
constexpr U32 LDRSB = 0b0000'1110'0001'0000'0000'0000'1111'0000;
constexpr U32 LDRSH = 0b0000'1110'0001'0000'0000'0000'1111'0000;
} // namespace ExtendedInstrMask

namespace ExtendedInstrEncoding {
constexpr U32 UNDEFINED1 = 0b0000'0110'1010'0000'0000'0000'0001'0000;
constexpr U32 UNDEFINED2 = 0b0000'0111'1111'0000'0000'0000'1111'0000;
constexpr U32 MUL = 0b0000'0000'0000'0000'0000'0000'1001'0000;
constexpr U32 MULS = 0b0000'0000'0001'0000'0000'0000'1001'0000;
constexpr U32 MLA = 0b0000'0000'0010'0000'0000'0000'1001'0000;
constexpr U32 MLAS = 0b0000'0000'0011'0000'0000'0000'1001'0000;
constexpr U32 UMULL = 0b0000'0000'1000'0000'0000'0000'1001'0000;
constexpr U32 UMULLS = 0b0000'0000'1001'0000'0000'0000'1001'0000;
constexpr U32 UMLAL = 0b0000'0000'1010'0000'0000'0000'1001'0000;
constexpr U32 UMLALS = 0b0000'0000'1011'0000'0000'0000'1001'0000;
constexpr U32 SMULL = 0b0000'0000'1100'0000'0000'0000'1001'0000;
constexpr U32 SMULLS = 0b0000'0000'1101'0000'0000'0000'1001'0000;
constexpr U32 SMLAL = 0b0000'0000'1110'0000'0000'0000'1001'0000;
constexpr U32 SMLALS = 0b0000'0000'1111'0000'0000'0000'1001'0000;
constexpr U32 MRS = 0b0000'0001'0000'0000'0000'0000'0000'0000;
constexpr U32 MSR = 0b0000'0001'0010'0000'0000'0000'0000'0000;
constexpr U32 BX = 0b0000'0001'0010'0000'0000'0000'0001'0000;
constexpr U32 MSR_IMM = 0b0000'0010'0010'0000'0000'0000'0000'0000;
constexpr U32 SWP = 0b0000'0001'0000'0000'0000'0000'1001'0000;
constexpr U32 SWPB = 0b0000'0001'0100'0000'0000'0000'1001'0000;
constexpr U32 STRH = 0b0000'0000'0000'0000'0000'0000'1011'0000;
constexpr U32 LDRH = 0b0000'0000'0001'0000'0000'0000'1011'0000;
constexpr U32 LDRSB = 0b0000'0000'0001'0000'0000'0000'1101'0000;
constexpr U32 LDRSH = 0b0000'0000'0001'0000'0000'0000'1111'0000;
} // namespace ExtendedInstrEncoding

enum class ExtendedInstr {
  UNDEFINED1,
  UNDEFINED2,
  MUL,
  MULS,
  MLA,
  MLAS,
  UMULL,
  UMULLS,
  UMLAL,
  UMLALS,
  SMULL,
  SMULLS,
  SMLAL,
  SMLALS,
  MRS,
  MSR,
  BX,
  MSR_IMM,
  SWP,
  SWPB,
  STRH,
  LDRH,
  LDRSB,
  LDRSH,
  NUM_OPCODES
};

inline const char *ToString(ExtendedInstr instr) {
  switch (instr) {
  case ExtendedInstr::UNDEFINED1:
    return "UNDEFINED1";
  case ExtendedInstr::UNDEFINED2:
    return "UNDEFINED2";
  case ExtendedInstr::MUL:
    return "MUL";
  case ExtendedInstr::MULS:
    return "MULS";
  case ExtendedInstr::MLA:
    return "MLA";
  case ExtendedInstr::MLAS:
    return "MLAS";
  case ExtendedInstr::UMULL:
    return "UMULL";
  case ExtendedInstr::UMULLS:
    return "UMULLS";
  case ExtendedInstr::UMLAL:
    return "UMLAL";
  case ExtendedInstr::UMLALS:
    return "UMLALS";
  case ExtendedInstr::SMULL:
    return "SMULL";
  case ExtendedInstr::SMULLS:
    return "SMULLS";
  case ExtendedInstr::SMLAL:
    return "SMLAL";
  case ExtendedInstr::SMLALS:
    return "SMLALS";
  case ExtendedInstr::MRS:
    return "MRS";
  case ExtendedInstr::MSR:
    return "MSR";
  case ExtendedInstr::BX:
    return "BX";
  case ExtendedInstr::MSR_IMM:
    return "MSR_IMM";
  case ExtendedInstr::SWP:
    return "SWP";
  case ExtendedInstr::SWPB:
    return "SWPB";
  case ExtendedInstr::STRH:
    return "STRH";
  case ExtendedInstr::LDRH:
    return "LDRH";
  case ExtendedInstr::LDRSB:
    return "LDRSB";
  case ExtendedInstr::LDRSH:
    return "LDRSH";
  case ExtendedInstr::NUM_OPCODES:
    return "NUM_OPCODES";
  default:
    return "UNKNOWN";
  }
}

constexpr U32 ExtendedInstrMasks[] = {
    ExtendedInstrMask::UNDEFINED1, ExtendedInstrMask::UNDEFINED2,
    ExtendedInstrMask::MUL,        ExtendedInstrMask::MULS,
    ExtendedInstrMask::MLA,        ExtendedInstrMask::MLAS,
    ExtendedInstrMask::UMULL,      ExtendedInstrMask::UMULLS,
    ExtendedInstrMask::UMLAL,      ExtendedInstrMask::UMLALS,
    ExtendedInstrMask::SMULL,      ExtendedInstrMask::SMULLS,
    ExtendedInstrMask::SMLAL,      ExtendedInstrMask::SMLALS,
    ExtendedInstrMask::MRS,        ExtendedInstrMask::MSR,
    ExtendedInstrMask::BX,         ExtendedInstrMask::MSR_IMM,
    ExtendedInstrMask::SWP,        ExtendedInstrMask::SWPB,
    ExtendedInstrMask::STRH,       ExtendedInstrMask::LDRH,
    ExtendedInstrMask::LDRSB,      ExtendedInstrMask::LDRSH};

constexpr U32 ExtendedInstrEncodings[] = {
    ExtendedInstrEncoding::UNDEFINED1, ExtendedInstrEncoding::UNDEFINED2,
    ExtendedInstrEncoding::MUL,        ExtendedInstrEncoding::MULS,
    ExtendedInstrEncoding::MLA,        ExtendedInstrEncoding::MLAS,
    ExtendedInstrEncoding::UMULL,      ExtendedInstrEncoding::UMULLS,
    ExtendedInstrEncoding::UMLAL,      ExtendedInstrEncoding::UMLALS,
    ExtendedInstrEncoding::SMULL,      ExtendedInstrEncoding::SMULLS,
    ExtendedInstrEncoding::SMLAL,      ExtendedInstrEncoding::SMLALS,
    ExtendedInstrEncoding::MRS,        ExtendedInstrEncoding::MSR,
    ExtendedInstrEncoding::BX,         ExtendedInstrEncoding::MSR_IMM,
    ExtendedInstrEncoding::SWP,        ExtendedInstrEncoding::SWPB,
    ExtendedInstrEncoding::STRH,       ExtendedInstrEncoding::LDRH,
    ExtendedInstrEncoding::LDRSB,      ExtendedInstrEncoding::LDRSH};

constexpr Instr ExtendedInstrToArmInstr[] = {
    Instr::BAD_CODE, Instr::BAD_CODE, Instr::MUL,   Instr::MUL,  Instr::MLA,
    Instr::MLA,      Instr::MUL,      Instr::MUL,   Instr::MUL,  Instr::MUL,
    Instr::MUL,      Instr::MUL,      Instr::MUL,   Instr::MUL,  Instr::MRS,
    Instr::MSR,      Instr::BX,       Instr::MSR,   Instr::SWP,  Instr::SWP,
    Instr::STR,      Instr::LDRH,     Instr::LDRSB, Instr::LDRSH};

static_assert(sizeof(ExtendedInstrEncodings) /
                  sizeof(ExtendedInstrEncodings[0]) ==
              U32(ExtendedInstr::NUM_OPCODES));
static_assert(sizeof(ExtendedInstrMasks) / sizeof(ExtendedInstrMasks[0]) ==
              U32(ExtendedInstr::NUM_OPCODES));

inline ExtendedInstr GetExtendedArmOpcode(U32 instr) {
  for (U32 i = 0; i < U32(ExtendedInstr::NUM_OPCODES); ++i) {
    if ((instr & ExtendedInstrMasks[i]) == ExtendedInstrEncodings[i]) {
      return ExtendedInstr(i);
    }
  }
  return ExtendedInstr::UNDEFINED1;
}

} // namespace Emulator::Arm