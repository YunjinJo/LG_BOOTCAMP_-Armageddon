#include "device_driver.h"

void Uart1_Init(int baud)
{
	double 					d_div;
	volatile unsigned int	u_div;

	Macro_Write_Block(rCLK_SRC_PERIL0_OFFSET,0x1,0x0,24);
	Macro_Write_Block(rCLK_SRC_PERIL0_OFFSET,0xf,0x6,4);
	Macro_Write_Block(rCLK_DIV_PERIL0_OFFSET,0xf,0x7,4);

	Macro_Write_Block(rGPA0CON,0xff,0x22,16);

	rUBRDIV1 = u_div = d_div = ((double)SCLK_UART0/(baud*16))-1;
	rUFRACVAL1 = (unsigned int)((d_div - u_div)*16);

	rULCON1	= (0x0<<6)|(0x0<<3)|(0x0<<2)|(0x3<<0);
	rUCON1	= (0x2<<8)|(0x0<<4)|(0x1<<2)|(0x1<<0);
	rUFCON1	= 0x0;
	rUMCON1	= 0;
}

void Uart1_Send_Byte(char data)
{
	if(data == '\n')
	{
		while(Macro_Check_Bit_Clear(rUTRSTAT1,1));
	 	rUTXH1 = '\r';
	}
	while(Macro_Check_Bit_Clear(rUTRSTAT1,1));
	rUTXH1 = data;
}

void Uart1_Send_String(const char *pt)
{
	while(*pt) Uart1_Send_Byte(*pt++);
}

void Uart1_Printf(const char *fmt,...)
{
	va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart1_Send_String(string);
    va_end(ap);
}

char Uart1_Get_Char(void)
{
	while(Macro_Check_Bit_Clear(rUTRSTAT1,0));
	return rURXH1;
}

char Uart1_Get_Pressed(void)
{
	if(Macro_Check_Bit_Clear(rUTRSTAT1,0)) return 0;
	return rURXH1;
}

void Uart1_ISR_Enable(int rx,int tx, int err)
{
	Macro_Write_Block(rUINTM1,0xf,0xf,0);
	rUINTSP1 = 0xf;
	rUINTP1 = 0xf;
	GIC_Clear_Pending_Clear(0,85);
	Macro_Write_Block(rUINTM1,0xf,(1<<3)|((!tx)<<2)|((!err)<<1)|(!rx),0);

	if(rx||tx||err)
	{
		GIC_Set_Interrupt_Priority(0,85,0);
		GIC_Interrupt_Enable(0,85);
		GIC_Set_Processor_Target(0,85,1);
	}
	else
	{
		GIC_Interrupt_Disable(0,85);
	}
}

void Uart1_GetString(char *string)
{
    char *string2 = string;
    char c;
    while((c = Uart1_Get_Char())!='\r')
    {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                Uart_Printf("\b \b");
                string--;
            }
        }
        else
        {
            *string++ = c;
            Uart1_Send_Byte(c);
        }
    }
    *string='\0';
    Uart1_Send_Byte('\n');
}

int Uart1_GetIntNum(void)
{
    char str[30];
    char *string = str;
    int base     = 10;
    int minus    = 0;
    int result   = 0;
    int lastIndex;
    int i;

    Uart1_GetString(string);

    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }

    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }

    lastIndex = strlen(string) - 1;

    if(lastIndex<0)
        return -1;

    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha((int)string[i]))
            {
                if(isupper((int)string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}
