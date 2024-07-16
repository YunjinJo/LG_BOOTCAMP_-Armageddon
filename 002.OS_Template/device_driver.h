#include "4412_addr.h"
#include "macro.h"
#include "option.h"
#include "cp15.h"

#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>

// runtime.c
extern void Delay(unsigned int v);
extern unsigned int Get_Stack_Base(void);
extern unsigned int Get_Stack_Limit(void);
extern unsigned int Get_Heap_Base(void);
extern unsigned int Get_Heap_Limit(void);

//led.c
extern void LED_Init(void);
extern void LED_Display(int led);

//uart.c
#define Uart_Init		Uart1_Init
#define Uart_Printf		Uart1_Printf

extern void Uart1_Init(int baud);
extern void Uart1_Send_Byte(char data);
extern void Uart1_Send_String(const char *pt);
extern void Uart1_Printf(const char *fmt,...);
extern char Uart1_Get_Char(void);
extern char Uart1_Get_Pressed(void);
extern int Uart1_GetIntNum(void);
extern void Uart1_ISR_Enable(int rx,int tx, int err);

//asm_function.s
extern unsigned int PABT_Falut_Status(void);
extern unsigned int PABT_Falut_Address(void);
extern unsigned int DABT_Falut_Status(void);
extern unsigned int DABT_Falut_Address(void);
extern unsigned int Get_SP(void);
extern unsigned int Main_ID(void);
extern unsigned int TLB_Type(void);
extern void Run_App(unsigned int addr, unsigned int stack);

//timer
extern void Timer0_Delay(int mtime);
extern void Timer0_Int_Delay(int en, int mtime);
extern void Timer0_Run(void);
extern unsigned int Timer0_Stop(void);

//key.c
extern void Key_Poll_Init(void);
extern int Key_Get_Key_Pressed(void);
extern void Key_Wait_Key_Released(void);
extern int Key_Wait_Key_Pressed(void);
extern void Key_ISR_Init(void);
extern void Key_ISR_Enable(int en);

//gic.c
extern void GIC_Distributor_Enable(int en);
extern void GIC_Interrupt_Enable(int cpuid,int intid);
extern void GIC_Interrupt_Disable(int cpuid,int intid);
extern void GIC_Set_Interrupt_Priority(int cpuid, int intid, int prio);
extern void GIC_Set_Priority_Mask(int cpuid, int prio);
extern void GIC_Set_Processor_Target(int cpuid, int intid, int cpubit);
extern inline void GIC_Clear_Pending_Clear(int cpuid, int intid);
extern void GIC_CPU_Interface_Enable(int cpuid, int value);
extern unsigned int GIC_Read_INTACK(int cpuid);
extern void GIC_Write_EOI(int cpuid, int cpu_int_id);
extern void GIC_Generate_SGI(int taregtfilter, int cpubit, int intid);;

//sdhc.c
extern void SDHC_Init(void);
extern void SDHC_Card_Init(void);
extern void SD_Read_Sector(int sec,int n,void *buf);
extern void SD_Write_Sector(int sec,int n,void *buf);
extern void SDHC_ISR_Enable(int en);

//graphics.c

typedef struct {
	unsigned int posx;
	unsigned int posy;
	unsigned int p_sizex;
	unsigned int p_sizey;
	unsigned int v_sizex;
	unsigned int v_sizey;
	unsigned int bytes_per_pixel;
	unsigned int bpp_mode;
}WIN_INFO_ST;

#define BPPMODE_1BPP				(0)
#define BPPMODE_2BPP				(1)
#define BPPMODE_4BPP				(2)
#define BPPMODE_8BPP_PAL			(3)
#define BPPMODE_8BPP_NOPAL			(4)
#define BPPMODE_16BPP_565			(5)
#define BPPMODE_16BPP_A555			(6)
#define BPPMODE_18BPP_666			(8)
#define BPPMODE_24BPP_888			(11)
#define BPPMODE_24BPP_A887			(12)
#define BPPMODE_25BPP_A888			(13)
#define BPPMODE_32BPP_A888			(13)
#define BPPMODE_13BPP_A444			(14)
#define BPPMODE_16BPP_A444			(14)
#define BPPMODE_15BPP_555			(15)

#define WIDTH		(1024)
#define HEIGHT		(600)

extern void	Lcd_Init(void);
extern void Lcd_Win_Init(int id,int en);
extern void Lcd_Put_Pixel(int x,int y, unsigned short color);
extern void * Lcd_Get_Pixel_Address(int x,int y);
extern unsigned int Lcd_Get_Pixel(int x,int y);
extern void Lcd_Clr_Screen(void);
extern void Lcd_Draw_Back_Color(int color);
extern void Lcd_Get_Info_BMP(int * x, int  * y, const unsigned short int *fp);
extern void Lcd_Draw_BMP(int x, int y, const unsigned short int *fp);
extern void Lcd_Select_Draw_Frame_Buffer(int win_id,int buf_num);
extern void Lcd_Select_Display_Frame_Buffer(int win_id,int buf_num);
extern void Lcd_Wait_Blank(void);
extern void Lcd_Draw_Image(int x, int y, const unsigned short int *fp, int width, int height);
extern void Lcd_Brightness_Control(int level);
extern void Lcd_Draw_BMP_File_24bpp(int x, int y, void *fp);
extern void Lcd_Draw_STACK(void);
extern void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...);
extern void Lcd_Draw_Bar(int x1, int y1, int x2, int y2, int color);
extern void Lcd_Draw_Line(int x1,int y1,int x2,int y2,int color);
