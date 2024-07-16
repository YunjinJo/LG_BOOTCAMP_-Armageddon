#include "device_driver.h"

void Timer0_Delay(int mtime)
{
	Macro_Write_Block(rTCFG0,0xff,(int)(TIMER_BASECLK/16/50000+0.5)-1,0);
	Macro_Write_Block(rTCFG1,0xf,4,0);

	rTCNTB0 = mtime*1000/TIMER_TICK;

	Macro_Write_Block(rTCON,0x1f,0x2,0);
	Macro_Write_Block(rTCON,0x1f,0x1,0);

	while(Macro_Check_Bit_Clear(rTINT_CSTAT,5));
	rTINT_CSTAT = 1<<5;
	Macro_Write_Block(rTCON,0x1f,0x0,0);
}

void Timer0_Int_Delay(int en, int mtime)
{
	if(en)
	{
		Macro_Write_Block(rTCFG0,0xff,(int)(TIMER_BASECLK/16./50000+0.5)-1,0);
		Macro_Write_Block(rTCFG1,0xf,4,0);

		rTCNTB0 = mtime * 50;

		Macro_Set_Bit(rTINT_CSTAT,0);

		Macro_Write_Block(rTCON,0x1f,0xa,0);
		Macro_Write_Block(rTCON,0x1f,0x9,0);

		GIC_Set_Interrupt_Priority(0,69,0);
		GIC_Interrupt_Enable(0,69);
		GIC_Set_Processor_Target(0,69,1);
	}

	else
	{
		GIC_Interrupt_Disable(0,69);
	}
}


