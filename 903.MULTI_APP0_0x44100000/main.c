#include "Device_Driver.h"

#include ".\images\image0.h"
#include ".\images\img101.h"

extern WIN_INFO_ST ArrWinInfo[5];

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f

#define DELAY	8000

const unsigned short * img[]={image0, img101};

void Main(void)
{
	Uart_Printf(">>APP0 => LCD Display\n");

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
	Lcd_Clr_Screen();

	for(;;)
	{
		Lcd_Draw_BMP(0,0,img[0]);
		Delay(DELAY);
		Lcd_Clr_Screen();

		Lcd_Draw_BMP(0,0,img[1]);
		Delay(DELAY);
		Lcd_Clr_Screen();
	}
}
