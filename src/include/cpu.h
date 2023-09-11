#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "mem.h"
#include "opcode.h"

#define OPCODE_MASK 0x7F
#define REG_ADDR_MASK 0x1F
#define RD_SHIFT 7
#define RS1_SHIFT 15
#define RS2_SHIFT 20
#define FUNCT3_SHIFT 12
#define FUNCT7_SHIFT 25
#define FUNCT3_MASK 0x7
#define FUNCT7_MASK 0x7F

typedef struct {
  uint64_t regs[32];
  size_t pc;
  RAM *ram;
} CPU;

typedef struct {
  uint8_t opcode;
  uint8_t rd;
  uint8_t rs1;
  uint8_t rs2;
  uint8_t funct3;
  uint8_t funct7;
} INST;

CPU *newCPU(uint8_t *code, size_t len);
void freeCPU(CPU *);
void cpu_state(CPU *);
uint64_t cpu_load(CPU *cpu, uint64_t addr, uint8_t size);
void cpu_store(CPU *cup, uint64_t addr, uint64_t value, uint8_t size);

uint64_t cpu_fetch(CPU *);
void cpu_decode(uint64_t raw_inst, INST *inst);
void cpu_execute(CPU *, uint64_t inst);

#endif