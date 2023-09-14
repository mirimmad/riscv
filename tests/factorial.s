	.file	"factorial.c"
	.option nopic
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	add	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	add	s0,sp,16
	li	a0,10
	call	factorial
	mv	a5,a0
	mv	a0,a5
	lw	ra,12(sp)
	lw	s0,8(sp)
	add	sp,sp,16
	jr	ra
	.size	main, .-main
	.align	2
	.globl	factorial
	.type	factorial, @function
factorial:
	add	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	add	s0,sp,32
	sw	a0,-20(s0)
	lw	a4,-20(s0)
	li	a5,1
	bne	a4,a5,.L4
	lw	a5,-20(s0)
	j	.L5
.L4:
	lw	a5,-20(s0)
	add	a5,a5,-1
	mv	a0,a5
	call	factorial
	mv	a4,a0
	lw	a5,-20(s0)
	mul	a5,a4,a5
.L5:
	mv	a0,a5
	lw	ra,28(sp)
	lw	s0,24(sp)
	add	sp,sp,32
	jr	ra
	.size	factorial, .-factorial
	.ident	"GCC: (GNU) 7.3.0"
