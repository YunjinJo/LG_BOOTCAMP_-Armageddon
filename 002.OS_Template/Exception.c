#include "device_driver.h"
#include "global.h"
#include "demand_page.h"

void Undef_Handler(unsigned int addr, unsigned int mode)
{
	Uart_Printf("UND-Exception @[0x%X]\nMode[0x%X]\n", addr, mode);
	Uart_Printf("Undefined Code Value[0x%X]\n", *(unsigned int *)addr);
	for(;;);
}

void Dabort_Handler(unsigned int addr, unsigned int mode)
{
	unsigned int r, d, s, w, sd;

	Uart_Printf("DABT-Exception @[0x%X]\nMode[0x%X]\n", addr, mode);
	Uart_Printf("DABT-Fault Address[0x%X]\n", DABT_Falut_Address());
	sd = DABT_Falut_Status();
	r = Macro_Extract_Area(sd, 0xf, 0);
	d = Macro_Extract_Area(sd, 0xf, 4);
	s = Macro_Extract_Area(sd, 0x1, 10);
	w = Macro_Extract_Area(sd, 0x1, 11);
	sd = Macro_Extract_Area(sd, 0x1, 12);
	r += (s << 4);
	Uart_Printf("Reason[0x%X]\nDomain[0x%X]\nRead(0)/Write(1)[%d]\nAXI-Decode(0)/Slave(1)[%d]\n", r, d, w, sd);
}

void Pabort_Handler(unsigned int addr, unsigned int mode)
{
	unsigned int r, s, sd;

	Uart_Printf("PABT-Exception @[0x%X]\nMode[0x%X]\n", addr, mode);
	Uart_Printf("PABT-Fault Address[0x%X]\n", PABT_Falut_Address());
	sd = PABT_Falut_Status();
	r = Macro_Extract_Area(sd, 0xf, 0);
	s = Macro_Extract_Area(sd, 0x1, 10);
	sd = Macro_Extract_Area(sd, 0x1, 12);
	r += (s << 4);
	Uart_Printf("Reason[0x%X]\nAXI-Decode(0)/Slave(1)[%d]\n", r, sd);
	for(;;);
}

unsigned int Get_Input_Flag(void)
{
	return INPUT_FLAG;
}

void Toggle_Input_Flag(void) 
{
    INPUT_FLAG = 1 - INPUT_FLAG;
}

void * SVC_Handler[] = {
	(void *) Lcd_Clr_Screen,					//0
	(void *) Lcd_Draw_BMP,						//1
	(void *) Uart_Printf,						//2
	(void *) Lcd_Init,							//3
	(void *) Lcd_Win_Init,						//4
	(void *) Lcd_Brightness_Control,			//5
	(void *) Lcd_Select_Display_Frame_Buffer,	//6
	(void *) Lcd_Select_Draw_Frame_Buffer,		//7
	(void *) Lcd_Draw_Back_Color,				//8
	(void *) Lcd_Draw_Line,						//9
	(void *) Lcd_Printf,						//10
	(void *) Uart1_GetIntNum,					//11
	(void *) Uart1_GetString,					//12
	(void *) Lcd_Clr_Screen, 					//13
	(void *) Get_Input_Flag, 					//14
	(void *) Toggle_Input_Flag, 				//15
	(void *) Lcd_Draw_Bar,        				//16
	(void *) Uart1_Get_Char,      				//17
	(void *) Lcd_Clr_Screen_left_half, 			//18
	(void *) Uart1_Get_Pressed, 				//19
	};

void Invalid_ISR(void);
void Uart1_ISR(void);
void Timer0_ISR(void);
void Key3_ISR(void);
void Key4_ISR(void);
void SDHC_ISR(void);

