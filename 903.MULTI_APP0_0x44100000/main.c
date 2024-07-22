#include "Device_Driver.h"
#include "board_info.h"

#define DELAY	8000
#define LCD_WIDTH	1024
#define LCD_HEIGHT	600
#define GO_BOARD_SIZE 600
#define MAX_XY 9
#define ROCK_OFFSET 25

void Main(void) {
			Get_Board_State();

			int cur_x = 0;
			int cur_y = 0;
			unsigned int cur_color = BLACK;
			unsigned short game_end = 0;

			for (;;) {

				int cur_x_pixel = GO_BOARD_OFFSET_X + cur_x*GO_BOARD_SPACE;
				int cur_y_pixel = GO_BOARD_OFFSET_Y + cur_y*GO_BOARD_SPACE;
				SVC_Lcd_Draw_Bar(cur_x_pixel - 10, cur_y_pixel - 10, cur_x_pixel+ 10, cur_y_pixel + 10, RED);

				SVC_Uart_Printf("=====MOVE : W/A/S/D , SET : ENTER=====\n\n");
				char arr_input;
				arr_input = SVC_Uart1_Get_Char();

				switch (arr_input) {
					case 'w': //up
					SVC_Uart_Printf("UP CLICKED\n");
					cur_y--;
					break;
					case 's'://down
					SVC_Uart_Printf("DOWN CLICKED\n");
					cur_y++;;
					break;
					case 'a'://left
					SVC_Uart_Printf("LEFT CLICKED\n");
					cur_x --;
					break;
					case 'd'://right
					SVC_Uart_Printf("RIGHT CLICKED\n");
					cur_x ++;
					break;
					case 13://ENTER
					SVC_Uart_Printf("ENTER CLICKED\n");
					if (Check_Validate(cur_x,cur_y)==0){
						SVC_Uart_Printf("Choose Another Spot!\n");

						break;
					}

					Add_Stone((STONE) {cur_color,cur_x,cur_y});
					if(Check_Win(cur_x, cur_y, cur_color)==0){
						cur_color = (cur_color==BLACK) ? WHITE : BLACK;
					}
					else{
						game_end = 1;
					}
					break;
					default:
					break;
				}
				if(game_end == 1){
					Get_Board_State();
					SVC_Uart_Printf("GAME END! WINNER : %s\n", cur_color==BLACK? "BLACK" : "WHITE");
					break;
				}

				// 이전 state 가져옴
				Get_Board_State();
				if(cur_x>MAX_XY) cur_x = MAX_XY;
				if(cur_y>MAX_XY) cur_y = MAX_XY;
				if(cur_x<0) cur_x = 0;
				if(cur_y<0) cur_y = 0;
			}

			for (;;)
			;
		}
