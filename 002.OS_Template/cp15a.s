
	.global	__start
	.code	32
	.text

@ Froggy @
				.global  exynos_smc
exynos_smc:
		        dsb
		        smc     #0
		        isb
		        bx		lr

			.global  CoGetOSReadPA
CoGetOSReadPA:
                MCR     p15,0,r0,c7,c8,0
                ISB
                MRC 	p15,0,r0,C7,C4,0
                BX     lr

				.global  CoGetOSWritePA
CoGetOSWritePA:
                MCR     p15,0,r0,c7,c8,1
                ISB
                MRC 	p15,0,r0,C7,C4,0
                BX     lr

				.global  CoGetUserReadPA
CoGetUserReadPA:
                MCR     p15,0,r0,c7,c8,2
                ISB
                MRC 	p15,0,r0,C7,C4,0
                BX     lr

				.global  CoGetUserWritePA
CoGetUserWritePA:
                MCR     p15,0,r0,c7,c8,3
                ISB
                MRC 	p15,0,r0,C7,C4,0
                BX     lr

				.global  CoEnableL2PrefetchHint
CoEnableL2PrefetchHint:
                MRC     p15,0,r0,c1,c0,1
                ORR     r0,r0,#(1<<1)
                MCR     p15,0,r0,c1,c0,1
                BX     lr

				.global  CoDisableL2PrefetchHint
CoDisableL2PrefetchHint:
                MRC     p15,0,r0,c1,c0,1
                BIC     r0,r0,#(1<<1)
                MCR     p15,0,r0,c1,c0,1
                BX     lr

                .global  CoEnableICache
CoEnableICache:
                MRC     p15,0,r0,c1,c0,0
                ORR     r0,r0,#(1<<12)
                MCR     p15,0,r0,c1,c0,0
                MRC     p15,0,r0,c1,c0,1
                ORR     r0,r0,#(1<<2)
                MCR     p15,0,r0,c1,c0,1
                BX     lr

                .global  CoDisableICache
CoDisableICache:
                MRC     p15,0,r0,c1,c0,0
                BIC     r0,r0,#(1<<12)
                MCR     p15,0,r0,c1,c0,0
                MRC     p15,0,r0,c1,c0,1
                BIC     r0,r0,#(1<<2)
                MCR     p15,0,r0,c1,c0,1
                BX     lr

                .global  CoEnableDCache
CoEnableDCache:
                MRC     p15,0,r0,c1,c0,0
                ORR     r0,r0,#(1<<2)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoDisableDCache
CoDisableDCache:
                MRC     p15,0,r0,c1,c0,0
                BIC     r0,r0,#(1<<2)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoReadCTR
CoReadCTR:
                MRC     p15,0,r0,c0,c0,1
                BX     lr

                .global  CoReadCLIDR
CoReadCLIDR:
                MRC     p15,1,r0,c0,c0,1
                BX     lr

                .global  CoReadCCSIDR
CoReadCCSIDR:
                MRC     p15,1,r0,c0,c0,0
                BX     lr

                .global  CoReadCSSELR
CoReadCSSELR:
                MRC     p15,2,r0,c0,c0,0
                BX     lr

@ Froggy End @

                .global  CoEnableIrq
CoEnableIrq:
                MRS     r0,cpsr
                BIC     r0,r0,#0x80
                MSR     cpsr_cxsf,r0
                BX     lr

                .global  CoDisableIrq
CoDisableIrq:
                MRS     r0,cpsr
                ORR     r0,r0,#0x80
                MSR     cpsr_cxsf,r0
                BX     lr

                .global  CoEnableFiq
CoEnableFiq:
                MRS     r0,cpsr
                BIC     r0,r0,#0x40
                MSR     cpsr_cxsf,r0
                BX     lr

                .global  CoDisableFiq
