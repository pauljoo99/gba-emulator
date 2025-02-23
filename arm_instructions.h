#pragma once

#include <cstdint>

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