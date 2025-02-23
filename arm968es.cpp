#include "arm968es.h"
#include <cstring>
#include <stdio.h>

namespace Emulator::Arm

{

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
  UKN = 0,
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

ConditionCode process_condition(uint32_t instr) {
  return ConditionCode(instr >> 28);
}

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
  default:
    return "UKN";
  }
}

Instr::Instr get_instr(uint32_t instr) {

#define checkInstr(instr_type)                                                 \
  if ((instr & uint32_t(InstrMask::InstrMask::instr_type)) ==                  \
      uint32_t(Instr::Instr::instr_type)) {                                    \
    return Instr::Instr::instr_type;                                           \
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

  return Instr::Instr::UKN;

#undef checkInstr
}

[[nodiscard]] bool CPU::dispatch(const GameCard::GameCard &game_card) noexcept {

  uint32_t instr;
  memcpy(&instr, (void *)&game_card.mem[pc], kInstrSize);

  printf("Condition: %s, ", toString(process_condition(instr)));
  printf("Instr: %s, ", toString(get_instr(instr)));
  printf("Raw Instr: 0x%08X", instr);
  printf("\n");

  pc += kInstrSize;

  if (pc > sizeof(game_card.mem) / sizeof(game_card.mem[0])) {
    return false;
  }

  return true;
}

} // namespace Emulator::Arm