CoDisableFiq:
                MRS     r0,cpsr
                ORR     r0,r0,#0x40
                MSR     cpsr_cxsf,r0
                BX     lr

          		.equ     NOINT, 	0xc0

                .global  CoSetIF
                @/* The return value is current CPSR. */
CoSetIF:
                MRS     r0,cpsr
                MOV     r1,r0
                ORR     r1,r1,#NOINT
                MSR     cpsr_cxsf,r1
                BX     lr

                .global  CoWrIF
                @/* r0 = uCpsrValue */
CoWrIF:
                MSR     cpsr_cxsf,r0
                BX     lr

                .global  CoClrIF
CoClrIF:
                MRS     r0,cpsr
                BIC     r0,r0,#NOINT
                MSR     cpsr_cxsf,r0
                BX     lr

                .global  CoEnableVectoredInt
CoEnableVectoredInt:
                MRC     p15,0,r0,c1,c0,0
                ORR     r0,r0,#(1<<24)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoDisableVectoredInt
CoDisableVectoredInt:
                MRC     p15,0,r0,c1,c0,0
                BIC     r0,r0,#(1<<24)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoEnableUnalignedAccess
CoEnableUnalignedAccess:
                MRC     p15,0,r0,c1,c0,0
                ORR     r0,r0,#(1<<22)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoDisableUnalignedAccess
CoDisableUnalignedAccess:
                MRC     p15,0,r0,c1,c0,0
                BIC     r0,r0,#(1<<22)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoEnableAlignFault
CoEnableAlignFault:
                MRC     p15,0,r0,c1,c0,0
                ORR     r0,r0,#(1<<1)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoDisableAlignFault
CoDisableAlignFault:
                MRC     p15,0,r0,c1,c0,0
                BIC     r0,r0,#(1<<1)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoEnableMmu
CoEnableMmu:
                MRC     p15,0,r0,c1,c0,0
                ORR     r0,r0,#(1<<0)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoDisableMmu
CoDisableMmu:
                MRC     p15,0,r0,c1,c0,0
                BIC     r0,r0,#(1<<0)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoSetFastBusMode
CoSetFastBusMode:
                MRC     p15,0,r0,c1,c0,0
                BIC     r0,r0,#(1<<30)|(1<<31)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoSetAsyncBusMode
CoSetAsyncBusMode:
                MRC     p15,0,r0,c1,c0,0
                ORR     r0,r0,#(1<<30)|(1<<31)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoEnableBranchPrediction
CoEnableBranchPrediction:
                MRC     p15,0,r0,c1,c0,0
                ORR     r0,r0,#(1<<11)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoDisableBranchPrediction
CoDisableBranchPrediction:
                MRC     p15,0,r0,c1,c0,0
                BIC     r0,r0,#(1<<11)
                MCR     p15,0,r0,c1,c0,0
                BX     lr

                .global  CoEnableVfp
CoEnableVfp:
                MRC     p15,0,r0,c1,c0,2
                ORR     r0,r0,#(3<<20)
                MCR     p15,0,r0,c1,c0,2
                BX     lr

                .global  CoEnableNeon
CoEnableNeon:
                MRC     p15,0,r0,c1,c0,2
                ORR     r0,r0,#(3<<22)
                MCR     p15,0,r0,c1,c0,2
                BX     lr

                .global  CoSetTTBase
CoSetTTBase:
                @/* r0 = base */
                MCR     p15,0,r0,c2,c0,0
                BX     lr

                .global  CoSelTTBReg0
CoSelTTBReg0:
                @/* r0 = base */
                MOV     r0,#0
                MCR     p15,0,r0,c2,c0,2
                BX     lr

                .global  CoSetASID
CoSetASID:
                @/* r0 = base */
                MCR     p15,0,r0,c13,c0,1
                BX     lr

                .global  CoSetDomain
CoSetDomain:
                @/* r0 = domain */
                MCR     p15,0,r0,c3,c0,0
                BX     lr

                .global  CoWaitForInterrupt
