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
// clang-format off
enum InstrMask : U32 {
  ADC = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  ADD = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  AND = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  B   = 0b0000'1110'0000'0000'0000'0000'0000'0000,
  BIC = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  BL  = 0b0000'1110'0000'0000'0000'0000'0000'0000,
  BX  = 0b0000'1111'1111'1111'1111'1111'1111'0000,
  CDP = 0b0000'1111'0000'0000'0000'0000'0001'0000,
  CMN = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  CMP = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  EOR = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  LDC = 0b0000'1110'0001'0000'0000'0000'0000'0000,
  LDM = 0b0000'1110'0001'0000'0000'0000'0000'0000,
  LDR = 0b0000'1100'0001'0000'0000'0000'0000'0000,
  MCR = 0b0000'1111'0001'0000'0000'0000'0001'0000,
  MLA = 0b0000'1111'0110'0000'0000'0000'1111'0000,
  MOV = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  MRC = 0b0000'1111'0001'0000'0000'0000'0001'0000,
  MRS = 0b0000'1111'1011'1111'0000'1111'1111'1111,
  MSR = 0b0000'1101'1011'1111'1111'0000'0000'0000,
  MUL = 0b0000'1111'0110'0000'0000'0000'1111'0000,
  MVN = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  ORR = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  RSB = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  RSC = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  SBC = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  STC = 0b0000'1110'0001'0000'0000'0000'0000'0000,
  STM = 0b0000'1110'0001'0000'0000'0000'0000'0000,
  STR = 0b0000'1100'0001'0000'0000'0000'0000'0000,
  SUB = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  SWI = 0b0000'1111'0000'0000'0000'0000'0000'0000,
  SWP = 0b0000'1111'1011'0000'0000'1111'1111'0000,
  TEQ = 0b0000'1101'1110'0000'0000'0000'0000'0000,
  TST = 0b0000'1101'1110'0000'0000'0000'0000'0000,
};

}
// clang-format on

namespace Instr {
// clang-format off
enum Instr : U32 {
  ADC = 0b0000'0000'1010'0000'0000'0000'0000'0000,
  ADD = 0b0000'0000'1000'0000'0000'0000'0000'0000,
  AND = 0b0000'0000'0000'0000'0000'0000'0000'0000,
  B   = 0b0000'1010'0000'0000'0000'0000'0000'0000,
  BIC = 0b0000'0001'1100'0000'0000'0000'0000'0000,
  BL  = 0b0000'1011'0000'0000'0000'0000'0000'0000,
  BX  = 0b0000'0001'0010'1111'1111'1111'0001'0000,
  CDP = 0b0000'1110'0000'0000'0000'0000'0000'0000,
  CMN = 0b0000'0001'0111'0000'0000'0000'0000'0000,
  CMP = 0b0000'0001'0100'0000'0000'0000'0000'0000,
  EOR = 0b0000'0000'0010'0000'0000'0000'0000'0000,
  LDC = 0b0000'1100'0001'0000'0000'0000'0000'0000,
  LDM = 0b0000'1000'0001'0000'0000'0000'0000'0000,
  LDR = 0b0000'0100'0001'0000'0000'0000'0000'0000, // revisit
  MCR = 0b0000'1110'0000'0000'0000'0000'0001'0000,
  MLA = 0b0000'0000'0010'0000'0000'0000'1001'0000,
  MOV = 0b0000'0001'1010'0000'0000'0000'0000'0000,
  MRC = 0b0000'1110'0001'0000'0000'0000'0001'0000,
  MRS = 0b0000'0001'0000'1111'0000'0000'0000'0000,
  MSR = 0b0000'0001'0010'1001'1111'0000'0000'0000,
  MUL = 0b0000'0000'0000'0000'0000'0000'1001'0000,
  MVN = 0b0000'0001'1110'0000'0000'0000'0000'0000,
  ORR = 0b0000'0001'1000'0000'0000'0000'0000'0000,
  RSB = 0b0000'0000'0110'0000'0000'0000'0000'0000,
  RSC = 0b0000'0000'1110'0000'0000'0000'0000'0000,
  SBC = 0b0000'0000'1100'0000'0000'0000'0000'0000,
  STC = 0b0000'1100'0000'0000'0000'0000'0000'0000,
  STM = 0b0000'1000'0000'0000'0000'0000'0000'0000,
  STR = 0b0000'0100'0000'0000'0000'0000'0000'0000,
  SUB = 0b0000'0000'0100'0000'0000'0000'0000'0000,
  SWI = 0b0000'1111'0000'0000'0000'0000'0000'0000,
  SWP = 0b0000'0001'0000'0000'0000'0000'1001'0000,
  TEQ = 0b0000'0001'0010'0000'0000'0000'0000'0000,
  TST = 0b0000'0001'0000'0000'0000'0000'0000'0000,
};
}
// clang-format on

