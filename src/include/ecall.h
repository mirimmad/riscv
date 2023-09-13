#ifndef ECALL_H
#define ECALL_H

#include "common.h"
#include "cpu.h"

#define SYSCALL_WRITE 64
#define SYSCALL_EXIT 93
#define STDOUT 1
#define STDERR 2

void ecall_handler(CPU *);

#endif