#include "board_info.h"
#include "device_driver.h"
#include ".\images\airplane.h"
#include ".\images\go_board_2.h"
#include ".\images\white_rock.h"
#include ".\images\black_rock.h"

// 전역 변수 정의
int stone_arr_size = 0;
STONE stone_arr[100];
unsigned short stone_arr_bool[100];

const unsigned short *img[] = { airplane, go_board_2, white_rock, black_rock };
//const unsigned int arr_key[] = { 72, 80, 75, 77 };

void Add_Stone(STONE stone) {
    stone_arr[stone_arr_size++] = stone;
    if (stone.color == BLACK) {
        stone_arr_bool[stone.x * 10 + stone.y] = 3;
    } else {
        stone_arr_bool[stone.x * 10 + stone.y] = 2;
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
//void addStone(STONE_ARRAY *array, STONE stone) {
//    if (array->size >= array->capacity) {
//        resizeStoneArray(array, array->capacity * 2);
//    }
//    array->stones[array->size++] = stone;
//}
