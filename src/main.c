#include "include/common.h"
#include "include/cpu.h"
#include "include/mem.h"

int main(int argc, char *argv[]) {
  // RAM_test();

  if (argc < 2) {
    printf("Usage: riscv [filename]\n");
    exit(-1);
  }

  const char *filename = argv[1];
  FILE *file = fopen(filename, "rb");
  fseek(file, 0L, SEEK_END);
  size_t codeSize = ftell(file);
  rewind(file);

  uint8_t *code = (uint8_t *)malloc(codeSize);
  fread(code, sizeof(uint8_t), codeSize, file);
  CPU *cpu = newCPU(code, codeSize);

  while (1) {
    uint32_t inst_raw = cpu_fetch(cpu);
    cpu->pc += 4;
    if (cpu->pc > codeSize) break;
    cpu_execute(cpu, inst_raw);

    if (cpu->pc == 0) break;
  }

  freeCPU(cpu);
}