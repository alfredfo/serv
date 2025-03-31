	.section .text
	.global _start
	.global handler
_start:
	la t0, handler
	csrw mtvec, t0
	li t0, 0x80000000
	li t1, 3000
	sw t1, 0(t0)
	li t0, 0x8
	csrrs x0, mstatus, t0
	li t0, 0x80
	csrrs x0, mie, t0
        addi t5, x0, 1111
        wfi
loop:
        addi t5, x0, 1337
        j loop

handler:
	li t0, 0x8
	csrrc x0, mstatus, t0
	li t0, 0x80
	csrrc x0, mie, t0
	mret
