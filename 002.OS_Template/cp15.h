/* PL310 Registers */
#define L2x0_Reg_Write(a, d)	(*(volatile unsigned int *)(L2x0_BASE_REG + (a)) = (d))
#define L2x0_Reg_Read(a)		(*(volatile unsigned int *)(L2x0_BASE_REG + (a)))

#define L2x0_BASE_REG 			0x10502000
#define L2X0_CACHE_ID			0x000
#define L2X0_CACHE_TYPE			0x004
#define L2X0_CTRL			0x100
#define L2X0_AUX_CTRL			0x104
#define L310_TAG_LATENCY_CTRL		0x108
#define L310_DATA_LATENCY_CTRL		0x10C
#define L2X0_EVENT_CNT_CTRL		0x200
#define L2X0_EVENT_CNT1_CFG		0x204
#define L2X0_EVENT_CNT0_CFG		0x208
#define L2X0_EVENT_CNT1_VAL		0x20C
#define L2X0_EVENT_CNT0_VAL		0x210
#define L2X0_INTR_MASK			0x214
#define L2X0_MASKED_INTR_STAT		0x218
#define L2X0_RAW_INTR_STAT		0x21C
#define L2X0_INTR_CLEAR			0x220
#define L2X0_CACHE_SYNC			0x730
#define L2X0_DUMMY_REG			0x740
#define L2X0_INV_LINE_PA		0x770
#define L2X0_INV_WAY			0x77C
#define L2X0_CLEAN_LINE_PA		0x7B0
#define L2X0_CLEAN_LINE_IDX		0x7B8
#define L2X0_CLEAN_WAY			0x7BC
#define L2X0_CLEAN_INV_LINE_PA		0x7F0
#define L2X0_CLEAN_INV_LINE_IDX		0x7F8
#define L2X0_CLEAN_INV_WAY		0x7FC

#define L2X0_LOCKDOWN_WAY_D_BASE	0x900
#define L2X0_LOCKDOWN_WAY_I_BASE	0x904
#define L2X0_LOCKDOWN_STRIDE		0x08
#define L310_ADDR_FILTER_START		0xC00
#define L310_ADDR_FILTER_END		0xC04
#define L2X0_TEST_OPERATION		0xF00
#define L2X0_LINE_DATA			0xF10
#define L2X0_LINE_TAG			0xF30
#define L2X0_DEBUG_CTRL			0xF40
#define L310_PREFETCH_CTRL		0xF60
#define L310_POWER_CTRL			0xF80

/* exynos_SMC Command Set */
#define SMC_CMD_INIT            (-1)
#define SMC_CMD_INFO            (-2)

/* For Power Management */
#define SMC_CMD_SLEEP           (-3)
#define SMC_CMD_CPU1BOOT        (-4)
#define SMC_CMD_CPU0AFTR        (-5)
#define SMC_CMD_SAVE            (-6)
#define SMC_CMD_SHUTDOWN        (-7)

/* For CP15 Access */
#define SMC_CMD_C15RESUME       (-11)

/* For L2 Cache Access */
#define SMC_CMD_L2X0CTRL        (-21)
#define SMC_CMD_L2X0SETUP1      (-22)
#define SMC_CMD_L2X0SETUP2      (-23)
#define SMC_CMD_L2X0INVALL      (-24)
#define SMC_CMD_L2X0DEBUG       (-25)

extern void exynos_smc(unsigned int reg, int val1, int val2, int val3);

// 1's-Level Translation Table Descriptor Type Definition
#define DT_PAGE			(0<<4|0<<2|1)	/* Page Table, bit[4]=bit[2]=0(SBZ=Should Be Zero)*/
#define DT_SECTION		(0<<4|2)		/* Section Table, XN(eXecute Never)=0 */
#define DT_SUPER_SECT	(1<<18|0<<4|2)	/* Super Section, XN(eXecute Never)=0 */

