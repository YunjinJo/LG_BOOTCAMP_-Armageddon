#include "cp15.h"
#include "option.h"
#include "device_driver.h"
#include "demand_page.h"

/* PA conversion */

unsigned int CoGetPAfromVA(unsigned int va, int mode)
{
	unsigned int r = 0;

	switch(mode)
	{
		case OS_READ: 	 r = CoGetOSReadPA(va); break;
		case OS_WRITE:	 r = CoGetOSWritePA(va); break;
		case USER_READ:  r = CoGetUserReadPA(va); break;
		case USER_WRITE: r = CoGetUserWritePA(va); break;
	}

	if(r & 0x1) return 0;
	return (r & ~0xfff)|(va & 0xfff);
}

void L2C_Invalidate_VA(unsigned int va, int mode)
{
	unsigned int r = CoGetPAfromVA(va, mode);

	if(r == 0) return;
	L2C_Invalidate_PA(r);
}

void L2C_Clean_VA(unsigned int va, int mode)
{
	unsigned int r = CoGetPAfromVA(va, mode);

	if(r == 0) return;
	L2C_Clean_PA(r);
}

void L2C_CleanAndInvalidate_VA(unsigned int va, int mode)
{
	unsigned int r = CoGetPAfromVA(va, mode);

	if(r == 0) return;
	L2C_CleanAndInvalidate_PA(r);
}

void L2C_Enable(void)
{
#ifdef	_uBOOT_MODE
	exynos_smc(SMC_CMD_L2X0SETUP1, 0x110, 0x120, 0x41000007);
	exynos_smc(SMC_CMD_L2X0SETUP2, 0, 0x7e470001, 0);
	exynos_smc(SMC_CMD_L2X0CTRL, 1, 0, 0);
#else
	L2x0_Reg_Write(L2X0_AUX_CTRL, 0x7e470001);
	L2x0_Reg_Write(L2X0_INTR_CLEAR, 0xFF);
	L2x0_Reg_Write(L2X0_INTR_MASK, 0x0);
	L2x0_Reg_Write(L2X0_CTRL, 1);
#endif

	CoEnableL2PrefetchHint();
}

void L2C_Disable(void)
{
	CoDisableL2PrefetchHint();

#ifdef	_uBOOT_MODE
	exynos_smc(SMC_CMD_L2X0CTRL, 0, 0, 0);
#else
	L2x0_Reg_Write(L2X0_CTRL, 0);
#endif
}

