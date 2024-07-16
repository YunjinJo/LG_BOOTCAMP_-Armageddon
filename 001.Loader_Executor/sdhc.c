#include "device_driver.h"

#define SD_CMD_COMPLETE_CHECK()  while (Macro_Check_Bit_Set(rPRNSTS2,0))
#define SD_READ_BUFF_RDY_CHECK()  while (Macro_Check_Bit_Set(rPRNSTS2,11))
#define SD_RW_TRANSFER_COMPLETE_CHECK()  {while (Macro_Check_Bit_Set(rPRNSTS2,9)); while (Macro_Check_Bit_Set(rPRNSTS2,8)); }

volatile unsigned short sd_rca = 0;

#define SDHC_BLK_SIZE	512

void SDHC_Port_Init(void);
void SDHC_ISR_Enable(int en);
void SDHC_Clock_Supply(void);
void SDHC_Change_Dat_Width_4bit(void);
void SDHC_BusPower_Control(void);

void SDHC_CMD0(void);
void SDHC_CMD55(void);
unsigned int SDHC_ACMD41(void);
void SDHC_CMD2(void);
unsigned int SDHC_CMD8(void);
unsigned int SDHC_CMD3(void);
void SDHC_CMD7(unsigned int en);
void SDHC_ACMD6_4bit(void);

extern volatile unsigned int sd_insert_flag;
extern volatile unsigned int sd_command_complete_flag;
extern volatile unsigned int sd_rd_buffer_flag;
extern volatile unsigned int sd_wr_buffer_flag;
extern volatile unsigned int sd_tr_flag;

/*************************************************************************/
/* 	FREQ_MPLL = SCLK_MPLL_USER_T										 */
/*	SCLK_MPLL_USER_T / MMC2_RATIO / MMC2_PRE_RATIO = max 50Mhz			 */
/* 	Set value 800Mhz / (7+1) / (1+1) = 50Mhz							 */
/*************************************************************************/

void SDHC_Init(void)
{
	SDHC_Port_Init();
	Macro_Write_Block(rCLK_DIV_FSYS2, 0xF, 0x7, 0); // MMC2_RATIO
	Macro_Write_Block(rCLK_DIV_FSYS2, 0xFF, 0x1, 8); // MMC2_PRE_RATIO
	Macro_Set_Bit(rCLK_GATE_IP_FSYS, 7); // CLK_SDMMC2 enable(pass)
	Macro_Set_Area(rERRINTSTSEN2, 0x3FF, 0);
	sd_insert_flag = Macro_Check_Bit_Set(rPRNSTS2,16);
}

void SDHC_Card_Init(void)
{
	SDHC_Clock_Supply();
	Macro_Set_Bit(rPWRCON2, 0);
	rBLKSIZE2 = SDHC_BLK_SIZE;

	rTIMEOUTCON2 = 0xE;
	SDHC_CMD0();

	while (!SDHC_ACMD41());
	SDHC_CMD2();
	sd_rca = Macro_Extract_Area(SDHC_CMD3(),0xFFFF,16);
	SDHC_CMD7(1);

	SDHC_Change_Dat_Width_4bit();
}

void SDHC_Port_Init(void)
{
	Macro_Write_Block(rGPK2CON, 0xFFFFFFF, 0x2222222, 0);
	Macro_Write_Block(rGPK2DRV, 0x3FFF, 0x3FFF, 0);
	Macro_Write_Block(rGPK2PUD, 0x3FFF, 0, 0);
}

void SDHC_Clock_Supply(void)
{
	rTIMEOUTCON2 = 0xE;
	Macro_Set_Bit(rCLKCON2, 0);
	while (!Macro_Check_Bit_Set(rCLKCON2,1));
	Macro_Set_Bit(rCLKCON2, 2);
}

void SDHC_Clock_Stop(void)
{
	Macro_Clear_Bit(rCLKCON2, 0);
}

void SDHC_CMD0(void)
{
	int cmd = 0;

	while (Macro_Check_Bit_Set(rPRNSTS2,0));
	rARGUMENT2 = 0x0;
	rCMDREG2 = ((cmd & 0x3f) << 8) | (0x0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0x0 << 0);
	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;
	rERRINTSTS2 = (unsigned int)rERRINTSTS2;
}

