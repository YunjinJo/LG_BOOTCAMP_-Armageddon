	.include "4412addr.inc"
	.include "option.inc"

	.code	32
	.text

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ UTILITY FUNCTIONS
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global Run_App
Run_App:

	push	{r4, lr}
	mrs		r4, cpsr
	cps		#0x1f
	mov 	sp, r1
	blx		r0
	msr		cpsr_cxsf, r4
	pop		{r4, pc}

	.global Get_User_SP
Get_User_SP:

	mrs		r1, cpsr
	cps		#0x1f
	mov 	r0, sp
	msr		cpsr_cxsf, r1
	bx 		lr

	.global Get_User_Stack_Base
Get_User_Stack_Base:

	ldr		r0, =SYS_STACK_BASE
	bx 		lr

	.global Get_User_Stack_Limit
Get_User_Stack_Limit:

	ldr		r0, =STACK_LIMIT
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int PABT_Falut_Status(void)
@ 명령어 Fault 발생 이유를 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  PABT_Falut_Status
PABT_Falut_Status:

	mrc		p15, 0, r0, c5, c0, 1
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int PABT_Falut_Address(void)
@ 데이터 Fault 발생 주소를 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  PABT_Falut_Address
PABT_Falut_Address:

	mrc		p15, 0, r0, c6, c0, 2
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int DABT_Falut_Status(void)
@ 데이터 Fault 발생 이유를 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  DABT_Falut_Status
DABT_Falut_Status:

	mrc		p15, 0, r0, c5, c0, 0
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int DABT_Falut_Address(void)
@ 데이터 Fault 발생 주소를 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  DABT_Falut_Address
DABT_Falut_Address:

	mrc		p15, 0, r0, c6, c0, 0
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Get_SP(void)
@ 현재 모드의 스택 포인터 값을 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global Get_SP
Get_SP:

	mov 	r0, sp
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Main_ID(void)
@ Core의 버전 값을 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global Main_ID
Main_ID:

	mrc 	p15, 0, r0, c0, c0, 0
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int TLB_Type(void)
@ TLB Type 값을 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global TLB_Type
TLB_Type:

	mrc 	p15, 0, r0, c0, c0, 3
	bx 		lr



@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Get_CPSR(void);
@ CPSR 값을 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.global Get_CPSR
Get_CPSR:
	mrs		r0, cpsr
	bx 		lr

	.extern reg_info_app0
	.extern Key3_ISR
	.global context_save
context_save:
	push {r0, r14}
	ldr r14, =reg_info_app0
	ldr r14, [r14, #0]

	add r14, r14, #8
	stmia r14, {r0-r14}^
	sub r14, r14, #8

	mov r1, r14

	pop {r0, r14}

	sub r0, r14, #4
	str r0, [r1]

	mrs r0, spsr
	str r0, [r1, #4]

	b Key3_ISR
	.extern reg_info_app1
	.global Get_Context_And_Switch
Get_Context_And_Switch:
	ldr r14, =reg_info_app1
	ldr r14, [r14, #0]

	add r14, r14, #8
	ldmia r14, {r0-r14}^
	sub r14, r14, #8

	push	{r0-r1}
	add r0, r14, #4
	ldr r0, [r0]
	msr spsr, r0

	pop {r0-r1}
	ldr r14, [r14]
	subs pc, r14, #4

.end
