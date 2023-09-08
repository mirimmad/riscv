#include "include/mem.h"
#include "include/common.h"

void fatal(const char *msg) {
  printf("[Fatal] %s\n", msg);
  exit(-1);
}

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

uint64_t ram_load(RAM *mem, uint64_t addr, uint8_t size) {
  uint64_t r = 0;
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
  case 64:
    r = ram_load64(mem, addr);
    break;
  default: {
    char str[50];
    sprintf(str, "RAM: invalid load size(%d)", size);
    fatal(str);
  }
  }

  return r;
}

void ram_store(RAM *mem, uint64_t addr, uint64_t value, uint8_t size) {
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
  case 64:
    ram_store64(mem, addr, value);
    break;
  default: {
    char str[50];
    sprintf(str, "RAM: invalid store size(%d)", size);
    fatal(str);
  }
  }
}

uint64_t ram_load8(RAM *mem, uint64_t addr) {
  size_t index = addr - RAM_BASE;
  return (uint64_t)mem->mem[index];
}

uint64_t ram_load16(RAM *mem, uint64_t addr) {
  size_t index = addr - RAM_BASE;
  return ((uint64_t)mem->mem[index] | ((uint64_t)mem->mem[index + 1] << 8));
}

uint64_t ram_load32(RAM *mem, uint64_t addr) {
  size_t index = addr - RAM_BASE;
  return ((uint64_t)mem->mem[index] | ((uint64_t)mem->mem[index + 1] << 8) |
          ((uint64_t)mem->mem[index + 2] << 16) |
          ((uint64_t)mem->mem[index + 3] << 24));
}

uint64_t ram_load64(RAM *mem, uint64_t addr) {
  size_t index = addr - RAM_BASE;
  return ((uint64_t)mem->mem[index] | ((uint64_t)mem->mem[index + 1] << 8) |
          ((uint64_t)mem->mem[index + 2] << 16) |
          ((uint64_t)mem->mem[index + 3] << 24) |
          ((uint64_t)mem->mem[index + 4] << 32) |
          ((uint64_t)mem->mem[index + 5] << 40) |
          ((uint64_t)mem->mem[index + 6] << 48) |
          ((uint64_t)mem->mem[index + 7] << 56));
}

void ram_store8(RAM *mem, uint64_t addr, uint64_t value) {
  size_t index = addr - RAM_BASE;
  mem->mem[index] = (uint8_t)value;
}

void ram_store16(RAM *mem, uint64_t addr, uint64_t value) {
  size_t index = addr - RAM_BASE;
  mem->mem[index] = (uint8_t)value & 0xFF;
  mem->mem[index + 1] = (uint8_t)(value >> 8) & 0xFF;
}

void ram_store32(RAM *mem, uint64_t addr, uint64_t value) {
  size_t index = addr - RAM_BASE;
  mem->mem[index] = (uint8_t)value & 0xFF;
  mem->mem[index + 1] = (uint8_t)(value >> 8) & 0xFF;
  mem->mem[index + 2] = (uint8_t)(value >> 16) & 0xFF;
  mem->mem[index + 3] = (uint8_t)(value >> 24) & 0xFF;
}

void ram_store64(RAM *mem, uint64_t addr, uint64_t value) {
  size_t index = addr - RAM_BASE;
  mem->mem[index] = (uint8_t)value & 0xFF;
  mem->mem[index + 1] = (uint8_t)(value >> 8) & 0xFF;
  mem->mem[index + 2] = (uint8_t)(value >> 16) & 0xFF;
  mem->mem[index + 3] = (uint8_t)(value >> 24) & 0xFF;
  mem->mem[index + 4] = (uint8_t)(value >> 32) & 0xFF;
  mem->mem[index + 5] = (uint8_t)(value >> 40) & 0xFF;
  mem->mem[index + 6] = (uint8_t)(value >> 48) & 0xFF;
  mem->mem[index + 7] = (uint8_t)(value >> 56) & 0xFF;
}

void RAM_test() {
  uint8_t c[] = {0x1};
  RAM *mem = newRAM(c, 1);
  ram_store(mem, RAM_BASE + 1, 10, 8);
  uint64_t v8 = ram_load(mem, RAM_BASE + 1, 8);
  assert(v8 == 10);

  ram_store(mem, RAM_BASE + 2, 123456789, 64);
  uint64_t v64 = ram_load(mem, RAM_BASE + 2, 64);
  assert(v64 == 123456789);

  freeRAM(mem);

  printf("===== RAM tests passed ====\n");
}