inline const char *toString(Instr::Instr instr) {
  switch (instr) {
  case Instr::Instr::ADC:
    return "ADC";
  case Instr::Instr::ADD:
    return "ADD";
  case Instr::Instr::AND:
    return "AND";
  case Instr::Instr::B:
    return "B";
  case Instr::Instr::BIC:
    return "BIC";
  case Instr::Instr::BL:
    return "BL";
  case Instr::Instr::BX:
    return "BX";
  case Instr::Instr::CDP:
    return "CDP";
  case Instr::Instr::CMN:
    return "CMN";
  case Instr::Instr::CMP:
    return "CMP";
  case Instr::Instr::EOR:
    return "EOR";
  case Instr::Instr::LDC:
    return "LDC";
  case Instr::Instr::LDM:
    return "LDM";
  case Instr::Instr::LDR:
    return "LDR";
  case Instr::Instr::MCR:
    return "MCR";
  case Instr::Instr::MLA:
    return "MLA";
  case Instr::Instr::MOV:
    return "MOV";
  case Instr::Instr::MRC:
    return "MRC";
  case Instr::Instr::MRS:
    return "MRS";
  case Instr::Instr::MSR:
    return "MSR";
  case Instr::Instr::MUL:
    return "MUL";
  case Instr::Instr::MVN:
    return "MVN";
  case Instr::Instr::ORR:
    return "ORR";
  case Instr::Instr::RSB:
    return "RSB";
  case Instr::Instr::RSC:
    return "RSC";
  case Instr::Instr::SBC:
    return "SBC";
  case Instr::Instr::STC:
    return "STC";
  case Instr::Instr::STM:
    return "STM";
  case Instr::Instr::STR:
    return "STR";
  case Instr::Instr::SUB:
    return "SUB";
  case Instr::Instr::SWI:
    return "SWI";
  case Instr::Instr::SWP:
    return "SWP";
  case Instr::Instr::TEQ:
    return "TEQ";
  case Instr::Instr::TST:
    return "TST";
  }
}

[[nodiscard]] __inline bool get_instr_type(U32 instr,
                                           Instr::Instr &instry_type) {

#define checkInstr(instr_type_)                                                \
  if ((instr & U32(InstrMask::InstrMask::instr_type_)) ==                      \
      U32(Instr::Instr::instr_type_)) {                                        \
    instry_type = Instr::Instr::instr_type_;                                   \
    return true;                                                               \
  }

  checkInstr(ADC);
  checkInstr(ADD);
  checkInstr(AND);
  checkInstr(B);
  checkInstr(BIC);
  checkInstr(BL);
  checkInstr(BX);
  checkInstr(CDP);
  checkInstr(CMN);
  checkInstr(CMP);
  checkInstr(EOR);
  checkInstr(LDC);
  checkInstr(LDM);
  checkInstr(LDR);
  checkInstr(MCR);
  checkInstr(MLA);
  checkInstr(MOV);
  checkInstr(MRC);
  checkInstr(MRS);
  checkInstr(MSR);
  checkInstr(MUL);
  checkInstr(MVN);
  checkInstr(ORR);
  checkInstr(RSB);
  checkInstr(RSC);
  checkInstr(SBC);
  checkInstr(STC);
  checkInstr(STM);
  checkInstr(STR);
  checkInstr(SUB);
  checkInstr(SWI);
  checkInstr(SWP);
  checkInstr(TEQ);
  checkInstr(TST);

  return false;

#undef checkInstr
}

} // namespace Emulator::Arm