// Thus, 'C' and 'B' bit is interpreted as below(When TEX[2:0]=000 in ARMv7)
#define NCNB		(0x0<<2)		/* Uncached <=> Unbuffered(ARMv7 : Strongly-Ordered)*/
#define NCB			(0x1<<2)		/* Uncached <=> Buffered(ARMv7 : Shareable Device) */
#define WT			(0x2<<2)		/* Write-Through Cached <=> Buffered(ARMv7 : Outer and Inner WT, No Write-Allocate) */
#define WB			(0x3<<2)		/* Write-Back Cached <=> Buffered(ARMv7 : Outer and Inner WB, No Write-Allocate) */
#define WBWA		((0x1<<12)|(0x3<<2))

// Access Permission Bits Definition(When AP[2]=0 in ARMv7)
#define ALL_NO_ACCESS	(0<<10)	/* Previleged:No, User:No */
#define KERNEL_RO		(1<<10)	/* Previleged:RW, User:No */
#define USER_RO			(2<<10)	/* Previleged:RW, User:RO */
#define USER_RW			(3<<10)	/* Previleged:RW, User:RW */

// Domain Access Values Definition
#define DOMAIN_NO_ACCESS	(0)	/* Any access generate Domain Falut */
#define DOMAIN_CLIENT		(1)	/* Check AP(Access Permission) bits */
#define DOMAIN_RESERVED		(2)	
#define DOMAIN_MANAGER		(3)	/* No Check, No Falut */

// Domain Number Definition
#define DOMAIN0			(0x0<<5)
#define DOMAIN1			(0x1<<5)

// Secure, Non Secure
#define SS				(0x0<<19)
#define NS				(0x0<<19)
//#define NS				(0x1<<19)

// 1's-Level Translation Table Bit Field Definition
#define RW_WBWA			(NS|USER_RW|DOMAIN0|WBWA|DT_SECTION)
#define RW_WB			(NS|USER_RW|DOMAIN0|WB|DT_SECTION)
#define RW_WT			(NS|USER_RW|DOMAIN0|WT|DT_SECTION)
#define RW_NCB			(SS|USER_RW|DOMAIN0|NCB|DT_SECTION)
#define RW_NCNB			(SS|USER_RW|DOMAIN0|NCNB|DT_SECTION)
#define RW_NO_ACCESS	(SS|USER_RW|DOMAIN1|NCNB|DT_SECTION)

// cp15a.s & cp15.c

void CoEnableL2PrefetchHint(void);
void CoDisableL2PrefetchHint(void);
void CoEnableIrq(void);
void CoEnableFiq(void);
void CoDisableIrq(void);
void CoDisableFiq(void);
unsigned int  CoSetIF(void); 
void CoWrIF(unsigned int cpsr);
void CoClrIF(void);

void CoSetProcessId(unsigned int uId);
void CoStopMmuAndL1L2Cache(void);
void CoInitMmuAndL1L2Cache(void);
void CoStartMmuAndDCache(void);
void CoStartMmuAndL1L2Cache(void);
void CoStopMmuAndDCache(void);
void CoStartICacheOnly(void);
void CoStartICache(void);
void CoStartL2Cache(void);

void CoEnableVectoredInt(void);
void CoDisableVectoredInt(void);

void CoEnableAlignFault(void);
void CoDisableAlignFault(void);
void CoEnableUnalignedAccess(void);
void CoDisableUnalignedAccess(void);

void CoEnableICache(void);
void CoEnableDCache(void);
void CoDisableDCache(void);
void CoDisableICache(void);

void CoEnableVfp(void);
void CoEnableNeon(void);

