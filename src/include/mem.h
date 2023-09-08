#ifndef MEM_H
#define MEM_H

#include "common.h"

static const unsigned int RAM_SIZE = 1204 * 1024 * 2; // 2 MiB
static const unsigned int RAM_BASE = 0x80000000;

typedef struct {
  uint8_t * mem;   
} RAM;

RAM * newRAM(uint8_t *, size_t);
void RAM_test();

uint64_t ram_load(RAM * mem, uint64_t addr, uint8_t size);
uint64_t ram_load8(RAM *, uint64_t);
uint64_t ram_load16(RAM *, uint64_t);
uint64_t ram_load32(RAM *, uint64_t);
uint64_t ram_load64(RAM *, uint64_t);

void ram_store(RAM * mem, uint64_t addr, uint64_t value, uint8_t size);
void ram_store8(RAM *, uint64_t, uint64_t);
void ram_store16(RAM *, uint64_t, uint64_t);
void ram_store32(RAM *, uint64_t, uint64_t);
void ram_store64(RAM *, uint64_t, uint64_t);

#endif
