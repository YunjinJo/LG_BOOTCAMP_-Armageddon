#include "device_driver.h"

extern WIN_INFO_ST ArrWinInfo[5];

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f

#define 	RAM_APP0			0x44100000
#define 	RAM_APP1			(RAM_APP0+SIZE_APP0)
#define 	SIZE_STACK0			(1*1024*1024)
#define 	SIZE_STACK1			(1*1024*1024)
#define 	STACK_LIMIT_APP0	(RAM_APP1+SIZE_APP1)
#define 	STACK_LIMIT_APP1	(STACK_LIMIT_APP0+SIZE_STACK0)
#define 	STACK_BASE_APP0		(STACK_LIMIT_APP0+SIZE_STACK0)
#define 	STACK_BASE_APP1		(STACK_LIMIT_APP1+SIZE_STACK1)

#define 	SIZE_APP0			(4*1024*1024)
#define 	SIZE_APP1			(4*1024*1024)

#define 	SECTOR_APP0			100
#define 	SECTOR_APP1			5000

#define SECTOR_SIZE 		512
#define ALIGN_SECTOR(x)	 	((((x+(SECTOR_SIZE-1))&~(SECTOR_SIZE-1))/SECTOR_SIZE))

#define TT_APP_0 MMU_PAGE_TABLE_BASE + 0x50000
#define TT_APP_1 MMU_PAGE_TABLE_BASE + 0x60000


typedef struct {
	unsigned int uVaStart;
	unsigned int uVaEnd;
	unsigned int uPaStart;
	unsigned int attr;
}TT_info;

TT_info info_app_0 = {RAM_APP0, (RAM_APP0+SIZE_APP0-1), RAM_APP0, (RW_WBWA | (1 << 17))};
TT_info info_stack_app_0 = {STACK_LIMIT_APP0, STACK_BASE_APP0-1, STACK_LIMIT_APP0, RW_WBWA};
TT_info info_app_1 = {RAM_APP0, (RAM_APP0+SIZE_APP1-1), RAM_APP1, (RW_WBWA | (1 << 17))};
TT_info info_stack_app_1 = {STACK_LIMIT_APP1, STACK_BASE_APP1-1, STACK_LIMIT_APP1, RW_WBWA};

void init_transtable(unsigned int tt_app_addr, TT_info info)
{
	int i = 0;
	unsigned int *pTT;
	unsigned int nNumOfSec;

	info.uPaStart &= ~0xfffff;
	info.uVaStart &= ~0xfffff;

	pTT = (unsigned int *) tt_app_addr + (info.uVaStart>>20);
	nNumOfSec = (0x1000+(info.uVaEnd>>20)-(info.uVaStart>>20))%0x1000;

	for(i=0; i<=nNumOfSec; i++)
	{
		Uart_Printf("\npTT : %p\n", pTT);
		Uart_Printf("\n*pTT : %X\n", *pTT);
		*pTT = info.attr|(info.uPaStart+(i<<20));
		Uart_Printf("\n changed *pTT : %X\n", *pTT);
		pTT++;
	}
}

void App_Read(unsigned int sector, unsigned int size, unsigned int addr)
{
	int i;

	for(i=sector; i<(sector+ALIGN_SECTOR(size)); i++)
	{
		SD_Read_Sector(i, 1,(void *)addr);
		addr += SECTOR_SIZE;
	}
}

void start_app_0()
{
	Uart_Printf("\nAPP0 RUN\n", 0);
//	Uart_Printf("\n==========\n");
//
//	init_transtable(TT_APP_0, info_app_0);
//	init_transtable(TT_APP_0, info_stack_app_0);
//
//	Uart_Printf("\n==========\n");
	CoSetASID(1);

	Uart_Printf("\n TTBR0 : %X", set_ttbr_app_0());

	Run_App(RAM_APP0, STACK_BASE_APP0);
}

void start_app_1()
{
	Uart_Printf("\nAPP1 RUN\n", 1);
//	Uart_Printf("\n==========\n");
//
//	init_transtable(TT_APP_1, info_app_1);
//	init_transtable(TT_APP_1, info_stack_app_1);
//
//	Uart_Printf("\n==========\n");
	CoSetASID(2);
	Uart_Printf("\n TTBR0 : %X", set_ttbr_app_1());
	Run_App(RAM_APP0, STACK_BASE_APP1);
}

void Main(void)
{
	CoInitMmuAndL1L2Cache();
	Uart_Init(115200);
	LED_Init();
	Key_ISR_Init();
	Key_ISR_Enable(1);

	Uart_Printf("\nOS Template\n");

	ArrWinInfo[0].bpp_mode = BPPMODE_16BPP_565;
	ArrWinInfo[0].bytes_per_pixel = 2;
	ArrWinInfo[0].p_sizex = 1024;
	ArrWinInfo[0].p_sizey = 600;
	ArrWinInfo[0].v_sizex = 1024;
	ArrWinInfo[0].v_sizey = 600;
	ArrWinInfo[0].posx = (1024 - ArrWinInfo[0].p_sizex) / 2;
	ArrWinInfo[0].posy = (600 - ArrWinInfo[0].p_sizey) / 2;

	Lcd_Init();
	Lcd_Win_Init(0, 1);
	Lcd_Brightness_Control(8);

	Lcd_Select_Display_Frame_Buffer(0, 0);
	Lcd_Select_Draw_Frame_Buffer(0, 0);
	Lcd_Draw_Back_Color(GREEN);

	GIC_CPU_Interface_Enable(0,1);
	GIC_Set_Priority_Mask(0,0xFF);
	GIC_Distributor_Enable(1);

	Uart_Printf("\n==========\n");

	init_transtable(TT_APP_0, info_app_0);
	init_transtable(TT_APP_0, info_stack_app_0);

	Uart_Printf("\n==========\n");

	Uart_Printf("\n==========\n");

	init_transtable(TT_APP_1, info_app_1);
	init_transtable(TT_APP_1, info_stack_app_1);

	Uart_Printf("\n==========\n");

#if 0 // SD Loading
	{
		extern volatile unsigned int sd_insert_flag;
		SDHC_Init();
		SDHC_ISR_Enable(1);
		if(!sd_insert_flag) Uart_Printf("SD!\n");
		while(!sd_insert_flag);
		SDHC_Card_Init();

		Uart_Printf("APP0 Loading!\n");
		App_Read(SECTOR_APP0, SIZE_APP0, RAM_APP0);
		Uart_Printf("APP1 Loading!\n");
		App_Read(SECTOR_APP1, SIZE_APP0, RAM_APP1);
	}
#endif

	for(;;)
	{
		unsigned char x;

		Uart_Printf("\nAPP [1]APP0, [2]APP1 >> ");
		x = Uart1_Get_Char();

		if(x == '1')
		{
			start_app_0();
		}

		if(x == '2')
		{
			start_app_1();
		}
	}
}