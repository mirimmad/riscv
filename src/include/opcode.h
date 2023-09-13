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

  // The R-type Integer computation instructions
  // or Integer Register-Regsiter Instructions
  // ADD, SUB, SLT, SLTU, ANT, OR, XOR, SLI,
  // SRL, SRA
  INTEGER_COMP_RR = 0x33,
  LUI = 0x37,
  AUIPC = 0x17,
  JAL = 0x6F,
  JALR = 0x67,
  // Conditional branches: BEQ, BNE, BLT, BGE, BLTU, BGEU
  BRANCH = 0x63,
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

// FUNCT3, FUNCT7 pairs for INTEGER_COMP_RR
#define ADD 0x0, 0x0
#define MUL 0x0, 0x1
#define MULH 0x1, 0x1
#define MULHSU 0x2, 0x1
#define MULHU 0x3, 0x1
#define DIV 0x4, 0x1
#define DIVU 0x5, 0x1
#define REM 0x6, 0x1
#define REMU 0x7, 0x1
#define SUB 0x0, 0x20
#define SLL 0x1, 0x0
#define SLT 0x2, 0x0
#define SLTU 0x3, 0x0
#define XOR 0x4, 0x0
#define SRL 0x5, 0x0
#define SRA 0x5, 0x20
#define OR 0x6, 0x0
#define AND 0x7, 0x0

// FUNCT3 for BRANCH
enum {
  BEQ = 0x0,
  BNE = 0x1,
  BLT = 0x4,
  BGE = 0x5,
  BLTU = 0x6,
  BGEU = 0x7,
};

#endif