CoWaitForInterrupt:
                MOV     r0,#0
                MCR     p15,0,r0,c7,c0,4
                BX     lr

                .global  CoInvalidateICache
CoInvalidateICache:
                MOV     r0,#0
                MCR     p15,0,r0,c7,c5,0
                DSB
                BX     lr

                .global  CoInvalidateDCache
CoInvalidateDCache:
                MOV     r0,#0
                MCR     p15,0,r0,c7,c6,0
                DSB
                BX     lr

                .global  CoInvalidateDCacheVA
CoInvalidateDCacheVA:
                @/* r0 = va */
                MCR     p15,0,r0,c7,c6,1
                DSB
                BX     lr

                .global  CoInvalidateDCacheIndex
CoInvalidateDCacheIndex:
                @/* r0 = index */
                MCR     p15,0,r0,c7,c6,2
                DSB
                BX     lr

                .global  CoInvalidateBothCaches
CoInvalidateBothCaches:
                MOV     r0,#0
                MCR     p15,0,r0,c7,c7,0
                DSB
                BX     lr

                .global  CoCleanDCacheVA
CoCleanDCacheVA:
                @/* r0 = va */
                MCR     p15,0,r0,c7,c10,1
                DSB
                BX     lr

                .global  CoCleanDCacheIndex
CoCleanDCacheIndex:
                @/* r0 = index */
                MCR     p15,0,r0,c7,c10,2
                DSB
                BX     lr

                .global  CoDataSyncBarrier
CoDataSyncBarrier:
                MCR     p15,0,r0,c7,c10,4
                BX     lr
                
                .global  CoPrefetchICacheLineVA
CoPrefetchICacheLineVA:
                @/* r0 = va */
                MCR     p15,0,r0,c7,c13,1
                BX     lr

                .global  CoCleanAndInvalidateDCacheVA
CoCleanAndInvalidateDCacheVA:
                @/* r0 = va */
                MCR     p15,0,r0,c7,c14,1
                DSB
                BX     lr

                .global  CoCleanAndInvalidateDCacheIndex
                @/* r0 = index */
CoCleanAndInvalidateDCacheIndex:
                MCR     p15,0,r0,c7,c14,2
                DSB
                BX     lr

                .global  CoInvalidateITlb
CoInvalidateITlb:
                MOV     r0,#0
                MCR     p15,0,r0,c8,c5,0
                BX     lr

                .global  CoInvalidateITlbVA
CoInvalidateITlbVA:
                @/* r0 = va */
                MCR     p15,0,r0,c8,c5,1
                BX     lr

                .global  CoInvalidateITlbASID /* Froggy */
CoInvalidateITlbASID:
                @/* r0 = va */
                MCR     p15,0,r0,c8,c5,2
                BX     lr

                .global  CoInvalidateDTlb
CoInvalidateDTlb:
                MOV     r0,#0
                MCR     p15,0,r0,c8,c6,0
                BX     lr

                .global  CoInvalidateDTlbVA
CoInvalidateDTlbVA:
                @/* r0 = va */
                MCR     p15,0,r0,c8,c6,1
                BX     lr

                .global  CoInvalidateDTlbASID @/* Froggy */
CoInvalidateDTlbASID:
                @/* r0 = va */
                MCR     p15,0,r0,c8,c6,2
                BX     lr

                .global  CoInvalidateMainTlb @/* Froggy */
CoInvalidateMainTlb:
                MOV     r0,#0
                MCR     p15,0,r0,c8,c7,0
                BX     lr

                .global  CoInvalidateMainTlbVA @/* Froggy */
CoInvalidateMainTlbVA:
                @/* r0 = va */
                MCR     p15,0,r0,c8,c7,1
                BX     lr

                .global  CoInvalidateMainTlbASID @/* Froggy */
CoInvalidateMainTlbASID:
                MOV     r0,#0
                MCR     p15,0,r0,c8,c7,2
                BX     lr

                .global  CoSetDCacheLockdownBase