unsigned int SDHC_CMD8(void)
{
	int cmd = 8;

	while (Macro_Check_Bit_Set(rPRNSTS2,0));

	rARGUMENT2 = 0x0;
	rCMDREG2 = ((cmd & 0x3f) << 8) | (0x0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0x0 << 0);
	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;
	rERRINTSTS2 = (unsigned int) rERRINTSTS2;

	return rRSPREG0_2;
}

void SDHC_CMD55(void)
{
	int cmd = 55;

	while (Macro_Check_Bit_Set(rPRNSTS2,0));
	rARGUMENT2 = sd_rca << 16;
	rCMDREG2 = ((cmd & 0x3f) << 8) | (0x0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0x2 << 0);
	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;
}

unsigned int SDHC_ACMD41(void)
{
	int cmd = 41;

	SDHC_CMD55();
	while (Macro_Check_Bit_Set(rPRNSTS2,0));
	rARGUMENT2 = 0xff8000;
	rCMDREG2 = ((cmd & 0x3f) << 8) | (0x0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0x2 << 0);
	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;

	return Macro_Check_Bit_Set(rRSPREG0_2,31);
}

void SDHC_CMD2(void)
{
	int cmd = 2;

	while (Macro_Check_Bit_Set(rPRNSTS2,0));
	sd_command_complete_flag = 0;
	rARGUMENT2 = 0x0;
	rCMDREG2 = ((cmd & 0x3f) << 8) | (0x0 << 6) | (0 << 5) | (0 << 4) | (1 << 3) | (0x1 << 0);
	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;
	rERRINTSTS2 = (unsigned int) rERRINTSTS2;
}

unsigned int SDHC_CMD3(void)
{
	int cmd = 3;

	while (Macro_Check_Bit_Set(rPRNSTS2,0));

	sd_command_complete_flag = 0;
	rARGUMENT2 = 0x0;
	rCMDREG2 = ((cmd & 0x3f) << 8) | (0x0 << 6) | (0 << 5) | (1 << 4) | (1 << 3) | (0x2 << 0);
	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;
	rERRINTSTS2 = (unsigned int) rERRINTSTS2;

	return rRSPREG0_2;
}

void SDHC_CMD7(unsigned int en)
{
	int cmd = 7;

	while (Macro_Extract_Area(rPRNSTS2,0x3,0));
	sd_command_complete_flag = 0;

	if (en)
	{
		rARGUMENT2 = (sd_rca << 16);
		rCMDREG2 = ((cmd & 0x3f) << 8) | (0x0 << 6) | (0 << 5) | (1 << 4) | (1 << 3) | (0x3 << 0);
	}

	else
	{
		rARGUMENT2 = 0;
		rCMDREG2 = ((cmd & 0x3f) << 8) | (0x0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0x0 << 0);
	}

	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;
	rERRINTSTS2 = (unsigned int) rERRINTSTS2;
}

void SDHC_ACMD6_4bit(void)
{
	int cmd = 6;

	SDHC_CMD55();
	while (Macro_Check_Bit_Set(rPRNSTS2,0));
	sd_command_complete_flag = 0;
	rARGUMENT2 = 0x2;
	rCMDREG2 = ((cmd & 0x3f) << 8) | (0x0 << 6) | (0 << 5) | (1 << 4) | (1 << 3) | (0x2 << 0);
	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;

	rERRINTSTS2 = (unsigned int) rERRINTSTS2;
}

void SDHC_ISR_Enable(int en)
{
	if (en)
	{
		Macro_Write_Block(rNORINTSTSEN2, 0x3, 0x3, 0);
		Macro_Write_Block(rNORINTSIGEN2, 0x3, 0x3, 0);
		Macro_Write_Block(rNORINTSTSEN2, 0xf, 0xf, 4);
		Macro_Write_Block(rNORINTSIGEN2, 0xf, 0xf, 4);
		GIC_Clear_Pending_Clear(0,107);
		GIC_Set_Interrupt_Priority(0, 107, 0);
		GIC_Interrupt_Enable(0, 107);
		GIC_Set_Processor_Target(0, 107, 1);
	}
	else
	{
		Macro_Write_Block(rNORINTSTSEN2, 0x3, 0x0, 0);
		Macro_Write_Block(rNORINTSIGEN2, 0x3, 0x0, 0);
		Macro_Write_Block(rNORINTSTSEN2, 0xf, 0x0, 4);
		Macro_Write_Block(rNORINTSIGEN2, 0xf, 0x0, 4);
		GIC_Interrupt_Disable(0,107);
	}
}

