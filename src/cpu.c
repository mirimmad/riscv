#include "include/cpu.h"

CPU *newCPU(uint8_t *code, size_t len) {
  CPU *cpu = (CPU *)malloc(sizeof(CPU));
  cpu->regs[0] = 0;
  // Stack pointer
  cpu->regs[2] = RAM_BASE + RAM_SIZE;
  cpu->pc = RAM_BASE;

  cpu->ram = newRAM(code, len);
  return cpu;
}

void cpu_state(CPU *cpu) {
  for (int i = 0; i < 32; i++) {
    printf("r[%d]: %ld\n", i, cpu->regs[i]);
  }
}

uint64_t cpu_load(CPU *cpu, uint64_t addr, uint8_t size) {
  ram_load(cpu->ram, addr, size);
}
void cpu_store(CPU *cpu, uint64_t addr, uint64_t value, uint8_t size) {
  ram_store(cpu->ram, addr, value, size);
}

uint64_t cpu_fetch(CPU *cpu) { return cpu_load(cpu, cpu->pc, 32); }

void cpu_decode(uint64_t inst_raw, INST *inst) {
  uint8_t opcode = inst_raw & OPCODE_MASK;
  uint8_t rd = (inst_raw >> RD_SHIFT) & REG_ADDR_MASK;
  uint8_t rs1 = (inst_raw >> RS1_SHIFT) & REG_ADDR_MASK;
  uint8_t rs2 = (inst_raw >> RS2_SHIFT) & REG_ADDR_MASK;
  uint8_t funct3 = (inst_raw >> FUNCT3_SHIFT) & FUNCT3_MASK;
  uint8_t funct7 = (inst_raw >> FUNCT7_SHIFT) & FUNCT7_SHIFT;

  inst->opcode = opcode;
  inst->rd = rd;
  inst->rs1 = rs1;
  inst->rs2 = rs2;
  inst->funct3 = funct3;
  inst->funct7 = funct7;
}

INST inst;
void cpu_execute(CPU *cpu, uint64_t inst_raw) {
  cpu_decode(inst_raw, &inst);

  switch (inst.opcode) {
  case ADDI:
    log("found ADDI\n");
    break;
  case ADD:
    log("found ADD\n");
    break;
  default:
    log("found unknown\n");
    break;
  }
}