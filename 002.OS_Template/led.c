#include "device_driver.h"

void LED_Init(void)
{
	Macro_Write_Block(rGPM4CON, 0xff, 0x11, 16);
	LED_Display(0);
}

void LED_Display(int led)
{
	Macro_Write_Block(rGPM4DAT, 0x3, (led) & 0x3, 4);
}


