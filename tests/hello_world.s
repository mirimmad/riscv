.global _start
_start:
    addi a7, zero, 64 # syscall 64 = write
    addi a0, zero, 1 # fd = stdout
    la a1, .helloworld
    addi a2, zero, 13 # count = 13
    ecall

    addi a7, zero, 93 # syscall 93 = exit
    addi a0, zero, 42
    ecall

.helloworld:
    .ascii "Hello World!\n"
