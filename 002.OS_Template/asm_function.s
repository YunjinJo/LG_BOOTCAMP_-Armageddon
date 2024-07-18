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

@ ���� ���μ����� ���¸� �����ϴ� �Լ�
	.global save_context
save_context:
    @ ���ÿ� �������͸� ����
    stmfd   sp!, {r0-r12, lr}
    mrs     r0, cpsr
    stmfd   sp!, {r0}
    @ ���� ���� �����͸� ����
    b Get_User_SP
    str     sp, [r0]
    bx      lr

@ �ٸ� ���μ����� ���¸� �����ϴ� �Լ�
	.global restore_context
restore_context:
    @ ���ο� ���� �����͸� �ε�
    ldr     r0, =SYS_STACK_BASE
    ldr     sp, [r0]
    @ CPSR�� ����
    ldmfd   sp!, {r0}
    msr     cpsr, r0
    @ �������͸� ����
    ldmfd   sp!, {r0-r12, pc}


	.global set_ttbr_app_0
set_ttbr_app_0:
    mrc p15, 0, r0, c1, c0, 0   // Control Register �б�
    bic r0, r0, #1              // MMU ��Ȱ��ȭ (��Ʈ 0 Ŭ����)
    mcr p15, 0, r0, c1, c0, 0   // Control Register ����

	mrc p15, 0, r0, c2, c0, 0
	ldr r1, =0x44000000
	orr r0, r0, r1
	mcr p15, 0, r0, c2, c0, 0

	mov r0, #0
	mcr p15, 0, r0, C8, C7, 2
	mrc p15, 0, r0, c2, c0, 0

    // MMU ��Ȱ��ȭ
    mrc p15, 0, r0, c1, c0, 0   // Control Register �б�
    orr r0, r0, #1              // MMU Ȱ��ȭ (��Ʈ 0 ����)
    mcr p15, 0, r0, c1, c0, 0   // Control Register ����

    mrc p15, 0, r0, c2, c0, 0

	bx lr


	.global set_ttbr_app_1
set_ttbr_app_1:

	mrc p15, 0, r0, c1, c0, 0   // Control Register �б�
    bic r0, r0, #1              // MMU ��Ȱ��ȭ (��Ʈ 0 Ŭ����)
    mcr p15, 0, r0, c1, c0, 0   // Control Register ����

	mrc p15, 0, r0, c2, c0, 0
	ldr r1, =0x44004000
	orr r0, r0, r1
	mcr p15, 0, r0, c2, c0, 0

	mov r0, #0
	mcr p15, 0, r0, C8, C7, 2
	mrc p15, 0, r0, c2, c0, 0

    // MMU ��Ȱ��ȭ
    mrc p15, 0, r0, c1, c0, 0   // Control Register �б�
    orr r0, r0, #1              // MMU Ȱ��ȭ (��Ʈ 0 ����)
    mcr p15, 0, r0, c1, c0, 0   // Control Register ����

    mrc p15, 0, r0, c2, c0, 0

	bx lr



@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int PABT_Falut_Status(void)
@ ��ɾ� Fault �߻� ������ ����
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
	sub lr, lr, #4
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



@ ////////
	.extern Key4_ISR
	.global context_save_1
context_save_1:
	pop {r0-r3,r12}
	@ldr r14, =reg_info_app1
	ldr r14, [r14, #0]
	add r14, r14, #8
	stmia r14, {r0-r14}^

	sub r1, r14, #8
	pop {r14}
	sub lr, lr, #4
	str r14, [r1]

	mrs r0, spsr
	str r0, [r1, #4]

	b Key4_ISR

	.global Get_Context_And_Switch_1
Get_Context_And_Switch_1:
	@ldr r14, =reg_info_app0
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

.end
