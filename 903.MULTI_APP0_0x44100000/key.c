#include "device_driver.h"

#define rEXT_INT40CON		(*(volatile unsigned long *)0x11000E00)
#define rEXT_INT40_MASK		(*(volatile unsigned long *)0x11000F00)
#define rEXT_INT40_PEND		(*(volatile unsigned long *)0x11000F40)

void Key_Poll_Init(void)
{
	Macro_Write_Block(rGPX0CON,0xff,0x00,12);
}

int Key_Get_Key_Pressed(void)
{
	return Macro_Extract_Area(~rGPX0DAT, 0x3, 3);
}

void Key_Wait_Key_Released(void)
{
	while(Macro_Extract_Area(~rGPX0DAT, 0x3, 3) != 0);
}

int Key_Wait_Key_Pressed(void)
{
	int x;

	while((x = Macro_Extract_Area(~rGPX0DAT, 0x3, 3)) == 0);
	return x;
}

void Key_ISR_Init(void)
{
	Macro_Write_Block(rGPX0CON,0xff,0xff,12);
	Macro_Write_Block(rGPX0PUD,0xf,0x0,6);

	Macro_Write_Block(rEXT_INT40CON,0xff,0x22,12);
}