CoSetDCacheLockdownBase:
                @/* r0 = base */
                MCR     p15,0,r0,c9,c0,0
                BX     lr

                .global  CoSetICacheLockdownBase
CoSetICacheLockdownBase:
                @/* r0 = base */
                MCR     p15,0,r0,c9,c0,1
                BX     lr

                .global  CoLockL2Cache
CoLockL2Cache:
                @/* r0=uWayNum */
                MRC     p15,1,r1,c9,c0,0
                BIC     r1,r1,r0
                MCR     p15,1,r1,c9,c0,0
                BX     lr

                .global  CoUnLockL2Cache
CoUnLockL2Cache:
                @/* r0=uWayNum */
                MRC     p15,1,r1,c9,c0,0
                BIC     r1,r1,r0
                MCR     p15,1,r1,c9,c0,0
                BX     lr

                .global  CoSetL2CacheAuxCrtlReg
CoSetL2CacheAuxCrtlReg:
                @/* r0 = uRegValue */
                MCR     p15,1,r0,c9,c0,2
                BX     lr

                .global  CoSetDTlbLockdown
CoSetDTlbLockdown:
                @/* r0 = uBaseVictim */
                MCR     p15,0,r0,c10,c0,0
                BX     lr

                .global  CoSetITlbLockdown
CoSetITlbLockdown:
                @/* r0 = uBaseVictim */
                MCR     p15,0,r0,c10,c0,1
                BX     lr

                .global  CoSetL2CacheLines
CoSetL2CacheLines:
                @/* r0=uNumOfLines */
                @ 1 line = 64 bytes
                LSL     r0,r0,#6
                MCR     p15,0,r0,c11,c7,0
                BX     lr
                
                .ltorg


@@ To load into L2 cache, memory region type should be cacheable. To determine the type,
@@ the PLE performs a hardware translation table walk at the start of the sequence and for any 4KB page boundary.
@@ Cacheable regions are the following three types.
@@ 1. Write-through, nonshared, 2. Write-back, no write-allocate, nonshared, 3. Write-back, write-allocate, nonshared
@@  When the preload engine encounters a noncached memory region, including at the start of the transfer,
@@   the preload engine stops the transfer and marks the transfer as complete.
@@ The PLE channel does not save any state for the table walk.
@@ The translation procedure is for exception checking purposes and for determination of the memory attributes of the page.

@@ You must enable the MMU for the PLE to operate.
@@ If you disabled the MMU during preloading engine configurations, the PLE treats all memory as noncacheable
@@regardless of the state of the Memory Region Remap Registers

                .global  CoCopyToL2Cache
CoCopyToL2Cache:
                @/* r0=uStAddr, r1=uWayNum, r2=uNumOfLines */
                MOV     r3,#0
                MCR     p15,0,r3,c11,c2,0   @/* Set Channel 0 or 1 */ @ Write PLE Channel Number Register
                ORR     r1,r1,#(0<<30|1<<29|1<<28) @ bit[30] - 0: to L2, 1: from L2 (0<<29)
                MCR     p15,0,r1,c11,c4,0   @/* Set PLE ConReg */ @ Write PLE Control Register
                BIC     r0,r0,#0x1f			@ why not 5bit(64bit aligned) clear? (BIC - Bitwise bit Clear)
                MCR     p15,0,r0,c11,c5,0   @/* Set PLE Start Addr */ @ Write PLE Internal Start Address Register
                LSL     r2,r2,#6			@ /* bit[N-1:6], N: log2(cache size in KB unit)+7 */
                MCR     p15,0,r2,c11,c7,0   @/* Set NumOfLines (1line = 64bytes) */ @ Write PLE Internal End Address Register
                MOV     r0,#0				@ /* Context ID is 0 */
                MCR     p15,0,r0,c11,c15,0  @/* Set Context ID */ @ Write PLE Context ID Register
                MOV     r0,#0				@ set the PLE Channel Number Register to the appropriate PLE channel
                MCR     p15,0,r0,c11,c3,1   @/* Start PLE */ @ start command triggers data to be transferred to or from the L2 cache RAM as defined by DT bit [30] of the L2 PLE Control Reg
