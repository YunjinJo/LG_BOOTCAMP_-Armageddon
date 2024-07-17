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
	.extern current_pcb
	.extern IRQ_Context_Switch
	.global Save_Context
Save_Context:

	@IRQ mode ���
	push	{r0, r14}

	@������� ����Ǵ� app�� pcb �ּ� ������ ����
	ldr		r14, =current_pcb

	@usr/sys ��� r0-r14���� ���
	stmia	r14, {r0-r14}^

	@������� ����Ǵ� app�� cpsr ���
	@mrs		r0, spsr
	@str		r0, [r14, #64]

	@IRQ mode ����
	pop		{r0, r14} @���� r14 -> ������ �ּ�

	b		IRQ_Context_Switch

@ �ٸ� ���μ����� ���¸� �����ϴ� �Լ�
	.extern current_pcb
	.global Restore_Context_And_Switch
Restore_Context_And_Switch:
    @ usr/sys r0-r14 ����
	ldr		r14, =current_pcb
	ldmia	r14, {r0-r14}^

	@r0�� ����ϱ� ���� �̸� ���
	push	{r0-r1}

	@spsr ����
	@ldr		r0, [r0]
	@msr		spsr, r0

	@ IRQ lr ����
	ldr		r14, [r14, #56]

	@r0 ����
	pop		{r0-r1}

	@ ���� ������ ��ɸ� pc�� ����
	subs	pc, lr, #4



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

	.end
