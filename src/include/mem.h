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
static uint32_t ram_load8(RAM *, uint32_t);
static uint32_t ram_load16(RAM *, uint32_t);
static uint32_t ram_load32(RAM *, uint32_t);

void ram_store(RAM *mem, uint32_t addr, uint32_t value, uint8_t size);
static void ram_store8(RAM *, uint32_t, uint32_t);
static void ram_store16(RAM *, uint32_t, uint32_t);
static void ram_store32(RAM *, uint32_t, uint32_t);

#endif