void CoEnableBranchPrediction(void);
void CoDisableBranchPrediction(void);
void CoInvalidateICache(void);
void CoPrefetchICacheLineVA(unsigned int va);
void CoInvalidateDCache(void);
void CoInvalidateDCacheVA(unsigned int va);
void CoInvalidateDCacheIndex(unsigned int index);
void CoCleanDCacheVA(unsigned int va);
void CoCleanDCacheIndex(unsigned int index);
void CoDataSyncBarrier(void);
void CoCleanAndInvalidateDCacheVA(unsigned int va);
void CoCleanAndInvalidateDCacheIndex(unsigned int index);
void CoInvalidateDCacheForV7(void);
void CoWaitForInterrupt(void);
void CoSetDCacheLockdownBase(unsigned int base);
void CoSetICacheLockdownBase(unsigned int base);
void CoSetDTlbLockdown(unsigned int baseVictim);
void CoSetITlbLockdown(unsigned int baseVictim);

void CoEnableMmu(void);
void CoDisableMmu(void);
void CoSetTTBase(unsigned int base);
void CoSelTTBReg0(void);
void CoSetASID(unsigned int ASID);
void CoSetDomain(unsigned int domain);
void CoInvalidateITlb(void);
void CoInvalidateITlbVA(unsigned int va);
void CoInvalidateITlbASID(unsigned int asid);
void CoInvalidateDTlb(void);
void CoInvalidateDTlbVA(unsigned int va);
void CoInvalidateDTlbASID(unsigned int asid);
void CoInvalidateMainTlb(void);
void CoInvalidateMainTlbVA(unsigned int va);
void CoInvalidateMainTlbASID(unsigned int asid);

void CoStartMmu(void);
void CoEnableL2Cache(void);
void CoDisableL2Cache(void);
void CoSetL2CacheAuxCrtlReg(unsigned int uRegValue);
void CoSetL2CacheLines(unsigned int uNumOfLinesTransferred); 
void CoCopyToL2Cache(unsigned int uAddr, unsigned int uWayNum, unsigned int uNumOfLines);
void CoCopyFromL2Cache(unsigned int uAddr, unsigned int uWayNum, unsigned int uNumOfLines);
void CoStopPLE(void);
void CoNonSecureAccCtrl(void);
void CoGetCacheSizeID(unsigned int * uSizeId);
void CoGetPAreg(unsigned int * uReg);
void CoGetNormalMemRemapReg(unsigned int * uReg);
void CoLockL2Cache(unsigned int uWayNum);
void CoUnLockL2Cache(unsigned int uWayNum);
void CoSetExceptonVectoerBase(unsigned int uBaseAddr);
void SetTransTable(unsigned int uVaStart, unsigned int uVaEnd, unsigned int uPaStart, unsigned int attr);

/* PA conversion */

extern unsigned int CoGetOSReadPA(unsigned int va);
extern unsigned int CoGetOSWritePA(unsigned int va);
extern unsigned int CoGetUserReadPA(unsigned int va);
extern unsigned int CoGetUserWritePA(unsigned int va);
extern unsigned int CoGetPAfromVA(unsigned int va, int mode);

/* PA conversion */

#define OS_READ		0
#define OS_WRITE	1
#define USER_READ	2
#define USER_WRITE	3

/* PA conversion */

extern void L2C_Invalidate_VA(unsigned int va, int mode);
extern void L2C_Clean_VA(unsigned int va, int mode);
extern void L2C_CleanAndInvalidate_VA(unsigned int va, int mode);

/* L2C PL310 */

void L2C_Enable(void);
void L2C_Enable(void);
void L2C_Invalidate_All(void);
void L2C_Invalidate_PA(unsigned int pa);
void L2C_Invalidate_Way(unsigned int way);
void L2C_Clean_All(void);
void L2C_Clean_PA(unsigned int pa);
void L2C_Clean_SetWay(unsigned int set, unsigned int way);
void L2C_Clean_Way(unsigned int way);
void L2C_CleanAndInvalidate_All(void);
void L2C_CleanAndInvalidate_PA(unsigned int pa);
void L2C_CleanAndInvalidate_SetWay(unsigned int set, unsigned int way);
void L2C_CleanAndInvalidate_Way(unsigned int way);
