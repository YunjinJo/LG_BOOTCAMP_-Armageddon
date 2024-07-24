#include "device_driver.h"
#include <stdio.h>
#include <string.h>
#include "./MemoMgr.h"

#define DELAY 8000

void Main(void)
{
	Uart_Printf(">>APP1 => UART Print, RO-BASE = 0x44100000 \n");

	// 메모장 초기화
	MemoList memo = {.file_cnt = 0};

	// 메모장 파일 1개 디폴트 생성
	char t[10] = "Hello";
	char c[50] = "We are ARMaggedon. Feel free to use our apps.";
	strncpy(memo.files[0].filename, t, 10);
	strncpy(memo.files[0].content, c, 50);
	memo.file_cnt++;

	unsigned int inputFlag = 0;

	for (;;)
	{
		char choice = 0;
		inputFlag = Get_Input_Flag();
		char filename[MAX_FILENAME_LENGTH];
		filename[0] = '\0';

		if (inputFlag)
		{
			displayFiles(&memo);
			Lcd_Printf(740, 440, WHITE, BLACK, 2, 2, "<Notepad Manual>");
			Lcd_Printf(740, 480, WHITE, BLACK, 2, 2, "1. Add Note");
			Lcd_Printf(740, 520, WHITE, BLACK, 2, 2, "2. Read Note");
			Lcd_Printf(740, 560, WHITE, BLACK, 2, 2, "3. Delete Note");

			while (!choice)
			{
				choice = Uart1_Get_Pressed();
			}
			Uart_Printf("get pressed: %c\n", choice);
			ECommand command;
			switch (choice)
			{
			case '1':
				command = ADD;
				writeFilename(&memo, filename, command);
				addFile(&memo, filename, "");
				break;
			case '2':
				command = READ;
				writeFilename(&memo, filename, command);
				readFile(&memo, filename);
				break;
			case '3':
				command = DELETE;
				writeFilename(&memo, filename, command);
				deleteFile(&memo, filename);
				break;
			case '/':
				Toggle_Input_Flag();
				break;
			default:
				break;
			}
		}
		else
		{ // inputFlag == 0
			Delay(DELAY);
			Lcd_Printf(700, 0, GREEN, BLACK, 2, 2, "Notepad App Waiting");
			Lcd_Printf(700, 40, GREEN, BLACK, 2, 2, "Press '/' to switch");
			Delay(DELAY);
			Lcd_Printf(700, 0, BLACK, GREEN, 2, 2, "Notepad App Waiting");
			Lcd_Printf(700, 40, BLACK, GREEN, 2, 2, "Press '/' to switch");
		}
	}
}
