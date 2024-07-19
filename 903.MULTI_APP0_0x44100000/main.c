#include "Device_Driver.h"

//#include ".\images\image0.h"
//#include ".\images\img101.h"
#include ".\images\airplane.h"
//#include ".\images\go_board.h"
#include ".\images\go_board_2.h"
#include ".\images\white_rock.h"

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f
#define GO_BOARD 0x996633

#define DELAY	8000
#define LCD_WIDTH	1024
#define LCD_HEIGHT	600
#define GO_BOARD_SIZE 600

const unsigned short * img[] = { airplane, go_board_2, white_rock };



//void Draw_Line_With_Thickness(int x1, int y1, int x2, int y2, int color,
//		int thickness, int dir) {
//	int i = 0;
//	if (dir == 0) {
//		for (i = 0; i < thickness; i++) {
//			SVC_Lcd_Draw_Line(x1 + i, y1, x2 + i, y2, color);
//		}
//	} else
//		for (i = 0; i < thickness; i++) {
//			SVC_Lcd_Draw_Line(x1, y1 + i, x2, y2 + i, color);
//		}
//}
//
//void Draw_Board(){
//
//}

void Main(void) {
	SVC_Uart_Printf(">>APP0 => LCD Display\n");

	//ArrWinInfo[0].bpp_mode = BPPMODE_16BPP_565;
	//ArrWinInfo[0].bytes_per_pixel = 2;
	//ArrWinInfo[0].p_sizex = 1024;
	//ArrWinInfo[0].p_sizey = 600;
	//ArrWinInfo[0].v_sizex = 1024;
	//ArrWinInfo[0].v_sizey = 600;
	//ArrWinInfo[0].posx = (1024 - ArrWinInfo[0].p_sizex) / 2;
	//ArrWinInfo[0].posy = (600 - ArrWinInfo[0].p_sizey) / 2;

//	SVC_Lcd_Init();
//	SVC_Lcd_Win_Init(0, 1);
//	SVC_Lcd_Brightness_Control(8);
//
//	SVC_Lcd_Select_Display_Frame_Buffer(0, 0);
//	SVC_Lcd_Select_Draw_Frame_Buffer(0, 0);
//	SVC_Lcd_Clr_Screen();

//	for(;;)
//	{
	//SVC_Lcd_Draw_BMP(0,0,img[0]);
	//Delay(DELAY);
	//SVC_Lcd_Clr_Screen();
	SVC_Lcd_Draw_Back_Color(GO_BOARD);
//
//	Draw_Line_With_Thickness((LCD_WIDTH - GO_BOARD_SIZE) / 2, 0,
//			(LCD_WIDTH - GO_BOARD_SIZE) / 2, 600, BLACK, 4, 0);



//	SVC_Lcd_Draw_Line((LCD_WIDTH - GO_BOARD_SIZE) / 2, 0,
//			(LCD_WIDTH - GO_BOARD_SIZE) / 2, 600, BLACK);

	//void Lcd_Draw_Line(int x1,int y1,int x2,int y2,int color)

//	SVC_Lcd_Draw_BMP(0 ,0,img[1]);
	SVC_Lcd_Draw_BMP((LCD_WIDTH - GO_BOARD_SIZE)/2 ,(LCD_HEIGHT - GO_BOARD_SIZE)/2,img[1]);
	Delay(DELAY);
	SVC_Lcd_Draw_BMP((LCD_WIDTH - GO_BOARD_SIZE)/2 ,(LCD_HEIGHT - GO_BOARD_SIZE)/2,img[2]);
//		Delay(DELAY);
	//SVC_Lcd_Clr_Screen();

//		SVC_Lcd_Draw_BMP(0,0,img[2]);
//		Delay(DELAY);
//		SVC_Lcd_Clr_Screen();
//	}
//}
	for(;;);
}
