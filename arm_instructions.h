#pragma once

#include "datatypes.h"

namespace Emulator::Arm {

struct SingleDataTransferInstrFields {
  U32 offset : 12;
  U32 rd : 4;
  U32 rn : 4;
  U32 l : 1;
  U32 w : 1;
  U32 b : 1;
  U32 u : 1;
  U32 p : 1;
  U32 i : 1;
  U32 : 2;
  U32 cond : 4;
};

union SingleDataTransferInstr {
  U32 value;
  SingleDataTransferInstrFields fields;

  SingleDataTransferInstr(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct MSRImmInstrFields {
  U32 imm : 8;
  U32 rotate_imm : 4;
  U32 sbo : 4;
  U32 field_mask : 4;
  U32 : 2;
  U32 r : 1;
  U32 : 5;
  U32 cond : 4;
};

union MSRImmInstr {
  U32 value;
  MSRImmInstrFields fields;

  MSRImmInstr(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct MRSRegInstrFields {
  U32 sbz : 12;
  U32 rd : 4;
  U32 sbo : 4;
  U32 : 2;
  U32 r : 1;
  U32 : 5;
  U32 cond : 4;
};

union MRSRegInstr {
  U32 value;
  MRSRegInstrFields fields;

  MRSRegInstr(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct MSRRegInstrFields {
  U32 rm : 4;
  U32 : 4;
  U32 sbz : 4;
  U32 sbo : 4;
  U32 field_mask : 4;
  U32 : 2;
  U32 r : 1;
  U32 : 5;
  U32 cond : 4;
};

union MSRRegInstr {
  U32 value;
  MSRRegInstrFields fields;

  MSRRegInstr(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct ShifterOperandResult {
  U32 shifter_operand;
  U32 shifter_carry_out;
};

struct LoadAndStoreHalfWordImmFields {
  U32 immedl : 4;
  U32 : 1;
  U32 h : 1;
  U32 s : 1;
  U32 : 1;
  U32 immedh : 4;
};

union LoadAndStoreHalfWordImm {
  U32 value;
  LoadAndStoreHalfWordImmFields fields;

  LoadAndStoreHalfWordImm(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct LoadAndStoreHalfWordRegFields {
  U32 rm : 4;
  U32 : 1;
  U32 h : 1;
  U32 s : 1;
  U32 : 1;
  U32 : 4;
};

union LoadAndStoreHalfWordReg {
  U32 value;
  LoadAndStoreHalfWordRegFields fields;

  LoadAndStoreHalfWordReg(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct LoadAndStoreWordOrByteImmFields {
  U32 offset : 12;
};

union LoadAndStoreWordOrByteImm {
  U32 value;
  LoadAndStoreWordOrByteImmFields fields;

  LoadAndStoreWordOrByteImm(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct LoadAndStoreWordOrByteRegFields {
  U32 rm : 4;
  U32 : 1;
  U32 shift : 2;
  U32 shift_imm : 5;
};

union LoadAndStoreWordOrByteReg {
  U32 value;
  LoadAndStoreWordOrByteRegFields fields;

  LoadAndStoreWordOrByteReg(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct LoadAndStoreMultipleFields {
  U32 register_list : 16;
  U32 rn : 4;
  U32 l : 1;
  U32 w : 1;
  U32 s : 1;
  U32 u : 1;
  U32 p : 1;
  U32 : 3;
  U32 cond : 4;
};

union LoadAndStoreMultiple {
  U32 value;
  LoadAndStoreMultipleFields fields;

  LoadAndStoreMultiple(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct LoadAndStoreMultipleAddrResult {
  U32 start_addr;
  U32 end_addr;
};

struct DataProcessingInstrImmediateFields {
  U32 immed_8 : 8;
  U32 rotate_imm : 4;
};

union DataProcessingInstrImmediate {
  U32 value;
  DataProcessingInstrImmediateFields fields;

  DataProcessingInstrImmediate(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct DataProcessingInstrRegisterFields {
  U32 rm : 4;
};

union DataProcessingInstrRegister {
  U32 value;
  DataProcessingInstrRegisterFields fields;

  DataProcessingInstrRegister(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct DataProcessingInstrShiftByImmFields {
  U32 rm : 4;
  U32 : 3;
  U32 shift_imm : 5;
};

union DataProcessingInstrShiftByImm {
  U32 value;
  DataProcessingInstrShiftByImmFields fields;

  DataProcessingInstrShiftByImm(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct DataProcessingInstrShiftByRegisterFields {
  U32 rm : 4;
  U32 : 4;
  U32 rs : 4;
};

union DataProcessingInstrShiftByRegister {
  U32 value;
  DataProcessingInstrShiftByRegisterFields fields;

  DataProcessingInstrShiftByRegister(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct DataProcessingInstrFields {
  U32 operand_2 : 12;
  U32 rd : 4;
  U32 rn : 4;
  U32 s : 1;
  U32 opcode : 4;
  U32 i : 1;
  U32 : 2;
  U32 cond : 4;
};

union DataProcessingInstr {
  U32 value;
  DataProcessingInstrFields fields;

  DataProcessingInstr(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct BranchInstrFields {
  U32 offset : 24;
  U32 link_bit : 1;
  U32 : 3;
  U32 cond : 4;
};

union BranchInstr {
  U32 value;
  BranchInstrFields fields;

  BranchInstr(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct BranchAndExchangeInstrFields {
  U32 rm : 4;
  U32 : 24;
  U32 cond : 4;
};

union BranchAndExchangeInstr {
  U32 value;
  BranchAndExchangeInstrFields fields;

  BranchAndExchangeInstr(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

enum ConditionCode : U8 {
  EQ = 0b0000,
  NE = 0b0001,
  CS = 0b0010,
  CC = 0b0011,
  MI = 0b0100,
  PL = 0b0101,
  VS = 0b0110,
  VC = 0b0111,
  HI = 0b1000,
  LS = 0b1001,
  GE = 0b1010,
  LT = 0b1011,
  GT = 0b1100,
  LE = 0b1101,
  AL = 0b1110,
};

inline const char *toString(ConditionCode cc) {
  switch (cc) {
  case EQ:
    return "EQ";
  case NE:
    return "NE";
  case CS:
    return "CS";
  case CC:
    return "CC";
  case MI:
    return "MI";
  case PL:
    return "PL";
  case VS:
    return "VS";
  case VC:
    return "VC";
  case HI:
    return "HI";
  case LS:
    return "LS";
  case GE:
    return "GE";
  case LT:
    return "LT";
  case GT:
    return "GT";
  case LE:
    return "LE";
  case AL:
    return "AL";
  default:
    return "UKN";
  }
}

namespace InstrMask {
constexpr U32 ADC = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 ADD = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 AND = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 B = 0b0000'1111'0000'0000'0000'0000'0000'0000;
constexpr U32 BIC = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 BL = 0b0000'1111'0000'0000'0000'0000'0000'0000;
constexpr U32 BX = 0b0000'1111'1111'1111'1111'1111'1111'0000;
constexpr U32 CDP = 0b0000'1111'0000'0000'0000'0000'0001'0000;
constexpr U32 CMN = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 CMP = 0b0000'1101'1111'0000'0000'0000'0000'0000;
constexpr U32 EOR = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 LDC = 0b0000'1110'0001'0000'0000'0000'0000'0000;
constexpr U32 LDM = 0b0000'1110'0001'0000'0000'0000'0000'0000;
constexpr U32 LDR = 0b0000'1100'0101'0000'0000'0000'0000'0000;
constexpr U32 LDRB = 0b0000'1100'0101'0000'0000'0000'0000'0000;
constexpr U32 LDRBT = 0b0000'1100'0111'0000'0000'0000'0000'0000;
constexpr U32 LDRH = 0b0000'1110'0001'0000'0000'0000'1111'0000;
constexpr U32 LDRSB = 0b0000'1110'0001'0000'0000'0000'1111'0000;
constexpr U32 LDRSH = 0b0000'1110'0001'0000'0000'0000'1111'0000;
constexpr U32 LDRT = 0b0000'1101'0111'1000'0000'0000'0000'0000;
constexpr U32 MCR = 0b0000'1111'0001'0000'0000'0000'0001'0000;
constexpr U32 MLA = 0b0000'1111'0110'0000'0000'0000'1111'0000;
constexpr U32 MOV = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 MRC = 0b0000'1111'0001'0000'0000'0000'0001'0000;
constexpr U32 MRS = 0b0000'1111'1011'1111'0000'1111'1111'1111;
constexpr U32 MSR = 0b0000'1101'1011'1111'1111'0000'0000'0000;
constexpr U32 MUL = 0b0000'1111'0110'0000'0000'0000'1111'0000;
constexpr U32 MVN = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 ORR = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 RSB = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 RSC = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 SBC = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 SMLAL = 0b0000'1111'1110'0000'0000'0000'1111'0000;
constexpr U32 SMULL = 0b0000'1111'1110'0000'0000'0000'1111'0000;
constexpr U32 STC = 0b0000'1110'0001'0000'0000'0000'0000'0000;
constexpr U32 STM = 0b0000'1110'0001'0000'0000'0000'0000'0000;
constexpr U32 STR = 0b0000'1100'0001'0000'0000'0000'0000'0000;
constexpr U32 STRB = 0b0000'1100'0101'0000'0000'0000'0000'0000;
constexpr U32 STRBT = 0b0000'1101'0111'0000'0000'0000'0000'0000;
constexpr U32 STRH = 0b0000'1110'0001'0000'0000'0000'1111'0000;
constexpr U32 STRT = 0b0000'1101'0111'0000'0000'0000'0000'0000;
constexpr U32 SUB = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 SWI = 0b0000'1111'0000'0000'0000'0000'0000'0000;
constexpr U32 SWP = 0b0000'1111'1011'0000'0000'1111'1111'0000;
constexpr U32 SWPB = 0b0000'1111'1111'0000'0000'0000'1111'0000;
constexpr U32 TEQ = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 TST = 0b0000'1101'1110'0000'0000'0000'0000'0000;
constexpr U32 UMLAL = 0b0000'1111'1110'0000'0000'0000'1111'0000;
constexpr U32 UMULL = 0b0000'1111'1110'0000'0000'0000'1111'0000;
} // namespace InstrMask

namespace InstrEncoding {
constexpr U32 ADC = 0b0000'0000'1010'0000'0000'0000'0000'0000;
constexpr U32 ADD = 0b0000'0000'1000'0000'0000'0000'0000'0000;
constexpr U32 AND = 0b0000'0000'0000'0000'0000'0000'0000'0000;
constexpr U32 B = 0b0000'1010'0000'0000'0000'0000'0000'0000;
constexpr U32 BIC = 0b0000'0001'1100'0000'0000'0000'0000'0000;
constexpr U32 BL = 0b0000'1011'0000'0000'0000'0000'0000'0000;
constexpr U32 BX = 0b0000'0001'0010'1111'1111'1111'0001'0000;
constexpr U32 CDP = 0b0000'1110'0000'0000'0000'0000'0000'0000;
constexpr U32 CMN = 0b0000'0001'0111'0000'0000'0000'0000'0000;
constexpr U32 CMP = 0b0000'0001'0101'0000'0000'0000'0000'0000;
constexpr U32 EOR = 0b0000'0000'0010'0000'0000'0000'0000'0000;
constexpr U32 LDC = 0b0000'1100'0001'0000'0000'0000'0000'0000;
constexpr U32 LDM = 0b0000'1000'0001'0000'0000'0000'0000'0000;
constexpr U32 LDR = 0b0000'0100'0001'0000'0000'0000'0000'0000;
constexpr U32 LDRB = 0b0000'0100'0101'0000'0000'0000'0000'0000;
constexpr U32 LDRBT = 0b0000'0100'0111'0000'0000'0000'0000'0000;
constexpr U32 LDRH = 0b0000'0000'0001'0000'0000'0000'1011'0000;
constexpr U32 LDRSB = 0b0000'0000'0001'0000'0000'0000'1101'0000;
constexpr U32 LDRSH = 0b0000'0000'0001'0000'0000'0000'1111'0000;
constexpr U32 LDRT = 0b0000'0100'0011'0000'0000'0000'0000'0000;
constexpr U32 MCR = 0b0000'1110'0000'0000'0000'0000'0001'0000;
constexpr U32 MLA = 0b0000'0000'0010'0000'0000'0000'1001'0000;
constexpr U32 MOV = 0b0000'0001'1010'0000'0000'0000'0000'0000;
constexpr U32 MRC = 0b0000'1110'0001'0000'0000'0000'0001'0000;
constexpr U32 MRS = 0b0000'0001'0000'1111'0000'0000'0000'0000;
constexpr U32 MSR = 0b0000'0001'0010'1001'1111'0000'0000'0000;
constexpr U32 MUL = 0b0000'0000'0000'0000'0000'0000'1001'0000;
constexpr U32 MVN = 0b0000'0001'1110'0000'0000'0000'0000'0000;
constexpr U32 ORR = 0b0000'0001'1000'0000'0000'0000'0000'0000;
constexpr U32 RSB = 0b0000'0000'0110'0000'0000'0000'0000'0000;
constexpr U32 RSC = 0b0000'0000'1110'0000'0000'0000'0000'0000;
constexpr U32 SBC = 0b0000'0000'1100'0000'0000'0000'0000'0000;
constexpr U32 SMLAL = 0b0000'0000'1110'0000'0000'0000'1001'0000;
constexpr U32 SMULL = 0b0000'0000'1100'0000'0000'0000'1001'0000;
constexpr U32 STC = 0b0000'1100'0000'0000'0000'0000'0000'0000;
constexpr U32 STM = 0b0000'1000'0000'0000'0000'0000'0000'0000;
constexpr U32 STR = 0b0000'0100'0000'0000'0000'0000'0000'0000;
constexpr U32 STRB = 0b0000'0100'0100'0000'0000'0000'0000'0000;
constexpr U32 STRBT = 0b0000'0100'0110'0000'0000'0000'0000'0000;
constexpr U32 STRH = 0b0000'0000'0000'0000'0000'0000'1011'0000;
constexpr U32 STRT = 0b0000'0100'0010'0000'0000'0000'0000'0000;
constexpr U32 SUB = 0b0000'0000'0100'0000'0000'0000'0000'0000;
constexpr U32 SWI = 0b0000'1111'0000'0000'0000'0000'0000'0000;
constexpr U32 SWP = 0b0000'0001'0000'0000'0000'0000'1001'0000;
constexpr U32 SWPB = 0b0000'0001'0100'0000'0000'0000'1001'0000;
constexpr U32 TEQ = 0b0000'0001'0010'0000'0000'0000'0000'0000;
constexpr U32 TST = 0b0000'0001'0000'0000'0000'0000'0000'0000;
constexpr U32 UMLAL = 0b0000'0000'1010'0000'0000'0000'1001'0000;
constexpr U32 UMULL = 0b0000'0000'1000'0000'0000'0000'1001'0000;
} // namespace InstrEncoding

enum class Instr {
  ADC,
  ADD,
  AND,
  B,
  BIC,
  BL,
  BX,
  CDP,
  CMN,
  CMP,
  EOR,
  LDC,
  LDM,
  LDR,
  LDRB,
  LDRBT,
  LDRH,
  LDRSB,
  LDRSH,
  LDRT,
  MCR,
  MLA,
  MOV,
  MRC,
  MRS,
  MSR,
  MUL,
  MVN,
  ORR,
  RSB,
  RSC,
  SBC,
  SMLAL,
  SMULL,
  STC,
  STM,
  STR,
  STRB,
  STRBT,
  STRH,
  STRT,
  SUB,
  SWI,
  SWP,
  SWPB,
  TEQ,
  TST,
  UMLAL,
  UMULL,
  NUM_OPCODES, // Must be last enum
  BAD_CODE,
};

inline const char *ToString(Instr instr) {
  switch (instr) {
  case Instr::ADC:
    return "ADC";
  case Instr::ADD:
    return "ADD";
  case Instr::AND:
    return "AND";
  case Instr::B:
    return "B";
  case Instr::BIC:
    return "BIC";
  case Instr::BL:
    return "BL";
  case Instr::BX:
    return "BX";
  case Instr::CDP:
    return "CDP";
  case Instr::CMN:
    return "CMN";
  case Instr::CMP:
    return "CMP";
  case Instr::EOR:
    return "EOR";
  case Instr::LDC:
    return "LDC";
  case Instr::LDM:
    return "LDM";
  case Instr::LDR:
    return "LDR";
  case Instr::LDRB:
    return "LDRB";
  case Instr::LDRBT:
    return "LDRBT";
  case Instr::LDRH:
    return "LDRH";
  case Instr::LDRSB:
    return "LDRSB";
  case Instr::LDRSH:
    return "LDRSH";
  case Instr::LDRT:
    return "LDRT";
  case Instr::MCR:
    return "MCR";
  case Instr::MLA:
    return "MLA";
  case Instr::MOV:
    return "MOV";
  case Instr::MRC:
    return "MRC";
  case Instr::MRS:
    return "MRS";
  case Instr::MSR:
    return "MSR";
  case Instr::MUL:
    return "MUL";
  case Instr::MVN:
    return "MVN";
  case Instr::ORR:
    return "ORR";
  case Instr::RSB:
    return "RSB";
  case Instr::RSC:
    return "RSC";
  case Instr::SBC:
    return "SBC";
  case Instr::SMLAL:
    return "SMLAL";
  case Instr::SMULL:
    return "SMULL";
  case Instr::STC:
    return "STC";
  case Instr::STM:
    return "STM";
  case Instr::STR:
    return "STR";
  case Instr::STRB:
    return "STRB";
  case Instr::STRBT:
    return "STRBT";
  case Instr::STRH:
    return "STRH";
  case Instr::STRT:
    return "STRT";
  case Instr::SUB:
    return "SUB";
  case Instr::SWI:
    return "SWI";
  case Instr::SWP:
    return "SWP";
  case Instr::SWPB:
    return "SWPB";
  case Instr::TEQ:
    return "TEQ";
  case Instr::TST:
    return "TST";
  case Instr::UMLAL:
    return "UMLAL";
  case Instr::UMULL:
    return "UMULL";
  case Instr::NUM_OPCODES:
    return "NUM_OPCODES";
  case Instr::BAD_CODE:
    return "BAD_CODE";
  }
  return "UNKNOWN";
}

constexpr U32 InstrMasks[] = {
    InstrMask::ADC,   InstrMask::ADD,   InstrMask::AND,   InstrMask::B,
    InstrMask::BIC,   InstrMask::BL,    InstrMask::BX,    InstrMask::CDP,
    InstrMask::CMN,   InstrMask::CMP,   InstrMask::EOR,   InstrMask::LDC,
    InstrMask::LDM,   InstrMask::LDR,   InstrMask::LDRB,  InstrMask::LDRBT,
    InstrMask::LDRH,  InstrMask::LDRSB, InstrMask::LDRSH, InstrMask::LDRT,
    InstrMask::MCR,   InstrMask::MLA,   InstrMask::MOV,   InstrMask::MRC,
    InstrMask::MRS,   InstrMask::MSR,   InstrMask::MUL,   InstrMask::MVN,
    InstrMask::ORR,   InstrMask::RSB,   InstrMask::RSC,   InstrMask::SBC,
    InstrMask::SMLAL, InstrMask::SMULL, InstrMask::STC,   InstrMask::STM,
    InstrMask::STR,   InstrMask::STRB,  InstrMask::STRBT, InstrMask::STRH,
    InstrMask::STRT,  InstrMask::SUB,   InstrMask::SWI,   InstrMask::SWP,
    InstrMask::SWPB,  InstrMask::TEQ,   InstrMask::TST,   InstrMask::UMLAL,
    InstrMask::UMULL,
};

constexpr U32 InstrEncodings[] = {
    InstrEncoding::ADC,   InstrEncoding::ADD,  InstrEncoding::AND,
    InstrEncoding::B,     InstrEncoding::BIC,  InstrEncoding::BL,
    InstrEncoding::BX,    InstrEncoding::CDP,  InstrEncoding::CMN,
    InstrEncoding::CMP,   InstrEncoding::EOR,  InstrEncoding::LDC,
    InstrEncoding::LDM,   InstrEncoding::LDR,  InstrEncoding::LDRB,
    InstrEncoding::LDRBT, InstrEncoding::LDRH, InstrEncoding::LDRSB,
    InstrEncoding::LDRSH, InstrEncoding::LDRT, InstrEncoding::MCR,
    InstrEncoding::MLA,   InstrEncoding::MOV,  InstrEncoding::MRC,
    InstrEncoding::MRS,   InstrEncoding::MSR,  InstrEncoding::MUL,
    InstrEncoding::MVN,   InstrEncoding::ORR,  InstrEncoding::RSB,
    InstrEncoding::RSC,   InstrEncoding::SBC,  InstrEncoding::SMLAL,
    InstrEncoding::SMULL, InstrEncoding::STC,  InstrEncoding::STM,
    InstrEncoding::STR,   InstrEncoding::STRB, InstrEncoding::STRBT,
    InstrEncoding::STRH,  InstrEncoding::STRT, InstrEncoding::SUB,
    InstrEncoding::SWI,   InstrEncoding::SWP,  InstrEncoding::SWPB,
    InstrEncoding::TEQ,   InstrEncoding::TST,  InstrEncoding::UMLAL,
    InstrEncoding::UMULL,
};

static_assert(sizeof(InstrEncodings) / sizeof(InstrEncodings[0]) ==
              U32(Instr::NUM_OPCODES));
static_assert(sizeof(InstrMasks) / sizeof(InstrMasks[0]) ==
              U32(Instr::NUM_OPCODES));

inline Instr GetArmOpcode(U32 instr) {
  for (U32 i = 0; i < U32(Instr::NUM_OPCODES); ++i) {
    if ((instr & InstrMasks[i]) == InstrEncodings[i]) {
      return Instr(i);
    }
  }
  ABORT("Could not find arm opcode for 0x%04X", instr);
}

} // namespace Emulator::Arm