1:              MRC     p15,0,r0,c11,c8,0   @/* Get status */ @ Read PLE Channel Status Register
                CMP     r0,#0x3				@ 0x3 means complete or error state.
                BNE     1b

				MOV     r0,#0				@ set the PLE Channel Number Register to the appropriate PLE channel
                MCR     p15,0,r0,c11,c3,2   @/* Clear PLE */ @ Channel status transitions from error or complete to idle and the interrupt or error flag is cleared to 0
2:              MRC     p15,0,r0,c11,c8,0   @/* Get status */
                CMP     r0,#0x0				@ 0x0 means idle or reset state
                BNE     2b

                BX     lr
                .ltorg

                .global  CoCopyFromL2Cache
CoCopyFromL2Cache:
                @/* r0=uStAddr, r1=uWayNum, r2=uNumOfLines */
                MOV     r3,#0
                MCR     p15,0,r3,c11,c2,0   @/* Set Channel 0 or 1 */ @ Write PLE Channel Number Register
                ORR     r1,r1,#(1<<30|1<<29|1<<28)	@ bit[30] - 0: to L2, 1: from L2 (0<<29)
                MCR     p15,0,r1,c11,c4,0   @/* Set PLE ConReg */
                BIC     r0,r0,#0x1f
                MCR     p15,0,r0,c11,c5,0   @/* Set PLE START Addr */
                LSL     r2,r2,#6
                MCR     p15,0,r2,c11,c7,0   @/* Set NumOfLines (1line = 64bytes) */
                MOV     r0,#0				@ set the PLE Channel Number Register to the appropriate PLE channel
                MCR     p15,0,r0,c11,c3,1   @/* Start PLE */ @ start command triggers data to be transferred to or from the L2 cache RAM as defined by DT bit [30] of the L2 PLE Control Reg
1:              MRC     p15,0,r0,c11,c8,0   @/* Get status */
                CMP     r0,#0x3				@ 0x3 means complete or error state.
                BNE     1b

                MOV     r0,#0				@ set the PLE Channel Number Register to the appropriate PLE channel
                MCR     p15,0,r0,c11,c3,2   @/* Clear PLE */ @ Channel status transitions from error or complete to idle and the interrupt or error flag is cleared to 0
2:              MRC     p15,0,r0,c11,c8,0   @/* Get status */
                CMP     r0,#0x0				@ 0x0 means idle or reset state
                BNE     2b

                BX     lr               @/* Return */
                .ltorg

                .global  CoStopPLE
CoStopPLE:
                @/* PLE Channel Number is always 0 */
                MOV     r0,#0				@ set the PLE Channel Number Register to the appropriate PLE channel
                MCR     p15,0,r0,c11,c3,0   @/* Stop PLE */ @ Stop command triggers data to be transferred to or from the L2 cache RAM as defined by DT bit [30] of the L2 PLE Control Reg

                BX     lr               @/* Return */
                .ltorg

                .global  CoNonSecureAccCtrl
CoNonSecureAccCtrl:
                @/*  */
                MOV     r0,#(0x73<<12)		@ set PLE, TL, CL, CP[n] to secure and non-secure accessible
                ORR		r0,r0,#(0xf<<8)
                ORR		r0,r0,#(0xff)
                MCR     p15,0,r0,c1,c1,2    @ Write Nonsecure Access Control Register data

                BX     lr               @/* Return */
                .ltorg

                .global  CoGetCacheSizeID
CoGetCacheSizeID:
                @/* r0=current cache size ID */
                MOV     r0,#0
                MRC     p14,0,r0,c0,c0,0    @/* Reads current Cache Size ID register */

                BX     lr               @/* Return */
                .ltorg

                .global  CoGetPAreg
