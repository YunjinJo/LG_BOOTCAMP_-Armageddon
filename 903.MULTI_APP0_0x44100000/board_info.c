#include "board_info.h"
#include "device_driver.h"
#include ".\images\go_board_2.h"
#include ".\images\white_rock.h"
#include ".\images\black_rock.h"

int stone_arr_size = 0;
STONE stone_arr[MAX_STONES];
unsigned short stone_arr_bool[MAX_STONES];

const unsigned short *img[] = { go_board_2, white_rock, black_rock };

void Add_Stone(STONE stone) {
	stone_arr[stone_arr_size++] = stone;
	if (stone.color == BLACK) {
		stone_arr_bool[stone.x * 10 + stone.y] = 3;
	} else {
		stone_arr_bool[stone.x * 10 + stone.y] = 2;
	}
}

void Clean_Stones() {
	stone_arr_size = 0;
	int i;
	for (i = 0; i < MAX_STONES; i++) {
		stone_arr_bool[i] = 0;
	}
}

int Count_In_Direction(int x, int y, int dx, int dy, unsigned int color) {
	int count = 0;
	if (color == BLACK)
		color = 3;
	if (color == WHITE)
		color = 2;
	while (stone_arr_bool[x * 10 + y] == color) {
		count++;
		x += dx;
		y += dy;
	}
	return count;
}

int Check_Validate(int x, int y) {
	if (!stone_arr_bool[x * 10 + y] == 0) {
		return 0;
	}
	return 1;
}

int Check_Win(int x, int y, unsigned int color) {
	int directions[4][2] = { { 1, 0 }, // 가로 방향
			{ 0, 1 }, // 세로 방향
			{ 1, 1 }, // 대각선 (\ 방향)
			{ 1, -1 } // 대각선 (/ 방향)
	};
	int d;

	for (d = 0; d < 4; ++d) {
		int dx = directions[d][0];
		int dy = directions[d][1];

		// 현재 위치를 기준으로 양쪽 방향을 검사
		int count = 1; // 현재 위치의 돌을 포함
		count += Count_In_Direction(x + dx, y + dy, dx, dy, color);
		count += Count_In_Direction(x - dx, y - dy, -dx, -dy, color);

		if (count >= WIN_LENGTH) {
			return 1;
		}
	}

	return 0;
}

void Draw_Stone(STONE s) {
	unsigned short rock = (s.color == BLACK) ? 2 : 1;

	SVC_Lcd_Draw_BMP(GO_BOARD_OFFSET_X + GO_BOARD_SPACE * s.x - ROCK_OFFSET,
			GO_BOARD_OFFSET_Y + GO_BOARD_SPACE * s.y - ROCK_OFFSET, img[rock]);
}

void Draw_Red_Dot(unsigned int x_pixel, unsigned int y_pixel) {
	SVC_Lcd_Draw_Bar(x_pixel - 10, y_pixel - 10, x_pixel + 10, y_pixel + 10,
			RED);
}

void Draw_Board_State() {
	SVC_Lcd_Draw_BMP(0, 0 / 2, img[0]);
	int i = 0;
	for (i = 0; i < stone_arr_size; i++) {
		Draw_Stone(stone_arr[i]);
	}
}

void Waiting_Mode() {
	Delay(DELAY);
	Lcd_Printf(700, 0, YELLOW, BLACK, 2, 2, "Omok App Waiting");
	Lcd_Printf(700, 40, YELLOW, BLACK, 2, 2, "Press '/' to switch");
	Delay(DELAY);
	Lcd_Printf(700, 0, BLACK, YELLOW, 2, 2, "Omok App Waiting");
	Lcd_Printf(700, 40, BLACK, YELLOW, 2, 2, "Press '/' to switch");
}

void Draw_Winner(unsigned int cur_color) {

	Lcd_Printf(LCD_WIDTH / 2 - 330, LCD_HEIGHT / 2 - 100, WHITE, BLACK, 2, 2,
			"WINNER : %s", cur_color == BLACK ? "BLACK" : "WHITE");
	Lcd_Printf(LCD_WIDTH / 2 - 380, LCD_HEIGHT / 2 - 65, WHITE, BLACK, 2, 2,
			"PRESS ANYTHING TO RESTART");
	SVC_Uart_Printf("\nGAME END! WINNER : %s\n",
			cur_color == BLACK ? "BLACK" : "WHITE\n");

}

void Get_Input(char *arr_input_p) {
	char val = 0;
	while (!val) {
		val = SVC_Uart1_Get_Pressed();
	}
	*arr_input_p = val;
}