void SDHC_BusPower_Control(void)
{
	unsigned int tmp_capa;

	tmp_capa = rCAPAREG2;

	switch(Macro_Extract_Area(tmp_capa,0x7,24))
	{
		case 1:
		case 3:
		case 5:
		case 7:
			Macro_Write_Block(rPWRCON2,0x7,0x7,1);
			break;
		case 2:
		case 6:
			Macro_Write_Block(rPWRCON2,0x7,0x6,1);
			break;
		case 4:
			Macro_Write_Block(rPWRCON2,0x7,0x5,1);
			break;
	}

	Macro_Set_Bit(rPWRCON2, 0);
}

void SDHC_Change_Dat_Width_4bit(void)
{
	SDHC_ACMD6_4bit();
	Macro_Set_Bit(rHOSTCTL2, 1);
}

void SD_Read_Sector(int sec, int n, void *buf)
{
	int i;
	unsigned int* buf_read;

	buf_read = (unsigned int*) buf;
	rBLKCNT2 = n;
	while (Macro_Check_Bit_Set(rPRNSTS2,1));
	rARGUMENT2 = sec * SDHC_BLK_SIZE;
	sd_command_complete_flag = 0;

	if (n == 1)
	{
		rTRNMOD2 = (0 << 5) | (1 << 4);
		rCMDREG2 = ((17) << 8) | (0x0 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (0x2 << 0);
	}

	else
	{
		rTRNMOD2 = (1 << 5) | (1 << 4) | (1 << 2) | (1 << 1);
		rCMDREG2 = ((18) << 8) | (0x0 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (0x2 << 0);
	}

	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;
	rERRINTSTS2 = (unsigned int) rERRINTSTS2;

	while (n--)
	{
		while (!sd_rd_buffer_flag);
		sd_rd_buffer_flag = 0;
		while (Macro_Check_Bit_Clear(rPRNSTS2,11));

		for (i = 0; i < (SDHC_BLK_SIZE / 4); i++)
		{
			*buf_read++ = rBDATA2;
		}
	}

	while (!sd_tr_flag);
	sd_tr_flag = 0;
}

void SD_Write_Sector(int sec, int n, void *buf)
{
	int i;
	unsigned int *buf_wr;

	buf_wr = (unsigned int*) buf;
	rBLKCNT2 = n;
	while (Macro_Check_Bit_Set(rPRNSTS2,1));
	rARGUMENT2 = sec * SDHC_BLK_SIZE;
	sd_command_complete_flag = 0;

	if (n == 0)
	{
		rTRNMOD2 = (0 << 5) | (0 << 4) | (1 << 2) | (1 << 1);
		rCMDREG2 = ((24) << 8) | (0x0 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (0x2 << 0);
	}
	else
	{
		rTRNMOD2 = (1 << 5) | (0 << 4) | (1 << 2) | (1 << 1);
		rCMDREG2 = ((25) << 8) | (0x0 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (0x2 << 0);
	}

	while (!sd_command_complete_flag);
	sd_command_complete_flag = 0;
	rERRINTSTS2 = (unsigned int) rERRINTSTS2;

	while (n--)
	{
		while(!sd_wr_buffer_flag);
		sd_wr_buffer_flag = 0;
		while(Macro_Check_Bit_Clear(rPRNSTS2,10));

		for (i = 0; i < (SDHC_BLK_SIZE / 4); i++)
		{
			rBDATA2 = *buf_wr++;
		}
	}

	while (!sd_tr_flag);
	sd_tr_flag = 0;
	rERRINTSTS2 = (unsigned int) rERRINTSTS2;
}