CoGetPAreg:
                @/* r0=PA reg value */
                MOV     r0,#0
                MRC     p15,0,r0,c7,c4,0    @/* Read PA Register */

                BX     lr               @/* Return */
                .ltorg

                .global  CoGetNormalMemRemapReg
CoGetNormalMemRemapReg:
                @/* r0=Normal Memory Remap reg value */
                MOV     r0,#0
                MRC     p15,0,r0,c10,c2,1   @/* Read Normal Memory Remap Register */

                BX     lr               @/* Return */
                .ltorg

                .global  CoInvalidateDCacheForV7
CoInvalidateDCacheForV7:
                PUSH    {r4-r10,lr}
                MRC     p15,1,r0,c0,c0,1    @/* Read CLIDR */
                ANDS    r3,r0,#0x7000000
                MOV     r3,r3,LSR #23       @/* Total cache levels << 1 */
                BEQ     Finished

                MOV     r10,#0              @/* R10 holds current cache level << 1 */
Loop1:          ADD     r2,r10,r10,LSR #1   @/* R2 holds cache "Set" position */
                MOV     r1,r0,LSR r2        @/* Bottom 3 bits are the Cache-type for this level */
                AND     r1,R1,#7            @/* Get those 3 bits alone */
                CMP     r1,#2
                BLT     Skip                @/* No cache or only instruction cache at this level */

                MCR     p15,2,r10,c0,c0,0   @/* Write the Cache Size selection register */
                MOV     r1,#0
                MCR     p15,0,r1,c7,c5,4    @/* PrefetchFlush to sync the change to the CacheSizeID reg */
                MRC     p15,1,r1,c0,c0,0    @/* Reads current Cache Size ID register */
                AND     r2,r1,#0x7           @/* Extract the line length field */
                ADD     r2,r2,#4            @/* Add 4 for the line length offset (log2 16 bytes) */
                LDR     r4,=0x3FF
                ANDS    r4,r4,r1,LSR #3     @/* R4 is the max number on the way size (right aligned) */
                CLZ     r5,r4               @/* R5 is the bit position of the way size increment */
                LDR     r7,=0x00007FFF
                ANDS    r7,r7,r1,LSR #13    @/* R7 is the max number of the index size (right aligned) */

Loop2:          MOV     r9,r4               @/* R9 working copy of the max way size (right aligned) */
Loop3:          ORR     r11,r10,r9,LSL r5   @/* Factor in the Way number and cache number into R11 */
                ORR     r11,r11,r7,LSL r2   @/* Factor in the Set number */
                MCR     p15,0,r11,c7,c14,2  @/* Clean and Invalidate by set/way */
                SUBS    r9,r9,#1            @/* Decrement the Way number */
                BGE     Loop3
                SUBS    r7,r7,#1            @/* Decrement the Set number */
                BGE     Loop2
Skip:           ADD     r10,r10,#2          @/* increment the cache number */
                CMP     r3,r10
                BGT     Loop1
Finished:
                POP     {r4-r10,pc}
                .ltorg

                .global  CoSetExceptonVectoerBase
CoSetExceptonVectoerBase:
                @/* r0=uBaseAddr */
                MCR     p15,0,r0,c12,c0,0
                BX     lr

                .global  CoSetProcessId
CoSetProcessId:
                @/* r0 =u32 pid */
                MCR     p15,0,r0,c13,c0,0
                BX     lr

                .global  CoSetMpll
CoSetMpll:
                @/* r0 = param */
                MOV     r2,r0
                LDR     r0,=0x0000C000
                ORR     r0,r0,r2
                MCR     p15,0,r0,c15,c12,0
0:
                MRC     p15,0,r1,c15,c12,0
                TST     r1,#0x00800000
                BEQ     0b
                BX     lr

                .end
