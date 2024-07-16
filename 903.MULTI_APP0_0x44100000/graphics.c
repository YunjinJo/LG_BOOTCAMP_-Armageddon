#include "Device_Driver.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// LCD Frame Buffer
#define KB(n)					(n<<10)
#define MB(n)					(n<<20)

#define LCD_FB_SIZE_ALIGN		(MB(16))		// 16[MB]

#define HFP			(0x1D)
#define HBP			(0x1D)
#define HSW			(0x1D)
#define VFP			(3)
#define VFPE		(0)
#define VBP			(2)
#define VBPE		(0)
#define VSW			(2)

#define RISE_VCLK	(1)
#define INV_HSYNC	(1)
#define INV_VSYNC	(0)
#define INV_VDEN	(0)

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f

unsigned int ArrFbSel[10]	={LCD_FB00_START_ADDR, LCD_FB01_START_ADDR,LCD_FB10_START_ADDR,LCD_FB11_START_ADDR,LCD_FB20_START_ADDR,LCD_FB21_START_ADDR,LCD_FB30_START_ADDR,LCD_FB31_START_ADDR,LCD_FB40_START_ADDR,LCD_FB41_START_ADDR};
WIN_INFO_ST	 ArrWinInfo[5];

unsigned int Selected_win = 0;
unsigned int Selected_frame = 0;
unsigned int Display_frame[2] = {0,0};

#define winosdna(x)			(*(volatile unsigned int*)(0x11C00040+(0x10*(x))))
#define winosdnb(x)			(*(volatile unsigned int*)(0x11C00044+(0x10*(x))))
#define winosdnc(x)			(*(volatile unsigned int*)(0x11C00048+(0x10*(x))))
#define winosdnd(x)			(*(volatile unsigned int*)(0x11C0004C+(0x10*(x))))
#define winwnadd0(x)		(*(volatile unsigned int*)(0x11C000A0+(0x8*(x))))
#define winwnadd1(x)		(*(volatile unsigned int*)(0x11C000D0+(0x8*(x))))
#define winwnadd2(x)		(*(volatile unsigned int*)(0x11C00100+(0x4*(x))))
#define winwnadd0b1(x)		(*(volatile unsigned int*)(0x11C000A4+(0x8*(x))))
#define winwnadd1b1(x)		(*(volatile unsigned int*)(0x11C000D4+(0x8*(x))))
#define winconn(x)			(*(volatile unsigned int*)(0x11C00020+(0x4*(x))))
#define wnkeycon0(x)		(*(volatile unsigned int*)(0x11C00140+(((x)-1)*0x8)))
#define wnkeycon1(x)		(*(volatile unsigned int*)(0x11C00144+(((x)-1)*0x8)))
#define blendeqn(x)			(*(volatile unsigned int*)(0x11C00244+(((x)-1)*0x4)))
#define vidwnalpha0(x)		(*(volatile unsigned int*)(0x11C0021C+(((x)*0x8))))
#define vidwnalpha1(x)		(*(volatile unsigned int*)(0x11C00220+(((x)*0x8))))

void udelay_f(unsigned long usec)
{
	unsigned long loops = usec *300;

	for(;loops>0;loops--);
}

volatile unsigned short *pLcdFb=(volatile unsigned short *)0;

void LCD_Clock_Init(void)
{
	Macro_Clear_Bit(rCLK_GATE_IP_LCD,0);

	Macro_Write_Block(rCLK_SRC_LCD0,0xf,0x6,0);
	Macro_Set_Bit(rCLK_SRC_MASK_LCD,0);
	Macro_Write_Block(rCLK_DIV_LCD,0xf,4,0);

	while(Macro_Check_Bit_Set(rCLK_DIV_STAT_LCD,0));
	Macro_Set_Bit(rCLK_GATE_IP_LCD,0);
}

