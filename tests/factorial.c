/*
/home/mir/github/riscv32-toolchain/riscv32-unknown-elf-gcc/bin/riscv32-unknown-elf-as -march=rv32i factorial.c
/home/mir/github/riscv32-toolchain/riscv32-unknown-elf-gcc/bin/riscv32-unknown-elf-ld a.out -o facto.bin
/home/mir/github/riscv32-toolchain/riscv32-unknown-elf-gcc/bin/riscv32-unknown-elf-objcopy -O binary --only-section=.text facto.bin $@

*/
int factorial(int n);
int main() {
  return factorial(10);
}

int factorial(int n) {
  if (n == 1) return n;
  return n * factorial(n - 1);
}
