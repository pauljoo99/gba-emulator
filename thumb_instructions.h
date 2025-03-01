#pragma once

#include <cstdint>

namespace Emulator::Thumb {

// Move Shifted Register (LSL, LSR, ASR)
struct MoveShiftedRegisterFields {
  uint16_t rd : 3;
  uint16_t rs : 3;
  uint16_t offset5 : 5;
  uint16_t op2 : 2;
  uint16_t : 3;
};

union MoveShiftedRegister {
  uint16_t value;
  MoveShiftedRegisterFields fields;

  MoveShiftedRegister(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
};

// Add/Subtract Register
struct AddSubtractRegisterFields {
  uint16_t rd : 3;
  uint16_t rs : 3;
  uint16_t rn_or_imm : 3;
  uint16_t op : 1;
  uint16_t : 6;
};

union AddSubtractRegister {
  uint16_t value;
  AddSubtractRegisterFields fields;

  AddSubtractRegister(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
};

// Immediate Operations (MOV, CMP, ADD, SUB)
struct ImmediateOperationFields {
  uint16_t rd : 3;
  uint16_t imm8 : 8;
  uint16_t op : 2;
  uint16_t : 3;
};

union ImmediateOperation {
  uint16_t value;
  ImmediateOperationFields fields;

  ImmediateOperation(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
};

// ALU Operations (AND, EOR, LSL, LSR, ASR, ADC, SBC, ROR, TST, NEG, CMP, CMN,
// ORR, MUL, BIC, MVN)
struct ALUOperationFields {
  uint16_t rd : 3;
  uint16_t rs : 3;
  uint16_t op : 4;
  uint16_t : 6;
};

union ALUOperation {
  uint16_t value;
  ALUOperationFields fields;

  ALUOperation(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
};

// High Register Operations (MOV, ADD, CMP, BX)
struct HighRegisterOperationFields {
  uint16_t rd : 3;
  uint16_t rs : 3;
  uint16_t op : 2;
  uint16_t h1 : 1;
  uint16_t h2 : 1;
  uint16_t : 6;
};

union HighRegisterOperation {
  uint16_t value;
  HighRegisterOperationFields fields;

  HighRegisterOperation(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
};

// Load/Store Word (LDR, STR)
struct LoadStoreWordFields {
  uint16_t rd : 3;
  uint16_t rb : 3;
  uint16_t offset5 : 5;
  uint16_t l : 1; // 1 = Load, 0 = Store
  uint16_t : 4;
};

union LoadStoreWord {
  uint16_t value;
  LoadStoreWordFields fields;

  LoadStoreWord(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
};

// Load/Store Byte (LDRB, STRB)
struct LoadStoreByteFields {
  uint16_t rd : 3;
  uint16_t rb : 3;
  uint16_t offset5 : 5;
  uint16_t b : 1;
  uint16_t l : 1;
  uint16_t : 3;
};

union LoadStoreByte {
  uint16_t value;
  LoadStoreByteFields fields;

  LoadStoreByte(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
};

// Load/Store with SP-relative Addressing
struct LoadStoreSPRelativeFields {
  uint16_t rd : 3;
  uint16_t imm8 : 8;
  uint16_t l : 1;
  uint16_t : 4;
};

union LoadStoreSPRelative {
  uint16_t value;
  LoadStoreSPRelativeFields fields;

  LoadStoreSPRelative(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
};

// Conditional Branch (BNE, BEQ, BGE, etc.)
struct ConditionalBranchFields {
  uint16_t offset8 : 8;
  uint16_t cond : 4;
  uint16_t : 4;
};

union ConditionalBranch {
  uint16_t value;
  ConditionalBranchFields fields;

  ConditionalBranch(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
};

// Unconditional Branch (B)
struct UnconditionalBranchFields {
  uint16_t offset11 : 11;
  uint16_t : 5;
};

union UnconditionalBranch {
  uint16_t value;
  UnconditionalBranchFields fields;

  UnconditionalBranch(uint16_t val = 0) : value(val) {}
  operator uint16_t() const { return value; }
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