void Lcd_Init(void)
{
	LCD_Clock_Init();

	Macro_Write_Block(rGPF0CON,0xffffffff,0x22222222,0);
	Macro_Write_Block(rGPF1CON,0xffffffff,0x22222222,0);
	Macro_Write_Block(rGPF2CON,0xffffffff,0x22222222,0);
	Macro_Write_Block(rGPF3CON,0xffff,0x2222,0);

	Macro_Write_Block(*(volatile unsigned long *)(&rGPF0CON+2),0xffff,0x0,0);
	Macro_Write_Block(*(volatile unsigned long *)(&rGPF1CON+2),0xffff,0x0,0);
	Macro_Write_Block(*(volatile unsigned long *)(&rGPF2CON+2),0xffff,0x0,0);
	Macro_Write_Block(*(volatile unsigned long *)(&rGPF3CON+2),0xff,0x0,0);

	//LCD_ENABLE		: GPX0CON[7]
	Macro_Write_Block(rGPX0CON,0xf,0x1,28);
	Macro_Set_Bit(rGPX0DAT,7);

	//LCD_BL_CTL		: GPD0CON[0] - XpwmTOUT0
	Macro_Write_Block(rGPD0CON,0xf,0x2,0);

	//rSMMU_CON |= (1<<13);
	Macro_Set_Bit(rLCDBLK_CFG,1);

	rVIDCON0 = (0<<26)|(0<<18)|(1<<17)|(0<<16)|(3<<6)|(0<<5)|(1<<4); // RGB Interface[28:26],RGB parallel format[18],Normal RGB Order[17],CLKVAL_F update Always[16],
	rVIDCON1 = (1<<9)|(RISE_VCLK << 7) | (INV_HSYNC<<6)|(INV_VSYNC<<5)|(INV_VDEN<<4);
	rVIDCON2 = 0;
	rVIDCON3 = 0;

	rVIDTCON0 = (VBPE<<24)|(VBP<<16)|(VFP<<8)|(VSW<<0);
	rVIDTCON1 = (VFPE<<24)|(HBP<<16)|(HFP<<8)|(HSW<<0);
	rVIDTCON2 = ((HEIGHT-1)<<11)|((WIDTH-1)<<0);
	rVIDTCON3 = (0<<31);

	//LCD_ENABLE		: GPX0CON[7]
	Macro_Write_Block(rGPX0CON,0xf,0x1,28);
	Macro_Set_Bit(rGPX0DAT,7);

	//LCD_BL_CTL		: GPD0CON[0] - XpwmTOUT0
	Macro_Write_Block(rGPD0CON,0xf,0x2,0);

	Macro_Write_Block(rVIDCON0,0x3,0x3,0); //Display ON
	Macro_Write_Block(rSHADOWCON,0x1f,0x1f,0);
}

void Lcd_Win_Init(int id,int en)
{
	int fb_size;
	int off_size;
	int page_width;

	fb_size 	= ArrWinInfo[id].v_sizex*ArrWinInfo[id].v_sizey*ArrWinInfo[id].bytes_per_pixel;
	off_size 	= (ArrWinInfo[id].v_sizex - ArrWinInfo[id].p_sizex) * ArrWinInfo[id].bytes_per_pixel;
	page_width 	= ArrWinInfo[id].p_sizex * ArrWinInfo[id].bytes_per_pixel;

	winosdna(id) = ((ArrWinInfo[id].posx&0x7ff)<<11)|(ArrWinInfo[id].posy&0x7ff);
	winosdnb(id) = (((ArrWinInfo[id].posx + ArrWinInfo[id].p_sizex-1)&0x7ff)<<11)|((ArrWinInfo[id].posy+ArrWinInfo[id].p_sizey-1)&0x7ff);

	if(id == 0)
	{
		winosdnc(id) = (ArrWinInfo[id].p_sizex * ArrWinInfo[id].p_sizey)/2;
	}
	else if((id == 1)||(id == 2))
	{
		winosdnd(id) = (ArrWinInfo[id].p_sizex * ArrWinInfo[id].p_sizey);
	}

	winwnadd0(id) = ArrFbSel[id*2];
	winwnadd1(id) = ArrFbSel[id*2]+fb_size;
	winwnadd2(id) = (((off_size) & 0x1fff)<<13)|((page_width) & 0x1fff);

	if(id == 0 || id == 1)
	{
		winwnadd0b1(id)  = ArrFbSel[id*2+1];
		winwnadd1b1(id) = ArrFbSel[id*2]+fb_size;
	}

	winconn(id) = (0<<22)|(1<<16)|(2<<9)|(ArrWinInfo[id].bpp_mode<<2);

	if(en) Macro_Set_Bit(winconn(id),0);
	else Macro_Clear_Bit(winconn(id),0);
}

