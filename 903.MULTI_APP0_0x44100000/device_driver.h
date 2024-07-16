#include "4412_addr.h"
#include "macro.h"
#include "option.h"

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

//uart.c
#define Uart_Printf		Uart1_Printf
extern void Uart1_Send_Byte(char data);
extern void Uart1_Send_String(const char *pt);
extern void Uart1_Printf(const char *fmt,...);
extern char Uart1_Get_Char(void);
extern char Uart1_Get_Pressed(void);
extern int Uart1_GetIntNum(void);

// key.c
extern int Key_Get_Key_Pressed(void);

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
