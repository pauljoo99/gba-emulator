#pragma once

#include "datatypes.h"

namespace Emulator::Thumb {

// Move Shifted Register (LSL, LSR, ASR)
struct MoveShiftedRegisterFields {
  U16 rd : 3;
  U16 rs : 3;
  U16 offset5 : 5;
  U16 op2 : 2;
  U16 : 3;
};

union MoveShiftedRegister {
  U16 value;
  MoveShiftedRegisterFields fields;

  MoveShiftedRegister(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

// Add/Subtract Register
struct AddSubtractRegisterFields {
  U16 rd : 3;
  U16 rs : 3;
  U16 rn_or_imm : 3;
  U16 op : 1;
  U16 : 6;
};

union AddSubtractRegister {
  U16 value;
  AddSubtractRegisterFields fields;

  AddSubtractRegister(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

// Immediate Operations (MOV, CMP, ADD, SUB)
struct ImmediateOperationFields {
  U16 rd : 3;
  U16 imm8 : 8;
  U16 op : 2;
  U16 : 3;
};

union ImmediateOperation {
  U16 value;
  ImmediateOperationFields fields;

  ImmediateOperation(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

// ALU Operations (AND, EOR, LSL, LSR, ASR, ADC, SBC, ROR, TST, NEG, CMP, CMN,
// ORR, MUL, BIC, MVN)
struct ALUOperationFields {
  U16 rd : 3;
  U16 rs : 3;
  U16 op : 4;
  U16 : 6;
};

union ALUOperation {
  U16 value;
  ALUOperationFields fields;

  ALUOperation(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

// High Register Operations (MOV, ADD, CMP, BX)
struct HighRegisterOperationFields {
  U16 rd : 3;
  U16 rs : 3;
  U16 op : 2;
  U16 h1 : 1;
  U16 h2 : 1;
  U16 : 6;
};

union HighRegisterOperation {
  U16 value;
  HighRegisterOperationFields fields;

  HighRegisterOperation(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

// Load/Store Word (LDR, STR)
struct LoadStoreWordFields {
  U16 rd : 3;
  U16 rb : 3;
  U16 offset5 : 5;
  U16 l : 1; // 1 = Load, 0 = Store
  U16 : 4;
};

union LoadStoreWord {
  U16 value;
  LoadStoreWordFields fields;

  LoadStoreWord(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

// Load/Store Byte (LDRB, STRB)
struct LoadStoreByteFields {
  U16 rd : 3;
  U16 rb : 3;
  U16 offset5 : 5;
  U16 b : 1;
  U16 l : 1;
  U16 : 3;
};

union LoadStoreByte {
  U16 value;
  LoadStoreByteFields fields;

  LoadStoreByte(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

// Load/Store with SP-relative Addressing
struct LoadStoreSPRelativeFields {
  U16 rd : 3;
  U16 imm8 : 8;
  U16 l : 1;
  U16 : 4;
};

union LoadStoreSPRelative {
  U16 value;
  LoadStoreSPRelativeFields fields;

  LoadStoreSPRelative(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

// Conditional Branch (BNE, BEQ, BGE, etc.)
struct ConditionalBranchFields {
  U16 offset8 : 8;
  U16 cond : 4;
  U16 : 4;
};

union ConditionalBranch {
  U16 value;
  ConditionalBranchFields fields;

  ConditionalBranch(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

// Unconditional Branch (B)
struct UnconditionalBranchFields {
  U16 offset11 : 11;
  U16 : 5;
};

union UnconditionalBranch {
  U16 value;
  UnconditionalBranchFields fields;

  UnconditionalBranch(U16 val = 0) : value(val) {}
  operator U16() const { return value; }
};

enum class ThumbOpcode {
  ADC,      // Add with Carry
  ADD,      // Add
  AND,      // AND
  ASR,      // Arithmetic Shift Right
  B,        // Unconditional Branch
  BXX,      // Conditional Branch
  BIC,      // Bit Clear
  BL,       // Branch and Link
  BLX,      // Branch with Link and Exchange
  BX,       // Branch and Exchange
  CMN,      // Compare Negative
  CMP,      // Compare
  EOR,      // EOR
  LDMIA,    // Load Multiple
  LDR,      // Load Word
  LDRB,     // Load Byte
  LDRH,     // Load Halfword
  LSL,      // Logical Shift Left
  LDSB,     // Load Sign-Extended Byte
  LDSH,     // Load Sign-Extended Halfword
  LSR,      // Logical Shift Right
  MOV,      // Move Register
  MUL,      // Multiply
  MVN,      // Move Negative Register
  NEG,      // Negate
  ORR,      // OR
  POP,      // Pop Registers
  PUSH,     // Push Registers
  ROR,      // Rotate Right
  SBC,      // Subtract with Carry
  STMIA,    // Store Multiple
  STR,      // Store Word
  STRB,     // Store Byte
  STRH,     // Store Halfword
  SWI,      // Software Interrupt
  SUB,      // Subtract
  TST,      // Test Bits
  UNDEFINED // Placeholder for unknown instructions
};

const char *toString(ThumbOpcode cc) {
  switch (cc) {
  case ThumbOpcode::ADC:
    return "Add with Carry";
  case ThumbOpcode::ADD:
    return "Add";
  case ThumbOpcode::AND:
    return "AND";
  case ThumbOpcode::ASR:
    return "Arithmetic Shift Right";
  case ThumbOpcode::B:
    return "Unconditional Branch";
  case ThumbOpcode::BXX:
    return "Conditional Branch";
  case ThumbOpcode::BIC:
    return "Bit Clear";
  case ThumbOpcode::BL:
    return "Branch and Link";
  case ThumbOpcode::BLX:
    return "Branch with Link and Exchange";
  case ThumbOpcode::BX:
    return "Branch and Exchange";
  case ThumbOpcode::CMN:
    return "Compare Negative";
  case ThumbOpcode::CMP:
    return "Compare";
  case ThumbOpcode::EOR:
    return "EOR";
  case ThumbOpcode::LDMIA:
    return "Load Multiple";
  case ThumbOpcode::LDR:
    return "Load Word";
  case ThumbOpcode::LDRB:
    return "Load Byte";
  case ThumbOpcode::LDRH:
    return "Load Halfword";
  case ThumbOpcode::LSL:
    return "Logical Shift Left";
  case ThumbOpcode::LDSB:
    return "Load Sign-Extended Byte";
  case ThumbOpcode::LDSH:
    return "Load Sign-Extended Halfword";
  case ThumbOpcode::LSR:
    return "Logical Shift Right";
  case ThumbOpcode::MOV:
    return "Move Register";
  case ThumbOpcode::MUL:
    return "Multiply";
  case ThumbOpcode::MVN:
    return "Move Negative Register";
  case ThumbOpcode::NEG:
    return "Negate";
  case ThumbOpcode::ORR:
    return "OR";
  case ThumbOpcode::POP:
    return "Pop Registers";
  case ThumbOpcode::PUSH:
    return "Push Registers";
  case ThumbOpcode::ROR:
    return "Rotate Right";
  case ThumbOpcode::SBC:
    return "Subtract with Carry";
  case ThumbOpcode::STMIA:
    return "Store Multiple";
  case ThumbOpcode::STR:
    return "Store Word";
  case ThumbOpcode::STRB:
    return "Store Byte";
  case ThumbOpcode::STRH:
    return "Store Halfword";
  case ThumbOpcode::SWI:
    return "Software Interrupt";
  case ThumbOpcode::SUB:
    return "Subtract";
  case ThumbOpcode::TST:
    return "Test Bits";
  case ThumbOpcode::UNDEFINED:
    return "Placeholder for unknown instructions";
  }
};

} // namespace Emulator::Thumb