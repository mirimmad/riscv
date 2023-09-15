#include "include/mem.h"
#include "include/common.h"

RAM *newRAM(uint8_t *code, size_t len) {
  RAM *ram = (RAM *)malloc(sizeof(RAM));
  ram->mem = (uint8_t *)malloc(RAM_SIZE);
  memcpy(ram->mem, code, len);
  return ram;
}

void freeRAM(RAM *mem) {
  free(mem->mem);
  free(mem);
}

static uint32_t ram_load8(RAM *, uint32_t);
static uint32_t ram_load16(RAM *, uint32_t);
static uint32_t ram_load32(RAM *, uint32_t);
static void ram_store8(RAM *, uint32_t, uint32_t);
static void ram_store16(RAM *, uint32_t, uint32_t);
static void ram_store32(RAM *, uint32_t, uint32_t);

uint32_t ram_load(RAM *mem, uint32_t addr, uint8_t size) {
  uint32_t r = 0;
  switch (size) {
  case 8:
    r = ram_load8(mem, addr);
    break;
  case 16:
    r = ram_load16(mem, addr);
    break;
  case 32:
    r = ram_load32(mem, addr);
    break;
  default: {
    fatal("RAM: invalid load size(%d)\n", size);
  }
  }

  return r;
}

void ram_store(RAM *mem, uint32_t addr, uint32_t value, uint8_t size) {
  switch (size) {
  case 8:
    ram_store8(mem, addr, value);
    break;
  case 16:
    ram_store16(mem, addr, value);
    break;
  case 32:
    ram_store32(mem, addr, value);
    break;
  default: {
    fatal("RAM: invalid store size(%d)\n", size);
  }
  }
}

static uint32_t ram_load8(RAM *mem, uint32_t addr) {
  size_t index = addr - RAM_BASE;
  return (uint32_t)mem->mem[index];
}

static uint32_t ram_load16(RAM *mem, uint32_t addr) {
  size_t index = addr - RAM_BASE;
  return ((uint32_t)mem->mem[index] | ((uint32_t)mem->mem[index + 1] << 8));
}

static uint32_t ram_load32(RAM *mem, uint32_t addr) {
  size_t index = addr - RAM_BASE;
  return ((uint32_t)mem->mem[index] | ((uint32_t)mem->mem[index + 1] << 8) |
          ((uint32_t)mem->mem[index + 2] << 16) |
          ((uint32_t)mem->mem[index + 3] << 24));
}

static void ram_store8(RAM *mem, uint32_t addr, uint32_t value) {
  size_t index = addr - RAM_BASE;
  mem->mem[index] = (uint8_t)value;
}

static void ram_store16(RAM *mem, uint32_t addr, uint32_t value) {
  size_t index = addr - RAM_BASE;
  mem->mem[index] = (uint8_t)value & 0xFF;
  mem->mem[index + 1] = (uint8_t)(value >> 8) & 0xFF;
}

static void ram_store32(RAM *mem, uint32_t addr, uint32_t value) {
  size_t index = addr - RAM_BASE;
  mem->mem[index] = (uint8_t)value & 0xFF;
  mem->mem[index + 1] = (uint8_t)(value >> 8) & 0xFF;
  mem->mem[index + 2] = (uint8_t)(value >> 16) & 0xFF;
  mem->mem[index + 3] = (uint8_t)(value >> 24) & 0xFF;
}


void RAM_test() {
  uint8_t c[] = {0x1};
  RAM *mem = newRAM(c, 1);
  ram_store(mem, RAM_BASE + 1, 10, 8);
  uint32_t v8 = ram_load(mem, RAM_BASE + 1, 8);
  assert(v8 == 10);

  ram_store(mem, RAM_BASE + 2, 123456789, 32);
  uint32_t v32 = ram_load(mem, RAM_BASE + 2, 32);
  assert(v32 == 123456789);

  freeRAM(mem);

  printf("===== RAM tests passed ====\n");
}