void Lcd_Put_Pixel(int x,int y, unsigned short color)
{
	*(pLcdFb+x+ArrWinInfo[Selected_win].v_sizex*y)=color;
}

unsigned int Lcd_Get_Pixel(int x,int y)
{
	return *(pLcdFb+x+ArrWinInfo[Selected_win].v_sizex*y);
}

void * Lcd_Get_Pixel_Address(int x,int y)
{
	return (void *)(pLcdFb+x+ArrWinInfo[Selected_win].v_sizex*y);
}

void Lcd_Clr_Screen(void)
{
	int x, y;

	for (y = 0; y < ArrWinInfo[Selected_win].v_sizey; y++)
	{
		for (x = 0; x < ArrWinInfo[Selected_win].v_sizex; x++)
		{
			Lcd_Put_Pixel(x, y, 0x00);
		}
	}
}

void Lcd_Draw_Back_Color(int color)
{
     int x,y;

     for(y=0;y<ArrWinInfo[Selected_win].v_sizey;y++)
     {
         for(x=0;x<ArrWinInfo[Selected_win].v_sizex;x++)
         {
             	Lcd_Put_Pixel(x,y,color);
         }
     }
}

void Lcd_Get_Info_BMP(int * x, int  * y, const unsigned short int *fp)
{
	*x =(int)fp[0];
	*y =(int)fp[1];
}

void Lcd_Draw_BMP(int x, int y, const unsigned short int *fp)
{
	register int width = fp[0], height = fp[1];
	register int xx, yy;

	for(yy=0;yy<height;yy++)
	{
		for(xx=0;xx<width;xx++)
		{
			 Lcd_Put_Pixel(xx+x,yy+y,(int)fp[yy*width+xx+2]);
		}
	}
}

void Lcd_Draw_Image(int x, int y, const unsigned short int *fp, int width, int height)
{
	register int xx, yy;

	for(yy=0;yy<height;yy++)
	{
		for(xx=0;xx<width;xx++)
		{
			Lcd_Put_Pixel(xx+x,yy+y,(int)fp[yy*width+xx]);
		}
	}
}

void Lcd_Select_Draw_Frame_Buffer(int win_id,int buf_num)
{
	pLcdFb = (volatile unsigned short *)ArrFbSel[win_id*2+buf_num];

	Selected_win = win_id;
	Selected_frame = buf_num;
}

void Lcd_Select_Display_Frame_Buffer(int win_id,int buf_num)
{
	if(buf_num == 0)
	{
		Macro_Clear_Bit(winconn(win_id),20);
	}
	else
	{
		Macro_Set_Bit(winconn(win_id),20);
	}

   	Display_frame[win_id] = buf_num;
}

__inline double absf(double data)
{
	return (0 <= data ? data : -data);
}

void Lcd_Brightness_Control(int level)
{
	Macro_Write_Block(rGPD0CON,0xf,0x1,0);
	Macro_Set_Bit(rGPD0DAT, 0);

	/*
	Macro_Write_Block(rTCFG0,0xff,(int)(TIMER_BASECLK/16/50000+0.5)-1,0);
	Macro_Write_Block(rTCFG1,0xf,4,0);

	rTCNTB0 = 10*1000/TIMER_TICK;
	rTCMPB0 = level*999/TIMER_TICK;

	Macro_Set_Bit(rTINT_CSTAT,0);

	Macro_Write_Block(rTCON,0x1f,0xa,0);
	Macro_Write_Block(rTCON,0x1f,0x9,0);
	*/
}

