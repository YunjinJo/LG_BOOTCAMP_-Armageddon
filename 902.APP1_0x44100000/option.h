/* Test Environment Selection */

#define __TEST_TYPE		__uBOOT

#define __uBOOT			0
#define __trace32		1

#if __TEST_TYPE == __uBOOT

	#define _uBOOT_MODE

#endif

#define	FREQ_APLL		(1200000000)
#define FREQ_MPLL		(800000000)
#define SCLK_UART0		(100000000)

#define	STACK_BASE		(0x44000000)
#define STACK_SIZE		(0x800000)
#define STACK_LIMIT		(STACK_BASE - 0x800000)

#define DRAM_START_ADDR			0x40000000
#define DRAM_END_ADDR			0x80000000

/* MMU Table 1MB */
#define MMU_PAGE_TABLE_BASE		STACK_BASE						 // 0x44000000
#define MMU_PAGE_TABLE_SIZE		0x100000						 // 1MB
#define MMU_PAGE_TABLE_LIMIT	(STACK_BASE+MMU_PAGE_TABLE_SIZE) // 0x44100000

#define TIMER_TICK		(20)
#define TIMER_BASECLK	(100000000)	// PERIL - ACLK_GPL

/* LCD */
#define KILOBYTES				(1024)
#define MEGABYTES				(1024 * KILOBYTES)

#define LCD_FB0_SIZE			(4	* MEGABYTES)
#define LCD_FB1_SIZE			(4	* MEGABYTES)
#define LCD_FB2_SIZE			(4	* MEGABYTES)
#define LCD_FB3_SIZE			(4	* MEGABYTES)
#define LCD_FB4_SIZE			(4	* MEGABYTES)
#define LCD_FB_TOTAL_SIZE	 	(40 * MEGABYTES)

#define LCD_FB_START_ADDR		(0x4B000000)
#define LCD_FB00_START_ADDR		(LCD_FB_START_ADDR)
#define LCD_FB01_START_ADDR		(LCD_FB00_START_ADDR + LCD_FB0_SIZE)
#define LCD_FB10_START_ADDR		(LCD_FB01_START_ADDR + LCD_FB0_SIZE)
#define LCD_FB11_START_ADDR		(LCD_FB10_START_ADDR + LCD_FB1_SIZE)
#define LCD_FB20_START_ADDR		(LCD_FB11_START_ADDR  + LCD_FB1_SIZE)
#define LCD_FB21_START_ADDR		(LCD_FB20_START_ADDR  + LCD_FB2_SIZE)
#define LCD_FB30_START_ADDR		(LCD_FB21_START_ADDR  + LCD_FB2_SIZE)
#define LCD_FB31_START_ADDR		(LCD_FB30_START_ADDR  + LCD_FB3_SIZE)
#define LCD_FB40_START_ADDR		(LCD_FB31_START_ADDR  + LCD_FB3_SIZE)
#define LCD_FB41_START_ADDR		(LCD_FB40_START_ADDR  + LCD_FB4_SIZE)
#define LCD_FB_END_ADDR			(LCD_FB41_START_ADDR  + LCD_FB4_SIZE)