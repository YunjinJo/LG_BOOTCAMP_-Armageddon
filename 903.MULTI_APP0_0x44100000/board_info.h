#ifndef BOARD_INFO_H
#define BOARD_INFO_H

#define GO_BOARD_SPACE 60
#define GO_BOARD_OFFSET_X 27
#define GO_BOARD_OFFSET_Y 27
#define DELAY 8000
#define LCD_WIDTH 1024
#define LCD_HEIGHT 600
#define GO_BOARD_SIZE 600
#define MAX_XY 9
#define MAX_STONES 100
#define ROCK_OFFSET 25
#define WIN_LENGTH 5 // 승리 기준

#define BLACK 0x0000
#define WHITE 0xffff
#define BLUE 0x001f
#define GREEN 0x07e0
#define RED 0xf800
#define YELLOW 0xffe0
#define VIOLET 0xf81f
//#define GO_BOARD 0xf589
#define GO_BOARD 0x0000

typedef struct {
    unsigned int color;
    unsigned int x;
    unsigned int y;
} STONE;

extern int stone_arr_size;
extern STONE stone_arr[100];
extern unsigned short stone_arr_bool[100];

extern const unsigned short *img[];

void Add_Stone(STONE stone);
int Count_In_Direction(int x, int y, int dx, int dy, unsigned int color);
int Check_Validate(int x, int y);
int Check_Win(int x, int y, unsigned int color);
void Draw_Stone(STONE s);
void Draw_Board_State(void);
void Draw_Color_Dot(unsigned int x_pixel, unsigned int y_pixel, unsigned int cur_color);;
void Clean_Stones(void);
void Waiting_Mode(void);
void Draw_Winner(unsigned int cur_color);
void Draw_Invalidate_Spot(void);
void Draw_Manual();
void Get_Input(char *arr_input_p);

#endif // BOARD_INFO_H