void Lcd_Draw_BMP_File_24bpp(int x, int y, void *fp)
{
	// 24bpp. 비압축 모드만 지원
	int xx, yy, p;
	unsigned char * t;

	unsigned char * raw;
	unsigned int w;
	unsigned int h;
	unsigned int pad;

	t = (unsigned char *)((unsigned int)fp + 0xA);
	raw = (unsigned char *)(t[0]+(t[1]<<8)+(t[2]<<16)+(t[3]<<24)+(unsigned int)fp);

	t = (unsigned char *)((unsigned int)fp + 0x12);
	w = (unsigned int)(t[0]+(t[1]<<8)+(t[2]<<16)+(t[3]<<24));

	t = (unsigned char *)((unsigned int)fp + 0x16);
	h = (unsigned int)(t[0]+(t[1]<<8)+(t[2]<<16)+(t[3]<<24));

	pad = (4-(w*3)%4)%4;

	Uart_Printf("fp=%#x, raw=%#x, w=%d, h=%d\n", fp, raw, w, h);

	for(yy=(h-1);yy>=0;yy--)
	{
		for(xx=0;xx<w;xx++)
		{
			p=(int)(((raw[0]&0xf8)>>3)+((raw[1]&0xfc)<<3)+((raw[2]&0xf8)<<8));
			Lcd_Put_Pixel(xx+x,yy+y,p);
			raw += 3;
		}
		raw = (unsigned char *)((unsigned int)raw + pad);
	}
}


