#include ".\images\airplane.h"
#include ".\images\go_board_2.h"
#include ".\images\white_rock.h"
#include ".\images\black_rock.h"

#define GO_BOARD_SPACE 60
#define GO_BOARD_OFFSET_X 27+(LCD_WIDTH-GO_BOARD_SIZE)/2
#define GO_BOARD_OFFSET_Y 27+(LCD_HEIGHT-GO_BOARD_SIZE)/2
#define MAX_STONES 100

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f
#define GO_BOARD 0xf589

#define DELAY	8000
#define LCD_WIDTH	1024
#define LCD_HEIGHT	600
#define GO_BOARD_SIZE 600
#define MAX_XY 9
#define ROCK_OFFSET 25
#define WIN_LENGTH 5

typedef struct {
	unsigned int color;
	unsigned int x;
	unsigned int y;
} STONE;

typedef struct {
	STONE *stones; // 동적 배열
	unsigned int size; // 현재 요소 개수
	unsigned int capacity; // 배열의 총 용량
} STONE_ARRAY;

int stone_arr_size;
STONE stone_arr[100];
unsigned short stone_arr_bool[100];

const unsigned short * img[] = { airplane, go_board_2, white_rock, black_rock };
const unsigned int arr_key[] = { 72, 80, 75, 77 }; // up down left right

void add_stone(STONE stone) {
	stone_arr[stone_arr_size++] = stone;
	if (stone.color == BLACK) {
		stone_arr_bool[stone.x * 10 + stone.y] = 3;
	} else {
		stone_arr_bool[stone.x * 10 + stone.y] = 2;
	}
}

int countInDirection(int x, int y, int dx, int dy, unsigned int color) {
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

int checkValidate(int x, int y) {
	if (!stone_arr_bool[x * 10 + y] == 0) {
		return 0;
	}
	return 1;
}

int checkWin(int x, int y, unsigned int color) {
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
		count += countInDirection(x + dx, y + dy, dx, dy, color);
		count += countInDirection(x - dx, y - dy, -dx, -dy, color);

		if (count >= WIN_LENGTH) {
			return 1;
		}
	}

	return 0;
}


void Draw_Stone(STONE s) {
	unsigned short rock = (s.color == BLACK) ? 3 : 2;

	SVC_Lcd_Draw_BMP(GO_BOARD_OFFSET_X + GO_BOARD_SPACE * s.x - ROCK_OFFSET,
	GO_BOARD_OFFSET_Y + GO_BOARD_SPACE * s.y - ROCK_OFFSET, img[rock]);
}

void Get_Board_State() {
	//SVC_Lcd_Clr_Screen();
	SVC_Lcd_Draw_Back_Color(GO_BOARD);
	SVC_Lcd_Draw_BMP((LCD_WIDTH - GO_BOARD_SIZE) / 2,
			(LCD_HEIGHT - GO_BOARD_SIZE) / 2, img[1]);
	int i = 0;
	for (i = 0; i < stone_arr_size; i++) {
		Draw_Stone(stone_arr[i]);
	}
}

void initStoneArray(STONE_ARRAY *array, unsigned int initialCapacity) {
	array->stones = (STONE *) malloc(initialCapacity * sizeof(STONE));
	array->size = 0;
	array->capacity = initialCapacity;
}

// STONE_ARRAY 메모리 해제 함수
void freeStoneArray(STONE_ARRAY *array) {
	free(array->stones);
	array->size = 0;
	array->capacity = 0;
}

// STONE_ARRAY 크기 조정 함수
void resizeStoneArray(STONE_ARRAY *array, unsigned int newCapacity) {
	STONE *newStones = (STONE *) realloc(array->stones,
			newCapacity * sizeof(STONE));
	array->stones = newStones;
	array->capacity = newCapacity;
}

// STONE_ARRAY에 STONE 요소 추가 함수
void addStone(STONE_ARRAY *array, STONE stone) {
	if (array->size >= array->capacity) {
		resizeStoneArray(array, array->capacity * 2);
	}
	array->stones[array->size++] = stone;
}
