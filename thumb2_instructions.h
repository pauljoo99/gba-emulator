#pragma once

#include <cstdlib>
#include <stdio.h>

#include "datatypes.h"

namespace Emulator::Thumb2 {

struct InstructionFields {
  U32 : 15;
  U32 op : 1;
  U32 : 4;
  U32 op2 : 7;
  U32 op1 : 2;
  U32 : 3;
};

union Instruction {
  U32 value;
  InstructionFields fields;

  Instruction(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

struct CoprocessorSIMDFloatingFields {
  U32 : 4;
  U32 op : 1;
  U32 : 3;
  U32 coproc : 4;
  U32 : 4;
  U32 rn : 3;
  U32 op1 : 6;
  U32 : 6;
};

union CoprocessorSIMDFloatingInstr {
  U32 value;
  CoprocessorSIMDFloatingFields fields;

  CoprocessorSIMDFloatingInstr(U32 val = 0) : value(val) {}

  operator U32() const { return value; } // Implicit conversion
};

// clang-format off
enum class InstructionClass {
    LoadStoreMultiple,          // 01 00xx0xx - Load/store multiple (A6-235)
    LoadStoreDualExclusive,     // 00xx1xx - Load/store dual, load/store exclusive, table branch (A6-236)
    DataProcessingShiftedReg,   // 01xxxxx - Data-processing (shifted register) (A6-241)
    CoprocessorSIMDFloating,    // 1xxxxxx - Coprocessor, Advanced SIMD, Floating-point (A6-249)
    DataProcessingModImm,       // 10 x0xxxxx 0 - Data-processing (modified immediate) (A6-229)
    DataProcessingPlainImm,     // x1xxxxx 0 - Data-processing (plain binary immediate) (A6-232)
    BranchMiscControl,          // - 1 - Branches and miscellaneous control (A6-233)
    StoreSingleData,            // 11 000xxx0 - Store single data item (A6-240)
    LoadByteMemoryHint,         // 00xx001 - Load byte, memory hints (A6-239)
    LoadHalfwordMemoryHint,     // 00xx011 - Load halfword, memory hints (A6-238)
    LoadWord,                   // 00xx101 - Load word (A6-237)
    Undefined,                  // 00xx111 - UNDEFINED
    AdvancedSIMDLoadStore,      // 001xxx0 - Advanced SIMD element/structure load/store (A7-273)
    DataProcessingRegister,     // 010xxxx - Data-processing (register) (A6-243)
    MultiplyAccumulate,         // 0110xxx - Multiply, multiply accumulate, absolute difference (A6-247)
    LongMultiplyDivide,         // 0111xxx - Long multiply, long multiply accumulate, and divide (A6-248)
};
// clang-format on

// clang-format off
const char* toString(InstructionClass instr) {
    switch (instr) {
        case InstructionClass::LoadStoreMultiple:        return "Load/Store Multiple";
        case InstructionClass::LoadStoreDualExclusive:   return "Load/Store Dual/Exclusive, Table Branch";
        case InstructionClass::DataProcessingShiftedReg: return "Data-Processing (Shifted Register)";
        case InstructionClass::CoprocessorSIMDFloating:  return "Coprocessor, Advanced SIMD, Floating-Point";
        case InstructionClass::DataProcessingModImm:     return "Data-Processing (Modified Immediate)";
        case InstructionClass::DataProcessingPlainImm:   return "Data-Processing (Plain Binary Immediate)";
        case InstructionClass::BranchMiscControl:        return "Branch & Miscellaneous Control";
        case InstructionClass::StoreSingleData:          return "Store Single Data Item";
        case InstructionClass::LoadByteMemoryHint:       return "Load Byte, Memory Hints";
        case InstructionClass::LoadHalfwordMemoryHint:   return "Load Halfword, Memory Hints";
        case InstructionClass::LoadWord:                 return "Load Word";
        case InstructionClass::Undefined:                return "UNDEFINED";
        case InstructionClass::AdvancedSIMDLoadStore:    return "Advanced SIMD Element/Structure Load/Store";
        case InstructionClass::DataProcessingRegister:   return "Data-Processing (Register)";
        case InstructionClass::MultiplyAccumulate:       return "Multiply, Multiply Accumulate, Absolute Difference";
        case InstructionClass::LongMultiplyDivide:       return "Long Multiply, Long Multiply Accumulate, Divide";
        default: return "Unknown Instruction";
    }
}
// clang-format on

inline InstructionClass get_instr_class(U32 instr_) {
  if (((instr_ >> 13) & 0b111) != 0b111) {
    printf("%i is not a Thumb2 instruction", instr_);
    exit(0);
  }
  Instruction instr(instr_);
  if (instr.fields.op1 == 0b01) {
    if ((instr.fields.op2 & 0b1100100) == 0b0000000) {
      return InstructionClass::LoadStoreMultiple;
    } else if ((instr.fields.op2 & 0b1100100) == 0b0000100) {
      return InstructionClass::LoadStoreDualExclusive;
    } else if ((instr.fields.op2 & 0b1100000) == 0b0100000) {
      return InstructionClass::DataProcessingShiftedReg;
    } else if ((instr.fields.op2 & 0b1000000) == 0b1000000) {
      return InstructionClass::CoprocessorSIMDFloating;
    } else {
      printf("%i is not a Thumb2 instruction", instr_);
      exit(0);
    }
  } else if (instr.fields.op1 == 0b10) {
    if ((instr.fields.op2 & 0b0100000) == 0b0000000) {
      return InstructionClass::DataProcessingModImm;
    } else if ((instr.fields.op2 & 0b0100000) == 0b0100000) {
      return InstructionClass::DataProcessingPlainImm;
    } else {
      return InstructionClass::BranchMiscControl;
    }
  } else if (instr.fields.op1 == 0b11) {
    if ((instr.fields.op2 & 0b1110001) == 0b0000000) {
      return InstructionClass::StoreSingleData;
    } else if ((instr.fields.op2 & 0b1100111) == 0b0000001) {
      return InstructionClass::LoadByteMemoryHint;
    } else if ((instr.fields.op2 & 0b1100111) == 0b0000011) {
      return InstructionClass::LoadHalfwordMemoryHint;
    } else if ((instr.fields.op2 & 0b1100111) == 0b0000111) {
      return InstructionClass::Undefined;
    } else if ((instr.fields.op2 & 0b1110001) == 0b0010000) {
      return InstructionClass::AdvancedSIMDLoadStore;
    } else if ((instr.fields.op2 & 0b1110000) == 0b0100000) {
      return InstructionClass::DataProcessingRegister;
    } else if ((instr.fields.op2 & 0b1111000) == 0b0110000) {
      return InstructionClass::MultiplyAccumulate;
    } else if ((instr.fields.op2 & 0b1111000) == 0b0111000) {
      return InstructionClass::LongMultiplyDivide;
    } else if ((instr.fields.op2 & 0b1000000) == 0b1000000) {
      return InstructionClass::CoprocessorSIMDFloating;
    } else {
      printf("%i is not a Thumb2 instruction", instr_);
      exit(0);
    }
  }
  printf("%i is not a Thumb2 instruction", instr_);
  exit(0);
}

} // namespace Emulator::Thumb2