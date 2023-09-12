#ifndef OPCODE_H
#define OPCODE_H

enum {
  LOAD = 0x3,
  STORE = 0x23,
  // The I-type Integer computation instructions
  // or Integer Register-Immediate Instructions
  // ADDI, SLTI, SLTIU, ANDI, ORI, XORI, SLLI,
  // SRLI, SRAI
  INTEGER_COMP_RI = 0x13,
  ADD = 0x33,
};

// FUNCT3 for LOAD/STORE
enum {
  BYTE = 0x0,
  HALF_WORD = 0x1,
  WORD = 0x2,
  DOUBLE_WORD = 0x3,
  BYTE_UNSIGNED = 0x4,
  HALF_WORD_UNSIGNED = 0x5,
  WORD_UNSIGNED = 0x6,

};

// FUNCT3 for INTEGER_COMPI_RI
enum {
  ADDI = 0x0,
  SLLI = 0x1,
  SLTI = 0x2,
  SLTIU = 0x3,
  XORI = 0x4,
  RIGHT_SHIFT = 0x5,
  ORI = 0x6,
  ANDI = 0x7,
};

#endif
