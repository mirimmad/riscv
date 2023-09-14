# riscv
`riscv` is an RISCV(RV32IM) emulator

## Installation
The emulator can be build by running:

`$./build.sh`

which uses `gcc`

## Usage
There are example binaries in the `tests` directory which are compiled using the `riscv32-unknown-elf-*` toolchain. Example usage:

`$./riscv tests/hello_world.bin 2> log.txt`

should print `Hello World`. Note the `2>`, it is used to redirect the `STDERR` to print to the file `log.txt`.

The emulator prints a log for every instruction it executes to the standard error stream.

## Environment support
The emulator provides two `syscalls`: `exit` and `write`. Here is a table that describes their usage using the `ecall` instruction.

name | syscall number (a7) | a0 | a1 | a2 | a3 | a4 | a5 | a6 |
-----|---------------------|----|----|----|----|----|----|----|
exit|93|exit code |-|-|-|-|-|-|
write|64|fd|*buf| count|-|-|-|-|
