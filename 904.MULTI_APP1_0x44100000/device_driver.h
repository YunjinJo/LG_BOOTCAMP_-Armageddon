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
//#define Uart_Printf		Uart1_Printf
extern void Uart_Printf(const char *fmt,...);

extern void Uart1_Send_Byte(char data);
extern void Uart1_Send_String(const char *pt);
extern void Uart1_Printf(const char *fmt,...);
extern char Uart1_Get_Char(void);
extern void Uart1_GetString(char *string);
extern char Uart1_Get_Pressed(void);
extern int Uart1_GetIntNum(void);

extern void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...);
void Lcd_Clr_Screen(void);
void Lcd_Clr_Screen_left_half(void);
extern void Lcd_Draw_Back_Color(int color);

extern unsigned int Get_Input_Flag(void);
extern void Toggle_Input_Flag(void);


// key.c
extern int Key_Get_Key_Pressed(void);
