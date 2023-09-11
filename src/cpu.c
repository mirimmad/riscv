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

void freeCPU(CPU *cpu) {
  freeRAM(cpu->ram);
  free(cpu);
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
  uint8_t funct7 = (inst_raw >> FUNCT7_SHIFT) & FUNCT7_MASK;

  inst->opcode = opcode;
  inst->rd = rd;
  inst->rs1 = rs1;
  inst->rs2 = rs2;
  inst->funct3 = funct3;
  inst->funct7 = funct7;
}

int32_t i_imm(uint64_t inst_raw) {
  // imm[11:0] = inst[31:20]
  return ((int32_t)inst_raw) >> 20;
}

int32_t s_imm(uint64_t inst_raw) {
  // imm[11:5 | 4:0] = inst[31:25 | 11:7]
  return ((int32_t)(inst_raw & 0xFE000000)) >> 20 |
         ((int32_t)(inst_raw >> 7) & 0x1F);
}

INST inst;
void cpu_execute(CPU *cpu, uint64_t inst_raw) {
  cpu_decode(inst_raw, &inst);
  cpu->regs[0] = 0;
  switch (inst.opcode) {

  case LOAD: {
    int32_t imm = i_imm(inst_raw);
    uint32_t addr = cpu->regs[inst.rs1] + imm;
    uint64_t val;
    switch (inst.funct3) {
    case BYTE: {
      val = (int8_t)cpu_load(cpu, addr, 8);
      cpu->regs[inst.rd] = val;
    } break;
    case HALF_WORD: {
      val = (int16_t)cpu_load(cpu, addr, 16);
      cpu->regs[inst.rd] = val;
    } break;
    case WORD: {
      val = (int32_t)cpu_load(cpu, addr, 32);
      cpu->regs[inst.rd] = val;
    } break;
    case DOUBLE_WORD: {
      val = cpu_load(cpu, addr, 64);
      cpu->regs[inst.rd] = val;
    } break;
    case BYTE_UNSIGNED: {
      val = cpu_load(cpu, addr, 8);
      cpu->regs[inst.rd] = val;
    } break;
    case HALF_WORD_UNSIGNED: {
      val = cpu_load(cpu, addr, 16);
      cpu->regs[inst.rd] = val;
    } break;
    case WORD_UNSIGNED: {
      val = cpu_load(cpu, addr, 32);
      cpu->regs[inst.rd] = val;
    } break;
    default: {
      log("Illegal FUNCT3 for LOAD: %d\n", inst.funct3);
      exit(-1);
    }
    }

    log("LOAD %d to reg r(%d) from addr 0x%x\n", val, inst.rd, addr);
  } break;

  case STORE: {
    int32_t imm = s_imm(inst_raw);
    uint32_t addr = cpu->regs[inst.rs1] + imm;
    uint64_t val = cpu->regs[inst.rs2];
    switch (inst.funct3) {
    case BYTE:
      cpu_store(cpu, addr, val, 8);
      break;
    case HALF_WORD:
      cpu_store(cpu, addr, val, 16);
      break;
    case WORD:
      cpu_store(cpu, addr, val, 32);
      break;
    case DOUBLE_WORD:
      cpu_store(cpu, addr, val, 64);
      break;
    default: {
      log("Illegal FUNCT3 for STORE: %d\n", inst.funct3);
    }
    }
    log("STORE r[%d] at 0x%x\n", inst.rs2, addr);
  } break;

  case ADDI: {
    int32_t imm = i_imm(inst_raw);
    cpu->regs[inst.rd] = cpu->regs[inst.rs1] + imm;
    log("ADDI r(%d) <- r(%d) %d\n", inst.rd, inst.rs1, imm);
  } break;
  case ADD: {
    cpu->regs[inst.rd] = cpu->regs[inst.rs1] + cpu->regs[inst.rs2];
    log("ADD r(%d) <- r(%d) r(%d)\n", inst.rd, inst.rs1, inst.rs2);
  } break;
  default: {
    log("Illegal Instruction 0x%x\n", inst.opcode);
    exit(-1);
  } break;
  }
}