#pragma once

#include <cstdint>

namespace Emulator::Arm {

struct SingleDataTransferInstrFields {
  uint32_t offset : 12;
  uint32_t rd : 4;
  uint32_t rn : 4;
  uint32_t l : 1;
  uint32_t w : 1;
  uint32_t b : 1;
  uint32_t u : 1;
  uint32_t p : 1;
  uint32_t i : 1;
  uint32_t : 2;
  uint32_t cond : 4;
};

union SingleDataTransferInstr {
  uint32_t value;
  SingleDataTransferInstrFields fields;

  SingleDataTransferInstr(uint32_t val = 0) : value(val) {}

  operator uint32_t() const { return value; } // Implicit conversion
};

struct MSRInstrFields {
  uint32_t rm : 4;
  uint32_t : 8;
  uint32_t : 10;
  uint32_t dest_psr : 1;
  uint32_t : 5;
  uint32_t cond : 4;
};

union MSRInstr {
  uint32_t value;
  MSRInstrFields fields;

  MSRInstr(uint32_t val = 0) : value(val) {}

  operator uint32_t() const { return value; } // Implicit conversion
};

struct DataProcessingInstrFields {
  uint32_t operand_2 : 12;
  uint32_t rd : 4;
  uint32_t rn : 4;
  uint32_t s : 1;
  uint32_t opcode : 4;
  uint32_t i : 1;
  uint32_t : 2;
  uint32_t cond : 4;
};

union DataProcessingInstr {
  uint32_t value;
  DataProcessingInstrFields fields;

  DataProcessingInstr(uint32_t val = 0) : value(val) {}

  operator uint32_t() const { return value; } // Implicit conversion
};

struct BranchInstrFields {
  uint32_t offset : 24;
  uint32_t link_bit : 1;
  uint32_t : 3;
  uint32_t cond : 4;
};

union BranchInstr {
  uint32_t value;
  BranchInstrFields fields;

  BranchInstr(uint32_t val = 0) : value(val) {}

  operator uint32_t() const { return value; } // Implicit conversion
};

struct BranchAndExchangeInstrFields {
  uint32_t rn : 4;
  uint32_t : 24;
  uint32_t cond : 4;
};

union BranchAndExchangeInstr {
  uint32_t value;
  BranchAndExchangeInstrFields fields;

  BranchAndExchangeInstr(uint32_t val = 0) : value(val) {}

  operator uint32_t() const { return value; } // Implicit conversion
};

enum ConditionCode : uint8_t {
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

const char *toString(ConditionCode cc) {
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
enum InstrMask : uint32_t {
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
enum Instr : uint32_t {
  ADC = 0b0000'0000'1010'0000'0000'0000'0000'0000,
  ADD = 0b0000'0000'1000'0000'0000'0000'0000'0000,
  AND = 0b0000'0000'0000'0000'0000'0000'0000'0000,
  B   = 0b0000'1010'0000'0000'0000'0000'0000'0000,
  BIC = 0b0000'0001'1100'0000'0000'0000'0000'0000,
  BL  = 0b0000'1011'0000'0000'0000'0000'0000'0000,
  BX  = 0b0000'0001'0010'1111'1111'1111'0001'0000,
  CDP = 0b0000'1110'0000'0000'0000'0000'0000'0000,
  CMN = 0b0000'0001'0110'0000'0000'0000'0000'0000,
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

const char *toString(Instr::Instr instr) {
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

[[nodiscard]] bool get_instr_type(uint32_t instr, Instr::Instr &instry_type) {

#define checkInstr(instr_type_)                                                \
  if ((instr & uint32_t(InstrMask::InstrMask::instr_type_)) ==                 \
      uint32_t(Instr::Instr::instr_type_)) {                                   \
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