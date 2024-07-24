#include "device_driver.h"
#include "board_info.h"
#include "4412_addr.h"

void Main(void) {

	int cur_x = 4;
	int cur_y = 4;
	unsigned int cur_color = BLACK;
	unsigned short game_end = 0;
	unsigned int input_flag = 0;
	unsigned int screen_cleared = 0;
	unsigned int duplicated = 0;

	// init
	SVC_Lcd_Draw_Back_Color(GO_BOARD);
	Draw_Board_State();
	Draw_Color_Dot(GO_BOARD_OFFSET_X + cur_x * GO_BOARD_SPACE,
			GO_BOARD_OFFSET_Y + cur_y * GO_BOARD_SPACE, cur_color);

	for (;;) {
		input_flag = Get_Input_Flag();

		if (!input_flag) {
			if (!screen_cleared) {
				SVC_Lcd_Clr_Screen();
				Draw_Board_State();
				Draw_Color_Dot(GO_BOARD_OFFSET_X + cur_x * GO_BOARD_SPACE,
						GO_BOARD_OFFSET_Y + cur_y * GO_BOARD_SPACE, cur_color);
				screen_cleared = 1 - screen_cleared;
				Draw_Manual();

			}

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
				screen_cleared = 1 - screen_cleared;
				break;

			case 'r':
			case 'R':
				Clean_Stones();
				Draw_Board_State();
				break;
			case 13: //ENTER

				if (Check_Validate(cur_x, cur_y)) {
					Add_Stone((STONE) {cur_color,cur_x,cur_y});

						}

						else {
							Draw_Invalidate_Spot();
							duplicated = 1;
							break;
						}

						// 승자가 없으면 색 toggle, 있으면 game_end
							if (Check_Win(cur_x, cur_y, cur_color) == 0) {
								cur_color = (cur_color == BLACK) ? WHITE : BLACK;
							}
							else game_end = 1-game_end;
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
			}

			if (!(arr_input == 13 && duplicated)) {
				Draw_Board_State();
				Draw_Color_Dot(GO_BOARD_OFFSET_X + cur_x * GO_BOARD_SPACE,
						GO_BOARD_OFFSET_Y + cur_y * GO_BOARD_SPACE, cur_color);
				duplicated = 0;
			}
		}

		else // inputFlag == 1
			Waiting_Mode();
	}

}

