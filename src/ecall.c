#include "include/ecall.h"

void ecall_handler(CPU *cpu) {
  uint32_t syscall_nr = cpu->regs[17];

  switch (syscall_nr) {
  case SYSCALL_WRITE: {
    uint32_t fd = cpu->regs[10];
    uint32_t addr = cpu->regs[11];
    uint32_t count = cpu->regs[12];
    log("ECALL: Writing to file-descriptor %d %d bytes.\n", fd, count);
    FILE *stream;

    switch (fd) {
    case STDOUT:
      stream = stdout;
      break;
    case STDERR:
      stream = stderr;
      break;
    default:
      fatal("invalid file-descriptor %d.\n", fd);
    }

    for (uint32_t i = 0; i < count; i++) {
      fprintf(stream, "%c", cpu_load(cpu, addr + i, 8));
    }
  } break;

  case SYSCALL_EXIT: {
    int32_t exit_code = cpu->regs[10];
    log("ECALL: Exiting with with exit code %d.\n", exit_code);
    exit(exit_code);
  } break;

  default: {
    fatal("unkown syscall number %d.\n", syscall_nr);
  }
  }
}