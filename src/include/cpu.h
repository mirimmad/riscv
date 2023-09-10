#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "mem.h"

typedef struct {
  uint64_t regs[32];
  size_t pc;
  RAM *ram;
} CPU;

CPU *newCPU(uint8_t *code, size_t len);
void cpu_state(CPU *);

#endif