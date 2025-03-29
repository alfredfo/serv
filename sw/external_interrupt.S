	.section .text
	.global _start
	.global handler

#define GPIO_BASE 0x40000000

_start:
	la t0, handler
	csrw mtvec, t0
	li t0, 0x8
	csrrs x0, mstatus, t0
	li t0, 0x800
	csrrs x0, mie, t0


	/* Load GPIO base address to a0 */
	lui a0, %hi(GPIO_BASE)
	addi a0, a0, %lo(GPIO_BASE)

	/* Clear t0 */
	addi	t0, zero, 0
	sb t0, 0(a0)

wait:


	/* Write to LEDs */
//	sb t0, 0(a0)
	/* invert LED */
//	xori t0, t0, 1

	wfi

	j wait // 40



handler:

	addi	t0, zero, 1
	sb t0, 0(a0)
	addi	t0, zero, 0
	sb t0, 0(a0)

	mret
