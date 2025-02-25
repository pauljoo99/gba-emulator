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

} // namespace Emulator::Thumb