static unsigned char _first[]={0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
static unsigned char _middle[]={0,0,0,1,2,3,4,5,0,0,6,7,8,9,10,11,0,0,12,13,14,15,16,17,0,0,18,19,20,21};
static unsigned char _last[]={0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,0,17,18,19,20,21,22,23,24,25,26,27};
static unsigned char cho[]={0,0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0};
static unsigned char cho2[]={0,5,5,5,5,5,5,5,5,6,7,7,7,6,6,7,7,7,6,6,7,5};
static unsigned char jong[]={0,0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1};

#include "ENG8X16.H"
#include "HAN16X16.H"
#include "HANTABLE.H"

#define 	ENG_FONT_X 		8
#define 	ENG_FONT_Y 		16

#define COPY(A,B) 	for(loop=0;loop<32;loop++) *(B+loop)=*(A+loop);
#define OR(A,B) 	for(loop=0;loop<32;loop++) *(B+loop)|=*(A+loop);

void Lcd_Han_Putch(int x,int y,int color,int bkcolor, int data, int zx, int zy)
{
	unsigned int first,middle,last;
	unsigned int offset,loop;
	unsigned char xs,ys, cx, cy;
	unsigned char temp[32];
	unsigned char bitmask[]={128,64,32,16,8,4,2,1};
	int newColor;

	first  = (unsigned)((data>>8)&0x00ff);	// 상위 바이트
	middle = (unsigned)(data&0x00ff);	// 하위 바이트
	offset = (first-0xA1)*(0x5E)+(middle-0xA1);

	first  = *(HanTable+offset*2);
	middle = *(HanTable+offset*2+1);
	data   = (int)((first<<8)+middle);

	first  = _first[(data>>10)&31];
	middle = _middle[(data>>5)&31];
	last   = _last[(data)&31];

	if(last==0)
	{
		offset=(unsigned)(cho[middle]*640);
		offset+=first*32;
		COPY(han16x16+offset,temp);

		if(first==1||first==24) offset=5120;
		else offset=5120+704;
		offset+=middle*32;
		OR(han16x16+offset,temp);
	}
	else
	{
		offset=(unsigned)(cho2[middle]*640);
		offset+=first*32;
		COPY(han16x16+offset,temp);

		if(first==1||first==24) offset=5120+704*2;
		else offset=5120+704*3;
		offset+=middle*32;
		OR(han16x16+offset,temp);

		offset=(unsigned)(5120+2816+jong[middle]*896);
		offset+=last*32;
		OR(han16x16+offset,temp);
	}

	for(ys=0;ys<16;ys++)
	{
		for(xs=0;xs<8;xs++)
		{
			newColor = ((temp[ys*2]&bitmask[xs]) ? color : bkcolor);
			for(cy=0; cy<zy; cy++)
			{
				for(cx=0; cx<zx; cx++)
				{
					Lcd_Put_Pixel(x+zx*xs+cx, y+zy*ys+cy, newColor);
				}
			}

			newColor = ((temp[ys*2+1]&bitmask[xs]) ? color : bkcolor);
			for(cy=0; cy<zy; cy++)
			{
				for(cx=0; cx<zx; cx++)
				{
					Lcd_Put_Pixel(x+zx*(xs+8)+cx, y+zy*ys+cy, newColor);
				}
			}
		}
	}
}

void Lcd_Eng_Putch(int x,int y,int color,int bkcolor,int data, int zx, int zy)
{
	unsigned offset,loop;
	unsigned char ys, xs, cy, cx;
	unsigned char temp[32];
	unsigned char bitmask[]={128,64,32,16,8,4,2,1};
	int newColor;

	offset=(unsigned)(data*16);
	COPY(eng8x16+offset,temp);

	for(ys=0;ys<16;ys++)
	{
		for(xs=0;xs<8;xs++)
		{
			newColor = ((temp[ys]&bitmask[xs]) ? color : bkcolor);
			for(cy=0; cy<zy; cy++)
			{
				for(cx=0; cx<zx; cx++)
				{
					Lcd_Put_Pixel(x+zx*xs+cx, y+zy*ys+cy, newColor);
				}
			}
		}
	}
}

void Lcd_Puts(int x, int y, int color, int bkcolor, char *str, int zx, int zy)
{
     unsigned data;

     while(*str)
     {
        data=*str++;
        if(data>=128)
        {
             data*=256;
             data|=*str++;
             Lcd_Han_Putch(x, y, color, bkcolor, (int)data, zx, zy);
             x+=zx*16;
        }
        else
        {
             Lcd_Eng_Putch(x, y, color, bkcolor, (int)data, zx, zy);
             x+=zx*ENG_FONT_X;
        }
     }
}

void Lcd_Draw_Bar(int x1, int y1, int x2, int y2, int color)
{
     int i, j;
     int xx1, yy1, xx2, yy2;

     if(x1<x2)
     {
     	xx1=x1;
     	xx2=x2;
     }
     else
     {
     	xx1=x2;
     	xx2=x1;
     }

     if(y1<y2)
     {
     	yy1=y1;
     	yy2=y2;
     }
     else
     {
     	yy1=y2;
     	yy2=y1;
     }

     for(i=yy1;i<=yy2;i++)
     {
         for(j=xx1;j<=xx2;j++)
         {
             Lcd_Put_Pixel(j,i,color);
         }
     }
}

void Lcd_Draw_Line(int x1,int y1,int x2,int y2,int color)
{
	double y=y1, x=x1;
	double dy=y2-y1, dx=x2-x1;
	double ey, ex;

	if(absf(dy)<absf(dx))
	{
		ey = dy / absf(dx);
		ex = (0<dx) ? 1 : -1;

		while(x!=x2)
		{
			Lcd_Put_Pixel((int)x, (int)y, color);
			y += ey;
			x += ex;
		}
		Lcd_Put_Pixel((int)x, (int)y, color);
	}

	else
	{
		ey = (0<dy) ? 1 : -1;
		ex = dx / absf(dy);

		while(y!=y2)
		{
			Lcd_Put_Pixel((int)x, (int)y, color);
			y += ey;
			x += ex;
		}
		Lcd_Put_Pixel((int)x, (int)y, color);
	}
}


void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...)
{
	va_list ap;
	char string[256];

	va_start(ap,fmt);
	vsprintf(string,fmt,ap);
	Lcd_Puts(x, y, color, bkcolor, string, zx, zy);
	va_end(ap);
}

void Lcd_Draw_STACK(void){
	//stack test, 현재 sp, 현재 stack guard, 지금까지 guard 개수, error상황(overflow, timer)
	Lcd_Printf(0,0,WHITE, BLACK,3,3,"APP0 STACK test", 1);
}
