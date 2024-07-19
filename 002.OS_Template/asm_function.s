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
@ ���ɾ� Fault �߻� ������ ����
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  PABT_Falut_Status
PABT_Falut_Status:

	mrc		p15, 0, r0, c5, c0, 1
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int PABT_Falut_Address(void)
@ ������ Fault �߻� �ּҸ� ����
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  PABT_Falut_Address
PABT_Falut_Address:

	mrc		p15, 0, r0, c6, c0, 2
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int DABT_Falut_Status(void)
@ ������ Fault �߻� ������ ����
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  DABT_Falut_Status
DABT_Falut_Status:

	mrc		p15, 0, r0, c5, c0, 0
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int DABT_Falut_Address(void)
@ ������ Fault �߻� �ּҸ� ����
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  DABT_Falut_Address
DABT_Falut_Address:

	mrc		p15, 0, r0, c6, c0, 0
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Get_SP(void)
@ ���� ����� ���� ������ ���� ����
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global Get_SP
Get_SP:

	mov 	r0, sp
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Main_ID(void)
@ Core�� ���� ���� ����
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global Main_ID
Main_ID:

	mrc 	p15, 0, r0, c0, c0, 0
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int TLB_Type(void)
@ TLB Type ���� ����
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global TLB_Type
TLB_Type:

	mrc 	p15, 0, r0, c0, c0, 3
	bx 		lr

	.global Get_ASID
Get_ASID:
	MRC    p15,0,r0,c13,c0,1
	bx lr

	.global Get_CPSR
Get_CPSR:
	mrs		r0, cpsr
	bx 		lr

	.extern sel_reg_info
	.extern Timer0_ISR
	.global context_save
context_save:
	pop {r0-r3,r12}
	ldr r14, =sel_reg_info
	ldr r14, [r14, #0]
	add r14, r14, #8
	stmia r14, {r0-r14}^

	sub r1, r14, #8
	pop {r14}
	str r14, [r1]

	mrs r0, spsr
	str r0, [r1, #4]

	b Timer0_ISR
	.extern reg_info_app1
	.global Get_Context_And_Switch
Get_Context_And_Switch:
	ldr r14, =sel_reg_info
	ldr r14, [r14, #0]

	add r14, r14, #8
	ldmia r14, {r0-r14}^
	sub r14, r14, #8

	push	{r0-r1}
	add r0, r14, #4
	ldr r0, [r0]
	msr spsr_csfx, r0

	pop {r0-r1}
	ldr r14, [r14]
	movs pc, r14

	.extern _Print_Test_SVC
	.global print_test
@print_test:
	@push {r4, r5, r6, lr}
	@ 사용자 모드의 스택 포인터를 가져오기 위해 모드 전환
    @mrs r4, cpsr          @ 현재 프로그램 상태 레지스터를 r0에 저장
    @bic r5, r4, #0x1F     @ 모드 비트를 클리어
    @orr r5, r5, #0x1f     @ SYS 모드로 설정
    @msr cpsr_cxsf, r5        @ SYS 모드로 전환
    @mov r6, sp            @ 사용자 모드의 스택 포인터를 r2에 저장
    @msr cpsr_cxsf, r4        @ 원래 모드로 복귀

    @ SYS 모드의 스택에서 구조체 데이터를 불러오기
    @ldm r6, {r4, r5}  @ stack에서 불러오기
	@push {r4, r5}
	@bl _Print_Test_SVC
	@add sp, sp, #8
	@pop {r4, r5, r6, pc}

.end
