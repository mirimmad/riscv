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

uint32_t cpu_load(CPU *cpu, uint32_t addr, uint8_t size) {
  return ram_load(cpu->ram, addr, size);
}
void cpu_store(CPU *cpu, uint32_t addr, uint32_t value, uint8_t size) {
  ram_store(cpu->ram, addr, value, size);
}

uint32_t cpu_fetch(CPU *cpu) { return cpu_load(cpu, cpu->pc, 32); }

void cpu_decode(uint32_t inst_raw, INST *inst) {
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

static inline int32_t i_imm(uint32_t inst_raw) {
  // imm[11:0] = inst[31:20]
  return ((int32_t)inst_raw) >> 20;
}

static inline int32_t s_imm(uint32_t inst_raw) {
  // imm[11:5 | 4:0] = inst[31:25 | 11:7]
  return ((int32_t)(inst_raw & 0xFE000000)) >> 20 |
         ((int32_t)(inst_raw >> 7) & 0x1F);
}

static inline int32_t u_imm(uint32_t inst_raw) {
  return ((int32_t) inst_raw & 0xFFFFF000);
}

static inline int32_t logical_right_shift(int32_t x, int32_t n) {
  int size = sizeof(int) * 8;
  return (x >> n) & ~(((0x1 << size) >> n) << 1);
}

static inline uint8_t match_funct3_funct7(INST *inst, uint8_t funct3,
                                          uint8_t funct7) {
  return (inst->funct3 == funct3 && inst->funct7 == funct7);
}

INST inst;
void cpu_execute(CPU *cpu, uint32_t inst_raw) {
  cpu_decode(inst_raw, &inst);
  cpu->regs[0] = 0;
  switch (inst.opcode) {

  case LOAD: {
    int32_t imm = i_imm(inst_raw);
    uint32_t addr = cpu->regs[inst.rs1] + imm;
    uint32_t val;
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
      fatal("Illegal FUNCT3 for LOAD: %d\n", inst.funct3);
      exit(-1);
    }
    }

    log("LOAD %d to reg r(%d) from addr 0x%x\n", val, inst.rd, addr);
  } break;

  case STORE: {
    int32_t imm = s_imm(inst_raw);
    uint32_t addr = cpu->regs[inst.rs1] + imm;
    uint32_t val = cpu->regs[inst.rs2];
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
    default: {
      fatal("Illegal FUNCT3 for STORE: %d\n", inst.funct3);
    }
    }
    log("STORE r(%d)=%d at 0x%x\n", inst.rs2, val, addr);
  } break;

  case INTEGER_COMP_RI: {
    int32_t imm = i_imm(inst_raw);
    // The shift ammout is encoded in lower 5 bits if imm field
    uint32_t shtamt = imm & SHAMT_MASK;
    switch (inst.funct3) {
    case ADDI: {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] + imm;
      log_RI("ADDI");
    } break;

    case SLLI: {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] << shtamt;
      log_RI("SLLI")
    } break;

    case SLTI: {
      int c = ((int32_t)cpu->regs[inst.rs1]) < ((int32_t)imm);
      cpu->regs[inst.rd] = c;
      log_RI("SLTI");
    } break;

    case SLTIU: {
      int c = cpu->regs[inst.rs1] < imm;
      cpu->regs[inst.rd] = c;
      log_RI("SLTIU");
    } break;

    case XORI: {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] ^ imm;
      log_RI("XORI");
    } break;

    case RIGHT_SHIFT: {
      switch (inst.funct7) {
      case 0x0: {
        // SRLI: Right-shift logical
        cpu->regs[inst.rd] = cpu->regs[inst.rs1] >> shtamt;
        log_RI("SRLI");
      } break;
      case 0x20: {
        // SRAI: Right-shift arithmetic
        cpu->regs[inst.rd] = logical_right_shift(cpu->regs[inst.rs1], shtamt);
        log_RI("SRAI");
      } break;
      }
    } break;

    case ORI: {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] | imm;
      log_RI("ORI");
    } break;

    case ANDI: {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] & imm;
      log_RI("ANDI");
    } break;

    default: {
      fatal("Unknown FUNCT3 for INTEGER_COMP_RI");
    }
    }

  } break;

  case INTEGER_COMP_RR: {
    // The shift ammount is stored in lower 5 bit of rs2
    uint32_t shtamt = cpu->regs[inst.rs2] & SHAMT_MASK;

    if (match_funct3_funct7(&inst, 0x0, 0x0)) {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] + cpu->regs[inst.rs2];
      log_RR("ADD");

    } else if (match_funct3_funct7(&inst, 0x0, 0x01)) {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] * cpu->regs[inst.rs2];
      log_RR("MUL");

      // TODO: Implement rest of M extension
    } else if (match_funct3_funct7(&inst, 0x0, 0x20)) {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] - cpu->regs[inst.rs2];
      log_RR("SUB");

    } else if (match_funct3_funct7(&inst, 0x1, 0x0)) {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] << cpu->regs[inst.rs2];
      log_RR("SLL");
      
    } else if (match_funct3_funct7(&inst, 0x2, 0x0)) {
      int c =
          ((int32_t)cpu->regs[inst.rs1]) < ((int32_t)cpu->regs[inst.rs2]);
      cpu->regs[inst.rd] = c;
      log_RR("SLT");

    } else if (match_funct3_funct7(&inst, 0x3, 0x0)) {
      int c = cpu->regs[inst.rs1] < cpu->regs[inst.rs2];
      cpu->regs[inst.rd] = c;
      log_RR("SLTU");

    } else if (match_funct3_funct7(&inst, 0x4, 0x0)) {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] ^ cpu->regs[inst.rs2];
      log_RR("XOR");

    } else if (match_funct3_funct7(&inst, 0x5, 0x0)) {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] >> cpu->regs[inst.rs2];
      log_RR("SRL");

    } else if (match_funct3_funct7(&inst, 0x5, 0x20)) {
      cpu->regs[inst.rd] =
          logical_right_shift(cpu->regs[inst.rs1], cpu->regs[inst.rs2]);
      log_RR("SRA");

    } else if (match_funct3_funct7(&inst, 0x6, 0x0)) {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] | cpu->regs[inst.rs2];
      log_RR("OR");

    } else if (match_funct3_funct7(&inst, 0x7, 0x0)) {
      cpu->regs[inst.rd] = cpu->regs[inst.rs1] & cpu->regs[inst.rs2];
      log_RR("AND");

    } else {
      fatal("Unkown funct3(%x) & funct7(%x) for INTEGER_COMP_RR\n", inst.funct3,
            inst.funct7);
    }
  } break;

  case LUI: {
    int32_t imm = u_imm(inst_raw);
    cpu->regs[inst.rd] = imm;
    log_RI("LUI");
  } break;

  case AUIPC: {
    int32_t imm = u_imm(inst_raw);
    cpu->regs[inst.rd] = (cpu->pc - 4) + imm;
    log_RI("AUIPC");
  } break;

  default: {
    fatal("Illegal Instruction 0x%x\n", inst.opcode);
  } break;
  }
}