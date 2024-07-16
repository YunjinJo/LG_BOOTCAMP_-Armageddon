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

	.global switch_app
switch_app:
	cps		#0x1f
	mov sp, r1
	blx		r0

	.global get_ASID
get_ASID:
	mrc p15, 0, r0, c13, c0, 1
	bx lr


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

@ 현재 프로세스의 상태를 저장하는 함수
	.global save_context
save_context:
    @ 스택에 레지스터를 저장
    stmfd   sp!, {r0-r12, lr}
    mrs     r0, cpsr
    stmfd   sp!, {r0}
    @ 현재 스택 포인터를 저장
    b Get_User_SP
    str     sp, [r0]
    bx      lr

@ 다른 프로세스의 상태를 복원하는 함수
	.global restore_context
restore_context:
    @ 새로운 스택 포인터를 로드
    ldr     r0, =SYS_STACK_BASE
    ldr     sp, [r0]
    @ CPSR을 복원
    ldmfd   sp!, {r0}
    msr     cpsr, r0
    @ 레지스터를 복원
    ldmfd   sp!, {r0-r12, pc}


	.global set_ttbr_app_0
set_ttbr_app_0:
    mrc p15, 0, r0, c1, c0, 0   // Control Register 읽기
    bic r0, r0, #1              // MMU 비활성화 (비트 0 클리어)
    mcr p15, 0, r0, c1, c0, 0   // Control Register 쓰기

	mrc p15, 0, r0, c2, c0, 0
	ldr r1, =0x44050000
	orr r0, r0, r1
	mcr p15, 0, r0, c2, c0, 0

	mov r0, #0
	mcr p15, 0, r0, C8, C7, 2
	mrc p15, 0, r0, c2, c0, 0

    // MMU 재활성화
    mrc p15, 0, r0, c1, c0, 0   // Control Register 읽기
    orr r0, r0, #1              // MMU 활성화 (비트 0 설정)
    mcr p15, 0, r0, c1, c0, 0   // Control Register 쓰기

    mrc p15, 0, r0, c2, c0, 0

	bx lr


	.global set_ttbr_app_1
set_ttbr_app_1:
    mrc p15, 0, r0, c1, c0, 0   // Control Register 읽기
    bic r0, r0, #1              // MMU 비활성화 (비트 0 클리어)
    mcr p15, 0, r0, c1, c0, 0   // Control Register 쓰기

	mrc p15, 0, r0, c2, c0, 0
	ldr r1, =0x44060000
	orr r0, r0, r1
	mcr p15, 0, r0, c2, c0, 0

	mov r0, #0
	mcr p15, 0, r0, C8, C7, 2
	mrc p15, 0, r0, c2, c0, 0

    // MMU 재활성화
    mrc p15, 0, r0, c1, c0, 0   // Control Register 읽기
    orr r0, r0, #1              // MMU 활성화 (비트 0 설정)
    mcr p15, 0, r0, c1, c0, 0   // Control Register 쓰기

    mrc p15, 0, r0, c2, c0, 0

	bx lr



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

	.end