void L2C_Invalidate_All(void)
{
	L2x0_Reg_Write(L2X0_INV_WAY, 0xFFFF);
	while(!(L2x0_Reg_Read(L2X0_INV_WAY) == 0x0));
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_Invalidate_PA(unsigned int pa)
{
	L2x0_Reg_Write(L2X0_INV_LINE_PA, pa & ~0x1F);
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_Invalidate_Way(unsigned int way)
{
	L2x0_Reg_Write(L2X0_INV_WAY, 1<<way);
	while(!(L2x0_Reg_Read(L2X0_INV_WAY) == 0x0));
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_Clean_All(void)
{
	L2x0_Reg_Write(L2X0_CLEAN_WAY, 0xFFFF);
	while(!(L2x0_Reg_Read(L2X0_CLEAN_WAY) == 0x0));
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_Clean_PA(unsigned int pa)
{
	L2x0_Reg_Write(L2X0_CLEAN_LINE_PA, pa & ~0x1F);
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_Clean_SetWay(unsigned int set, unsigned int way)
{
	L2x0_Reg_Write(L2X0_CLEAN_LINE_IDX, (way<<28)|(set<<5));
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_Clean_Way(unsigned int way)
{
	L2x0_Reg_Write(L2X0_CLEAN_WAY, 1<<way);
	while(!(L2x0_Reg_Read(L2X0_CLEAN_WAY) == 0x0));
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_CleanAndInvalidate_All(void)
{
	L2x0_Reg_Write(L2X0_CLEAN_INV_WAY, 0xFFFF);
	while(!(L2x0_Reg_Read(L2X0_CLEAN_INV_WAY) == 0x0));
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_CleanAndInvalidate_PA(unsigned int pa)
{
	L2x0_Reg_Write(L2X0_CLEAN_INV_LINE_PA, pa & ~0x1F);
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_CleanAndInvalidate_SetWay(unsigned int set, unsigned int way)
{
	L2x0_Reg_Write(L2X0_CLEAN_INV_LINE_IDX, (way<<28)|(set<<5));
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void L2C_CleanAndInvalidate_Way(unsigned int way)
{
	L2x0_Reg_Write(L2X0_CLEAN_INV_WAY, 1<<way);
	while(!(L2x0_Reg_Read(L2X0_CLEAN_INV_WAY) == 0x0));
	L2x0_Reg_Write(L2X0_CACHE_SYNC, 0x0);
}

void SetTransTable(unsigned int uVaStart, unsigned int uVaEnd, unsigned int uPaStart, unsigned int attr)
{
	int i;
	unsigned int* pTT;
	unsigned int nNumOfSec;
	
	uPaStart &= ~0xfffff;
	uVaStart &= ~0xfffff;
	
	pTT = (unsigned int *)MMU_PAGE_TABLE_BASE+(uVaStart>>20);
	nNumOfSec = (0x1000+(uVaEnd>>20)-(uVaStart>>20))%0x1000;

	for(i=0; i<=nNumOfSec; i++)
	{
		*pTT++ = attr|(uPaStart+(i<<20));
	}
}

void SetTransTable_app1(unsigned int uVaStart, unsigned int uVaEnd, unsigned int uPaStart, unsigned int attr)
{
	int i;
	unsigned int* pTT;
	unsigned int nNumOfSec;

	uPaStart &= ~0xfffff;
	uVaStart &= ~0xfffff;

	pTT = (unsigned int *)MMU_PAGE_TABLE_BASE_APP1+(uVaStart>>20);
	nNumOfSec = (0x1000+(uVaEnd>>20)-(uVaStart>>20))%0x1000;

	for(i=0; i<=nNumOfSec; i++)
	{
		*pTT++ = attr|(uPaStart+(i<<20));
	}
}

void set_second_table_address_App0(unsigned int uVaStart)
{
	// 1차 T/T -> 2차 T/T 찾아갈 수 있도록 주소 지정
	unsigned int* pTT;
	pTT = get_first_TT_addr(uVaStart, MMU_PAGE_TABLE_BASE); // 시작 주소
	*pTT++ = (SND_PAGE_TABLE_BASE_APP0        ) | 0x1;
	*pTT++ = (SND_PAGE_TABLE_BASE_APP0 + 0x400) | 0x1;
	*pTT++ = (SND_PAGE_TABLE_BASE_APP0 + 0x800) | 0x1;
	*pTT   = (SND_PAGE_TABLE_BASE_APP0 + 0xC00) | 0x1;
}
void set_second_table_address_App1(unsigned int uVaStart)
{
	// 1차 T/T -> 2차 T/T 찾아갈 수 있도록 주소 지정
	unsigned int* pTT;
	pTT = get_first_TT_addr(uVaStart, MMU_PAGE_TABLE_BASE_APP1); // 시작 주소
	*pTT++ = (SND_PAGE_TABLE_BASE_APP1        ) | 0x1;
	*pTT++ = (SND_PAGE_TABLE_BASE_APP1 + 0x400) | 0x1;
	*pTT++ = (SND_PAGE_TABLE_BASE_APP1 + 0x800) | 0x1;
	*pTT   = (SND_PAGE_TABLE_BASE_APP1 + 0xC00) | 0x1;
}

void init_second_table_descriptor_App(unsigned int PAGE_APP)
{
	unsigned int* pTT;
	int i;
	int next_section;

	for (next_section = 0; next_section < 4; next_section++) {
		pTT = (unsigned int *) (PAGE_APP + (0x400 * next_section));
		for (i=0; i<256; i++) {
			*pTT++ = 0; // 2차 T/T 0으로 초기화
		}
	}
}

static void CoTTSet_L1(void);
static void CoTTSet_L1L2(void);

void CoStopMmuAndL1L2Cache(void)
{
	int i, j;

	CoDisableICache();
	CoDisableDCache();

	for(i=0;i<4;i++)
	{
		for(j=0;j<256;j++)
		{
			CoCleanAndInvalidateDCacheIndex((i<<30)|(j<<5)|(0<<0));
		}
	}

	CoInvalidateICache();
	L2C_Disable();
	L2C_CleanAndInvalidate_All();
	CoDisableMmu();
	CoInvalidateMainTlb();
	CoDisableBranchPrediction();

	CoEnableMmu();
	CoEnableICache();
	CoEnableBranchPrediction();
}

void CoStartMmuAndDCache(void)
{
	int i, j;
	
	CoDisableICache();
	CoDisableDCache();

	for(i=0;i<4;i++)
	{
		for(j=0;j<256;j++)
		{
			CoCleanAndInvalidateDCacheIndex((i<<30)|(j<<5)|(0<<0));
		}
	}

	CoInvalidateICache();
	L2C_Disable();
	L2C_CleanAndInvalidate_All();
	CoDisableMmu();
	CoInvalidateMainTlb();
	CoDisableBranchPrediction();

	CoTTSet_L1();

	CoEnableMmu();
	CoEnableICache();
	CoEnableDCache();
	CoEnableBranchPrediction();
}

void CoInitMmuAndL1L2Cache(void)
{
	int i, j;

	CoDisableICache();
	CoDisableDCache();
	CoDisableMmu();

	for(i=0;i<4;i++)
	{
		for(j=0;j<256;j++)
		{
			CoInvalidateDCacheIndex((i<<30)|(j<<5)|(0<<0));
		}
	}

	CoInvalidateICache();
	L2C_Disable();

#ifdef	_uBOOT_MODE
	exynos_smc(SMC_CMD_L2X0INVALL, 0, 0, 0);
#else
	L2C_Invalidate_All();
#endif

	CoInvalidateMainTlb();
	CoDisableBranchPrediction();

	CoTTSet_L1L2();

	CoEnableMmu();
	L2C_Enable();
	CoEnableICache();
	CoEnableDCache();
	CoEnableBranchPrediction();
}

void CoStartMmuAndL1L2Cache(void)
{
	int i, j;
	
	CoDisableICache();
	CoDisableDCache();

	for(i=0;i<4;i++)
	{
		for(j=0;j<256;j++)
		{
			CoCleanAndInvalidateDCacheIndex((i<<30)|(j<<5)|(0<<0));
		}
	}

	CoInvalidateICache();
	L2C_Disable();
	L2C_CleanAndInvalidate_All();
	CoDisableMmu();
	CoInvalidateMainTlb();
	CoDisableBranchPrediction();

	CoTTSet_L1L2();

	CoEnableMmu();
	L2C_Enable();
	CoEnableICache();
	CoEnableDCache();
	CoEnableBranchPrediction();
}

static void CoTTSet_L1(void)
{
	SetTransTable(0x00000000, 0x0CDFFFFF, 0x00000000, RW_NO_ACCESS);
	SetTransTable(0x0CE00000, 0x13FFFFFF, 0x0CE00000, RW_NCNB);
	SetTransTable(0x14000000, DRAM_START_ADDR-1, 0x14000000, RW_NO_ACCESS);
	SetTransTable(DRAM_START_ADDR, MMU_PAGE_TABLE_BASE-1, DRAM_START_ADDR, RW_WBWA);
	SetTransTable(MMU_PAGE_TABLE_BASE, MMU_PAGE_TABLE_LIMIT-1, MMU_PAGE_TABLE_BASE, RW_WBWA);

	/* Free Memory */
	SetTransTable(MMU_PAGE_TABLE_LIMIT, LCD_FB00_START_ADDR-1, MMU_PAGE_TABLE_LIMIT, RW_WT);

	/* LCD Frame Buffer */
	SetTransTable(LCD_FB00_START_ADDR, LCD_FB01_START_ADDR-1, LCD_FB00_START_ADDR, RW_WT);
	SetTransTable(LCD_FB01_START_ADDR, LCD_FB10_START_ADDR-1, LCD_FB01_START_ADDR, RW_WT);
	SetTransTable(LCD_FB10_START_ADDR, LCD_FB11_START_ADDR-1, LCD_FB10_START_ADDR, RW_WT);
	SetTransTable(LCD_FB11_START_ADDR, LCD_FB20_START_ADDR-1, LCD_FB11_START_ADDR, RW_WT);
	SetTransTable(LCD_FB20_START_ADDR, LCD_FB21_START_ADDR-1, LCD_FB20_START_ADDR, RW_WT);
	SetTransTable(LCD_FB21_START_ADDR, LCD_FB30_START_ADDR-1, LCD_FB21_START_ADDR, RW_WT);
	SetTransTable(LCD_FB30_START_ADDR, LCD_FB31_START_ADDR-1, LCD_FB30_START_ADDR, RW_WT);
	SetTransTable(LCD_FB31_START_ADDR, LCD_FB40_START_ADDR-1, LCD_FB31_START_ADDR, RW_WT);
	SetTransTable(LCD_FB40_START_ADDR, LCD_FB41_START_ADDR-1, LCD_FB40_START_ADDR, RW_WT);
	SetTransTable(LCD_FB41_START_ADDR, LCD_FB_END_ADDR-1,     LCD_FB41_START_ADDR, RW_WT);

	SetTransTable(LCD_FB_END_ADDR, 0x80000000-1, LCD_FB_END_ADDR, RW_NO_ACCESS);
	SetTransTable(0x80000000, 0xFFFFFFFF, 0x80000000, RW_NO_ACCESS);

	CoSetTTBase(MMU_PAGE_TABLE_BASE|(1<<6)|(1<<3)|(0<<1)|(0<<0));
	CoSetDomain(0x55555550|(DOMAIN_NO_ACCESS<<2)|(DOMAIN_CLIENT));
}

static void CoTTSet_L1L2(void)
{
	SetTransTable(0x00000000, 0x0CDFFFFF, 0x00000000, RW_NO_ACCESS);
	SetTransTable(0x0CE00000, 0x13FFFFFF, 0x0CE00000, RW_NCNB);
	SetTransTable(0x14000000, DRAM_START_ADDR-1, 0x14000000, RW_NO_ACCESS);
	SetTransTable(DRAM_START_ADDR, MMU_PAGE_TABLE_BASE-1, DRAM_START_ADDR, RW_WBWA);
	SetTransTable(MMU_PAGE_TABLE_BASE, MMU_PAGE_TABLE_LIMIT-1, MMU_PAGE_TABLE_BASE, RW_WBWA);

	/* Free Memory */
	SetTransTable(MMU_PAGE_TABLE_LIMIT, LCD_FB00_START_ADDR-1, MMU_PAGE_TABLE_LIMIT, RW_WT);

	/* LCD Frame Buffer */
	SetTransTable(LCD_FB00_START_ADDR, LCD_FB01_START_ADDR-1, LCD_FB00_START_ADDR, RW_WT);
	SetTransTable(LCD_FB01_START_ADDR, LCD_FB10_START_ADDR-1, LCD_FB01_START_ADDR, RW_WT);
	SetTransTable(LCD_FB10_START_ADDR, LCD_FB11_START_ADDR-1, LCD_FB10_START_ADDR, RW_WT);
	SetTransTable(LCD_FB11_START_ADDR, LCD_FB20_START_ADDR-1, LCD_FB11_START_ADDR, RW_WT);
	SetTransTable(LCD_FB20_START_ADDR, LCD_FB21_START_ADDR-1, LCD_FB20_START_ADDR, RW_WT);
	SetTransTable(LCD_FB21_START_ADDR, LCD_FB30_START_ADDR-1, LCD_FB21_START_ADDR, RW_WT);
	SetTransTable(LCD_FB30_START_ADDR, LCD_FB31_START_ADDR-1, LCD_FB30_START_ADDR, RW_WT);
	SetTransTable(LCD_FB31_START_ADDR, LCD_FB40_START_ADDR-1, LCD_FB31_START_ADDR, RW_WT);
	SetTransTable(LCD_FB40_START_ADDR, LCD_FB41_START_ADDR-1, LCD_FB40_START_ADDR, RW_WT);
	SetTransTable(LCD_FB41_START_ADDR, LCD_FB_END_ADDR-1,     LCD_FB41_START_ADDR, RW_WT);

	SetTransTable(LCD_FB_END_ADDR, 0x80000000-1, LCD_FB_END_ADDR, RW_NO_ACCESS);
	SetTransTable(0x80000000, 0xFFFFFFFF, 0x80000000, RW_NO_ACCESS);

	CoSetTTBase(MMU_PAGE_TABLE_BASE|(1<<6)|(1<<3)|(0<<1)|(0<<0));
	CoSetDomain(0x55555550|(DOMAIN_NO_ACCESS<<2)|(DOMAIN_CLIENT));
}

void CoTTSet_L1L2_app1(void)
{
	SetTransTable_app1(0x00000000, 0x0CDFFFFF, 0x00000000, RW_NO_ACCESS);
	SetTransTable_app1(0x0CE00000, 0x13FFFFFF, 0x0CE00000, RW_NCNB);
	SetTransTable_app1(0x14000000, DRAM_START_ADDR-1, 0x14000000, RW_NO_ACCESS);
	SetTransTable_app1(DRAM_START_ADDR, MMU_PAGE_TABLE_BASE-1, DRAM_START_ADDR, RW_WBWA); //RW_WBWA, RW_WT, RW_WT_WBWA
	SetTransTable_app1(MMU_PAGE_TABLE_BASE, MMU_PAGE_TABLE_LIMIT-1, MMU_PAGE_TABLE_BASE, RW_WBWA); //RW_WBWA, RW_WT, RW_WT_WBWA

	/* Free Memory */
	SetTransTable_app1(MMU_PAGE_TABLE_LIMIT, LCD_FB00_START_ADDR-1, MMU_PAGE_TABLE_LIMIT, RW_WT); //RW_WBWA, RW_WT, RW_WT_WBWA

	/* LCD Frame Buffer */
	SetTransTable_app1(LCD_FB00_START_ADDR, LCD_FB01_START_ADDR-1, LCD_FB00_START_ADDR, RW_WT);
	SetTransTable_app1(LCD_FB01_START_ADDR, LCD_FB10_START_ADDR-1, LCD_FB01_START_ADDR, RW_WT);
	SetTransTable_app1(LCD_FB10_START_ADDR, LCD_FB11_START_ADDR-1, LCD_FB10_START_ADDR, RW_WT);
	SetTransTable_app1(LCD_FB11_START_ADDR, LCD_FB20_START_ADDR-1, LCD_FB11_START_ADDR, RW_WT);
	SetTransTable_app1(LCD_FB20_START_ADDR, LCD_FB21_START_ADDR-1, LCD_FB20_START_ADDR, RW_WT);
	SetTransTable_app1(LCD_FB21_START_ADDR, LCD_FB30_START_ADDR-1, LCD_FB21_START_ADDR, RW_WT);
	SetTransTable_app1(LCD_FB30_START_ADDR, LCD_FB31_START_ADDR-1, LCD_FB30_START_ADDR, RW_WT);
	SetTransTable_app1(LCD_FB31_START_ADDR, LCD_FB40_START_ADDR-1, LCD_FB31_START_ADDR, RW_WT);
	SetTransTable_app1(LCD_FB40_START_ADDR, LCD_FB41_START_ADDR-1, LCD_FB40_START_ADDR, RW_WT);
	SetTransTable_app1(LCD_FB41_START_ADDR, LCD_FB_END_ADDR-1,     LCD_FB41_START_ADDR, RW_WT);

	SetTransTable_app1(LCD_FB_END_ADDR, 0x80000000-1, LCD_FB_END_ADDR, RW_NO_ACCESS);
	SetTransTable_app1(0x80000000, 0xFFFFFFFF, 0x80000000, RW_NO_ACCESS);

	CoSetTTBase(0x44080000|(1<<6)|(1<<3)|(0<<1)|(0<<0));
	CoSetDomain(0x55555550|(DOMAIN_NO_ACCESS<<2)|(DOMAIN_CLIENT));
}
