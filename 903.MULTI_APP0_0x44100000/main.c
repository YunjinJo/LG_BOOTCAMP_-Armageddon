#include "Device_Driver.h"
#include "board_info.h"
#include "4412_addr.h"

void Main(void) {
	Draw_Board_State();

	int cur_x = 4;
	int cur_y = 4;
	unsigned int cur_color = BLACK;
	unsigned short game_end = 0;
	unsigned int input_flag = 0;
	unsigned int screen_cleared = 0;

	SVC_Lcd_Draw_Back_Color(GO_BOARD);
	SVC_Uart_Printf(
			"===== MOVE : W/A/S/D , SET : ENTER , CHANGE APP : / =====\n\n");
	for (;;) {
		input_flag = Get_Input_Flag();

		if (!input_flag) {
			if (!screen_cleared) {
				SVC_Lcd_Clr_Screen();
				screen_cleared = 1 - screen_cleared;
			}
			Draw_Board_State();
			Draw_Red_Dot(GO_BOARD_OFFSET_X + cur_x * GO_BOARD_SPACE,
					GO_BOARD_OFFSET_Y + cur_y * GO_BOARD_SPACE);

			char arr_input = 0;
			Get_Input(&arr_input);

			switch (arr_input) {
			case 'w': //up
			case 'W': //up
				cur_y--;
				break;
			case 's': //down
			case 'S':
				cur_y++;
				break;
			case 'a': //left
			case 'A':
				cur_x--;
				break;
			case 'd': //right
			case 'D':
				cur_x++;
				break;
			case '/':
				Toggle_Input_Flag();
				input_flag = Get_Input_Flag();
				SVC_Uart_Printf("=====INPUT FLAG is %d\n", input_flag);
				screen_cleared = 1 - screen_cleared;
				break;

			case 13: //ENTER
				if (Check_Validate(cur_x, cur_y))
					Add_Stone((STONE) {cur_color,cur_x,cur_y});
							else {
								SVC_Uart_Printf("CHOOSE ANOTHER PLACE\n",input_flag);
					break;
				}

					// 승자가 없으면 색 toggle, 있으면 game_end
					if (Check_Win(cur_x, cur_y, cur_color) == 0) {
						cur_color = (cur_color == BLACK) ? WHITE : BLACK;
					} else {
						game_end = 1-game_end;
					}
					break;
					default:
					break;
				}

				// adjust xy
			cur_x = (cur_x > MAX_XY) ? MAX_XY : cur_x;
			cur_x = (cur_x < 0) ? 0 : cur_x;
			cur_y = (cur_y > MAX_XY) ? MAX_XY : cur_y;
			cur_y = (cur_y < 0) ? 0 : cur_y;

			// print winner on LCD & clean stones
			if (game_end) {
				Draw_Board_State();
				game_end = 1 - game_end;
				Draw_Winner(cur_color);
				Get_Input(&arr_input);
				Clean_Stones();
				SVC_Uart_Printf(
						"===== MOVE : W/A/S/D , SET : ENTER , CHANGE APP : / =====\n\n");
			}
		}

		else
			// inputFlag == 1
			Waiting_Mode();
	}

}