void (*ISR_Vector[])(void) =
{
		Invalid_ISR,		// 0
		Invalid_ISR,		// 1
		Invalid_ISR,		// 2
		Invalid_ISR,		// 3
		Invalid_ISR,		// 4
		Invalid_ISR,		// 5
		Invalid_ISR,		// 6
		Invalid_ISR,		// 7
		Invalid_ISR,		// 8
		Invalid_ISR,		// 9
		Invalid_ISR,		// 10
		Invalid_ISR,		// 11
		Invalid_ISR,		// 12
		Invalid_ISR,		// 13
		Invalid_ISR,		// 14
		Invalid_ISR,		// 15
		Invalid_ISR,		// 16
		Invalid_ISR,		// 17
		Invalid_ISR,		// 18
		Invalid_ISR,		// 19
		Invalid_ISR,		// 20
		Invalid_ISR,		// 21
		Invalid_ISR,		// 22
		Invalid_ISR,		// 23
		Invalid_ISR,		// 24
		Invalid_ISR,		// 25
		Invalid_ISR,		// 26
		Invalid_ISR,		// 27
		Invalid_ISR,		// 28
		Invalid_ISR,		// 29
		Invalid_ISR,		// 30
		Invalid_ISR,		// 31
		Invalid_ISR,		// 32
		Invalid_ISR,		// 33
		Invalid_ISR,		// 34
		Invalid_ISR,		// 35
		Invalid_ISR,		// 36
		Invalid_ISR,		// 37
		Invalid_ISR,		// 38
		Invalid_ISR,		// 39
		Invalid_ISR,		// 40
		Invalid_ISR,		// 41
		Invalid_ISR,		// 42
		Invalid_ISR,		// 43
		Invalid_ISR,		// 44
		Invalid_ISR,		// 45
		Invalid_ISR,		// 46
		Invalid_ISR,		// 47
		Invalid_ISR,		// 48
		Invalid_ISR,		// 49
		Invalid_ISR,		// 50
		Key3_ISR,			// 51
		Key4_ISR,			// 52
		Invalid_ISR,		// 53
		Invalid_ISR,		// 54
		Invalid_ISR,		// 55
		Invalid_ISR,		// 56
		Invalid_ISR,		// 57
		Invalid_ISR,		// 58
		Invalid_ISR,		// 59
		Invalid_ISR,		// 60
		Invalid_ISR,		// 61
		Invalid_ISR,		// 62
		Invalid_ISR,		// 63
		Invalid_ISR,		// 64
		Invalid_ISR,		// 65
		Invalid_ISR,		// 66
		Invalid_ISR,		// 67
		Invalid_ISR,		// 68
		context_save,			// 69
		Invalid_ISR,		// 70
		Invalid_ISR,		// 71
		Invalid_ISR,		// 72
		Invalid_ISR,		// 73
		Invalid_ISR,		// 74
		Invalid_ISR,		// 75
		Invalid_ISR,		// 76
		Invalid_ISR,		// 77
		Invalid_ISR,		// 78
		Invalid_ISR,		// 79
		Invalid_ISR,		// 80
		Invalid_ISR,		// 81
		Invalid_ISR,		// 82
		Invalid_ISR,		// 83
		Invalid_ISR,		// 84
		Uart1_ISR,			// 85
		Invalid_ISR,		// 86
		Invalid_ISR,		// 87
		Invalid_ISR,		// 88
		Invalid_ISR,		// 89
		Invalid_ISR,		// 90
		Invalid_ISR,		// 91
		Invalid_ISR,		// 92
		Invalid_ISR,		// 93
		Invalid_ISR,		// 94
		Invalid_ISR,		// 95
		Invalid_ISR,		// 96
		Invalid_ISR,		// 97
		Invalid_ISR,		// 98
		Invalid_ISR,		// 99
		Invalid_ISR,		// 100
		Invalid_ISR,		// 101
		Invalid_ISR,		// 102
		Invalid_ISR,		// 103
		Invalid_ISR,		// 104
		Invalid_ISR,		// 105
		Invalid_ISR,		// 106
		SDHC_ISR,			// 107
		Invalid_ISR,		// 108
		Invalid_ISR,		// 109
		Invalid_ISR,		// 110
		Invalid_ISR,		// 111
		Invalid_ISR,		// 112
		Invalid_ISR,		// 113
		Invalid_ISR,		// 114
		Invalid_ISR,		// 115
		Invalid_ISR,		// 116
		Invalid_ISR,		// 117
		Invalid_ISR,		// 118
		Invalid_ISR,		// 119
};

void Invalid_ISR(void)
{
	Uart1_Printf("Invalid_ISR\n");
}

volatile unsigned int sd_insert_flag = 0;
volatile unsigned int sd_command_complete_flag = 0;
volatile unsigned int sd_rd_buffer_flag = 0;
volatile unsigned int sd_wr_buffer_flag = 0;
volatile unsigned int sd_tr_flag = 0;

void SDHC_ISR(void)
{
	volatile unsigned int tmp;

	tmp = rNORINTSTS2;
	rNORINTSTS2 = tmp;

	if((tmp & (1 << 6)) || (tmp & (1 << 7)))
	{
		if (Macro_Check_Bit_Set(rPRNSTS2,16)) sd_insert_flag = 1;
		else sd_insert_flag = 0;
	}


	if(tmp & (1 << 5)) sd_rd_buffer_flag = 1;
	if(tmp & (1 << 4)) sd_wr_buffer_flag = 1;
	if(tmp & (1 << 1)) sd_tr_flag = 1;
	if(tmp & 1) sd_command_complete_flag = 1;

	GIC_Clear_Pending_Clear(0,107);
	GIC_Write_EOI(0, 107);
}

void Uart1_ISR(void)
{
	rUINTSP1 = 0xf;
	rUINTP1 = 0xf;

	GIC_Clear_Pending_Clear(0,85);
	GIC_Write_EOI(0, 85);

	Uart1_Printf("Uart1 => %c\n", rURXH1);
}

void Key3_ISR(void)
{
	rEXT_INT40_PEND = 0x1<<3;

	Uart1_Printf("Key3 Pressed\n");


	GIC_Clear_Pending_Clear(0,51);
	GIC_Write_EOI(0, 51);
  
	Toggle_Input_Flag();
}

void Key4_ISR(void)
{
	rEXT_INT40_PEND = 0x1<<4;

	Uart1_Printf("Key4 Pressed\n");

	GIC_Clear_Pending_Clear(0,52);
	GIC_Write_EOI(0, 52);
  
	Toggle_Input_Flag();
}

void Timer0_ISR(void)
{
	static int value = 0;
	rTINT_CSTAT |= ((1<<5)|1);
	GIC_Clear_Pending_Clear(0,69);
	GIC_Write_EOI(0, 69);


	LED_Display(value);
	value = (value + 1) % 4;

	switch (Get_ASID()){
		case 1:
		{
			CoSetTTBase((0x44080000 |(1<<6)|(1<<3)|(0<<1)|(0<<0)));
			CoSetASID(2);

			sel_reg_info = reg_info_app1;
		}
			break;
		case 2:
		{
			CoSetTTBase((0x44000000 |(1<<6)|(1<<3)|(0<<1)|(0<<0)));
			CoSetASID(1);

			sel_reg_info = reg_info_app0;
		}
			break;
	}
	Get_Context_And_Switch();

}
