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
