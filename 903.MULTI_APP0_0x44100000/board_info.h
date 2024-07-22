#ifndef BOARD_INFO_H
#define BOARD_INFO_H


#define GO_BOARD_SPACE 60
#define GO_BOARD_OFFSET_X 27+(LCD_WIDTH-GO_BOARD_SIZE)/2
#define GO_BOARD_OFFSET_Y 27+(LCD_HEIGHT-GO_BOARD_SIZE)/2
#define MAX_STONES 100

#define BLACK 0x0000
#define WHITE 0xffff
#define BLUE 0x001f
#define GREEN 0x07e0
#define RED 0xf800
#define YELLOW 0xffe0
#define VIOLET 0xf81f
#define GO_BOARD 0xf589

#define DELAY 8000
#define LCD_WIDTH 1024
#define LCD_HEIGHT 600
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

extern int stone_arr_size;
extern STONE stone_arr[100];
extern unsigned short stone_arr_bool[100];

extern const unsigned short *img[];
//extern const unsigned int arr_key[];

void Add_Stone(STONE stone);
int Count_In_Direction(int x, int y, int dx, int dy, unsigned int color);
int Check_Validate(int x, int y);
int Check_Win(int x, int y, unsigned int color);
void Draw_Stone(STONE s);
void Get_Board_State();
void initStoneArray(STONE_ARRAY *array, unsigned int initialCapacity);
void freeStoneArray(STONE_ARRAY *array);
void resizeStoneArray(STONE_ARRAY *array, unsigned int newCapacity);
//void addStone(STONE_ARRAY *array, STONE stone);

#endif // BOARD_INFO_H
