	.include "4412addr.inc"
	.include "option.inc"

	.code	32
	.text
 
	.global	__start
__start:

	push {r0-r3, r12, lr}

	.extern __ZI_BASE__
	.extern __ZI_LIMIT__
	.extern Main

	ldr		r0, =__ZI_BASE__
	ldr		r1, =__ZI_LIMIT__
	mov		r2, #0x0
1:
	cmp		r0, r1
	strlo	r2, [r0], #4
	blo		1b

	bl		Main

	pop {r0-r3, r12, pc}

	.end
