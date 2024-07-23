#include "Device_Driver.h"
#include "board_info.h"

void Main(void) {
	Draw_Board_State();

	int cur_x = 4;
	int cur_y = 4;
	unsigned int cur_color = BLACK;
	unsigned short game_end = 0;
	unsigned int input_flag = 0;

	SVC_Lcd_Draw_Back_Color(GO_BOARD);
	SVC_Uart_Printf(
			"===== MOVE : W/A/S/D , SET : ENTER , CHANGE APP : Q =====\n\n");
	for (;;) {

		input_flag = Get_Input_Flag();

//		if(input_flag)
//		{
//			Credit_Mode();
//		}


		if (!input_flag) {
			Draw_Board_State();
			Draw_Red_Dot(GO_BOARD_OFFSET_X + cur_x * GO_BOARD_SPACE,
			GO_BOARD_OFFSET_Y + cur_y * GO_BOARD_SPACE);

			char arr_input;
			arr_input = SVC_Uart1_Get_Char();
//			SVC_Uart_Printf("choice: %c\n", arr_input);
			switch (arr_input) {
			case 'w': //up
			case 'W': //up
//				SVC_Uart_Printf("UP CLICKED\n");
				cur_y--;
				break;
			case 's': //down
			case 'S':
//				SVC_Uart_Printf("DOWN CLICKED\n");
				cur_y++;
				break;
			case 'a': //left
			case 'A':
//				SVC_Uart_Printf("LEFT CLICKED\n");
				cur_x--;
				break;
			case 'd': //right
			case 'D':
//				SVC_Uart_Printf("RIGHT CLICKED\n");
				cur_x++;
				break;
			case 'q':
			case 'Q':
				Toggle_Input_Flag();
				input_flag = Get_Input_Flag();
				SVC_Uart_Printf("=====INPUT FLAG is %d\n", input_flag);
				break;
//			case 'c':
//			case 'C':
//				Credit_Mode();
//				break;

			case 13: //ENTER
//				SVC_Uart_Printf("ENTER CLICKED\n");
				if (Check_Validate(cur_x, cur_y))
					Add_Stone((STONE) {cur_color,cur_x,cur_y});
							else {
								SVC_Uart_Printf("CHOOSE ANOTHER PLACE\n",input_flag);
					break;
				}

					if (Check_Win(cur_x, cur_y, cur_color) == 0) {
						cur_color = (cur_color == BLACK) ? WHITE : BLACK;
					} else {
						game_end = 1-game_end;
					}
					break;

					default:
					break;
				}

				// 이전 state 가져옴
			cur_x = (cur_x > MAX_XY) ? MAX_XY : cur_x;
			cur_x = (cur_x < 0) ? 0 : cur_x;
			cur_y = (cur_y > MAX_XY) ? MAX_XY : cur_y;
			cur_y = (cur_y < 0) ? 0 : cur_y;

			if (game_end) {
				Draw_Board_State();
				game_end = 1 - game_end;
				Lcd_Printf(LCD_WIDTH / 2 - 200, LCD_HEIGHT / 2 - 150, WHITE,
						BLACK, 2, 2, "WINNER : %s",
						cur_color == BLACK ? "BLACK" : "WHITE");
				Lcd_Printf(LCD_WIDTH / 2 - 200, LCD_HEIGHT / 2 - 120, WHITE,
						BLACK, 2, 2, "PRESS ANYTHING TO RESTART");
				SVC_Uart_Printf("\nGAME END! WINNER : %s\n",
						cur_color == BLACK ? "BLACK" : "WHITE\n");

				arr_input = SVC_Uart1_Get_Char();
				Clean_Stones();
				SVC_Uart_Printf(
						"===== MOVE : W/A/S/D , SET : ENTER , CHANGE APP : Q =====\n\n");
			}

		}
	}

}

//void Credit_Mode() {
//	unsigned int height = LCD_HEIGHT / 2;
//	for (;;) {
//
//		SVC_Lcd_Clr_Screen();
//		Lcd_Printf(LCD_WIDTH / 2 - 200, height, WHITE, BLACK, 2, 2,
//				"THANK YOU");
//		height -= 20;
//		Delay(DELAY);
//	}
//
//}
