#include "include/cpu.h"

CPU *newCPU(uint8_t *code, size_t len) {
  CPU *cpu = (CPU *)malloc(sizeof(CPU));
  cpu->regs[0] = 0;
  // Stack pointer
  cpu->regs[2] = RAM_BASE + RAM_SIZE;

  cpu->ram = newRAM(code, len);
  return cpu;
}

void cpu_state(CPU *cpu) {
  for (int i = 0; i < 32; i++) {
    printf("r[%d]: %ld\n", i, cpu->regs[i]);
  }
}
