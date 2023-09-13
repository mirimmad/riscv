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
#define SHAMT_MASK 0x1F

typedef struct {
  uint32_t regs[32];
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
uint32_t cpu_load(CPU *cpu, uint32_t addr, uint8_t size);
void cpu_store(CPU *cup, uint32_t addr, uint32_t value, uint8_t size);

uint32_t cpu_fetch(CPU *);
void cpu_decode(uint32_t raw_inst, INST *inst);
void cpu_execute(CPU *, uint32_t inst);

#define log_RR(INST)                                                           \
  log(INST);                                                                   \
  log(" :");                                                                   \
  log("r(%d) <- r(%d) r(%d)\n", inst.rd, inst.rs1, inst.rs2);                  \
  log("r(%d)=%d\n", inst.rd, cpu->regs[inst.rd]);

#define log_RI(INST)                                                           \
  log(INST);                                                                   \
  log(" :");                                                                   \
  log("r(%d) <- r(%d) %d\n", inst.rd, inst.rs1, imm);                          \
  log("r(%d)=%d\n", inst.rd, cpu->regs[inst.rd]);

#define log_JMP(JMP)                                                           \
  log(JMP);                                                                    \
  log(" :Jump to addr %x\n", jmp_addr);                                        \
  log("r(%d)=%d\n", inst.rd, cpu->pc);

#endif