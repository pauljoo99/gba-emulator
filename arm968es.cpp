#include "arm968es.h"
#include <cstring>
#include <stdio.h>

namespace Emulator::Arm

{

inline uint32_t generateMask(uint8_t a, uint8_t b) {
  return ((1U << (b - a + 1)) - 1) << a;
}

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

bool evaluate_cond(ConditionCode cond, ConditionFlags &cond_flags) {
  switch (cond) {
  case ConditionCode::EQ:
    return cond_flags.z;
  case ConditionCode::NE:
    return !cond_flags.z;
  case ConditionCode::CS:
    return cond_flags.c;
  case ConditionCode::CC:
    return !cond_flags.c;
  case ConditionCode::MI:
    return cond_flags.n;
  case ConditionCode::PL:
    return !cond_flags.n;
  case ConditionCode::VS:
    return cond_flags.v;
  case ConditionCode::VC:
    return !cond_flags.v;
  case ConditionCode::HI:
    return cond_flags.c && !cond_flags.z;
  case ConditionCode::LS:
    return !cond_flags.c && cond_flags.z;
  case ConditionCode::GE:
    return cond_flags.n == cond_flags.v;
  case ConditionCode::LT:
    return cond_flags.n != cond_flags.v;
  case ConditionCode::GT:
    return !cond_flags.z && cond_flags.n == cond_flags.v;
  case ConditionCode::LE:
    return cond_flags.z || cond_flags.n != cond_flags.v;
  case ConditionCode::AL:
    return true;
  }
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

[[nodiscard]] bool process_instr(uint32_t instr, Instr::Instr instr_type,
                                 const Memory::Memory &memory, CPU &cpu) {

  printf("Instr: %s, ", toString(instr_type));
  printf("Raw Instr: 0x%08X", instr);
  printf("\n");

  switch (instr_type) {
  case Instr::Instr::B:
    return cpu.dispatch_B(instr);
    break;
  case Instr::Instr::MOV:
    return cpu.dispatch_MOV(instr, memory);
  default:
    return false;
  }
}

[[nodiscard]] bool get_reg(const uint8_t reg_num, Registers &regs,
                           uint32_t *&reg) {
  if (reg_num == 0) {
    reg = &regs.r0;
    return true;
  }
  printf("Could not find register %u", reg_num);
  return false;
}

[[nodiscard]] bool CPU::dispatch(const GameCard::GameCard &game_card,
                                 const Memory::Memory &memory) noexcept {

  if (registers.r15 > sizeof(game_card.mem) / sizeof(game_card.mem[0])) {
    return false;
  }

  uint32_t instr;
  memcpy(&instr, (void *)&game_card.mem[registers.r15], kInstrSize);

  Instr::Instr instr_type;
  if (!get_instr_type(instr, instr_type)) {
    return false;
  }

  return process_instr(instr, instr_type, memory, *this);
}

[[nodiscard]] bool CPU::dispatch_B(uint32_t instr) noexcept {
  if (evaluate_cond(ConditionCode(instr >> 28), cond_flags)) {
    const uint32_t offset = instr & ((1 << 23) - 1);
    registers.r15 += (offset << 2) + 8;
  } else {
    registers.r15 += kInstrSize;
  }
  return true;
}

[[nodiscard]] bool CPU::dispatch_MOV(uint32_t instr,
                                     const Memory::Memory &memory) noexcept {
  if ((instr & generateMask(25, 25)) == 0) {
    const uint32_t shift = (instr & generateMask(4, 11)) >> 4;
    const uint32_t rm = instr & generateMask(0, 3);
    (void)shift;
    (void)rm;
    printf("Not yet implemented.");
    return false;
  } else {
    const uint32_t rotate = ((instr & generateMask(8, 11)) >> 8) * 2;
    const uint8_t lmm = instr & generateMask(0, 7);
    const uint8_t rotated_lmm = (lmm >> rotate) | (lmm << (8 - rotate));
    uint32_t *r = nullptr;
    if (!get_reg((instr & generateMask(12, 15)) >> 12, registers, r)) {
      return false;
    }
    *r = rotated_lmm;
    registers.r15 += kInstrSize;
    return true;
  }
}

} // namespace Emulator::Arm
