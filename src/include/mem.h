#ifndef MEM_H
#define MEM_H

#include "common.h"

static const unsigned int RAM_SIZE = 1204 * 1024 * 2; // 2 MiB


static const unsigned int RAM_BASE = 0x0;

typedef struct {
  uint8_t *mem;
} RAM;

RAM *newRAM(uint8_t *, size_t);
void freeRAM(RAM *);
void RAM_test();

uint32_t ram_load(RAM *mem, uint32_t addr, uint8_t size);

void ram_store(RAM *mem, uint32_t addr, uint32_t value, uint8_t size);


#endif
