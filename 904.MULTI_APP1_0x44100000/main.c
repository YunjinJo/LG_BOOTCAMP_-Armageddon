#include "Device_Driver.h"
#include <stdio.h>
#include <string.h>
#include "./MemoMgr.h"

typedef struct{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
	unsigned int e;
	unsigned int f;
}TEST_T;

TEST_T t1 = {0, 1, 2, 3, 4, 5};
TEST_T t2 = {6, 7, 8, 9, 10, 11};
TEST_T t3 = {12, 13, 14, 15, 16, 17};

#define DELAY	8000

typedef enum {
	add = 1,
	read,
	delete,
} ECommand;

void writeFilename(MemoList* memo, char * filename, ECommand e_command) {
	char *command;
	switch (e_command)
	{
	case 1:
		command = "add";
		break;
	case 2:
		command = "read";
		break;
	case 3:
		command = "delete";
		break;
	
	default:
		break;
	}
	// Lcd_Clr_Screen();
	displayFiles(memo);
	// Lcd_Printf(100, 570, WHITE, BLACK, 1, 1, "Enter filename to %s: ", command);
	Lcd_Printf(0, 560, WHITE, BLACK, 2, 2, "Enter filename to %s: ", command);
	char x;
	while (1) {
        x = Uart1_Get_Char();
        Uart_Printf("input char : %c\n", x);
        
        if (x == '\b') { // Backspace key
            Uart_Printf("backspace entered\n");
            size_t len = strlen(filename);
            if (len > 0) {
                filename[len - 1] = '\0'; // Remove the last character
				// Lcd_Clr_Screen();
				displayFiles(memo);
				// Lcd_Printf(100, 570, WHITE, BLACK, 1, 1, "Enter filename to %s: ", command);
				// Lcd_Printf(300, 570, WHITE, BLACK, 1, 1, "%s", filename);
				Lcd_Printf(0, 560, WHITE, BLACK, 2, 2, "Enter filename to %s: ", command);
				Lcd_Printf(400, 560, WHITE, BLACK, 2, 2, "%s", filename);
            }
        } else if (x == '\r') { 
            return;
        } else { // Other characters
            size_t len = strlen(filename);
            if (len < MAX_FILENAME_LENGTH - 1) {
                filename[len] = x; // Add new character
                filename[len + 1] = '\0'; // Null-terminate the string
				// Lcd_Clr_Screen();
				displayFiles(memo);
				// Lcd_Printf(100, 570, WHITE, BLACK, 1, 1, "Enter filename to %s: ", command);
				// Lcd_Printf(300, 570, WHITE, BLACK, 1, 1, "%s", filename);
				Lcd_Printf(0, 560, WHITE, BLACK, 2, 2, "Enter filename to %s: ", command);
				Lcd_Printf(400, 560, WHITE, BLACK, 2, 2, "%s", filename);
            }
        }
	}
}

void Main(void)
{
	Uart_Printf(">>APP1 => UART Print, RO-BASE = 0x44100000 \n");

	MemoList memo = { .file_cnt = 0 };
    
	char choice;
    char content[MAX_CONTENT_LENGTH];

	unsigned int inputFlag = 0;

	for(;;)
	{
		inputFlag = Get_Input_Flag();
		char filename[MAX_FILENAME_LENGTH];
		filename[0] = '\0';

		// Uart_Printf("inputflag = %d\n", inputFlag);
		
		if (inputFlag) {
			displayFiles(&memo);

			// Lcd_Printf(850, 490, WHITE, BLACK, 1, 1, "<Manual>");
			// Lcd_Printf(850, 510, WHITE, BLACK, 1, 1, "1. Add File");
			// Lcd_Printf(850, 530, WHITE, BLACK, 1, 1, "2. Read File");
			// Lcd_Printf(850, 550, WHITE, BLACK, 1, 1, "3. Delete File");
			// Lcd_Printf(850, 570, WHITE, BLACK, 1, 1, "4. Close App");
			Lcd_Printf(740, 400, WHITE, BLACK, 2, 2, "<Manual>");
			Lcd_Printf(740, 440, WHITE, BLACK, 2, 2, "1. Add File");
			Lcd_Printf(740, 480, WHITE, BLACK, 2, 2, "2. Read File");
			Lcd_Printf(740, 520, WHITE, BLACK, 2, 2, "3. Delete File");
			Lcd_Printf(740, 560, WHITE, BLACK, 2, 2, "4. Close App");

			Uart_Printf("Enter your choice: ");
			
			choice = Uart1_Get_Char();
			ECommand command;
			switch (choice) {
				case '1':
					command = add;
					// Uart_Printf("%d. Add File\n", choice - '1' + 1);
					// Uart_Printf("Enter filename to add: ");
					// Uart1_GetString(filename);
					writeFilename(&memo, filename, command);
					// Uart_Printf("Enter content: ");
					// Uart1_GetString(content);
					addFile(&memo, filename, "");
					// Uart_Printf("filename: %s\ncontent: %s\n", filename, content);
					break;
				case '2':
					command = read;
					// Uart_Printf("%d. Read File\n", choice - '1' + 1);
					// Uart_Printf("Enter filename to read: ");
					// Lcd_Printf(100, 570, WHITE, BLACK, 1, 1, "Enter filename to read: ");
					// Lcd_Printf(0, 560, WHITE, BLACK, 2, 2, "Enter filename to read: ");
					// Uart1_GetString(filename);
					writeFilename(&memo, filename, command);
					readFile(&memo, filename);
					break;
				case '3':
					command = delete;
					// Uart_Printf("%d. Delete File\n", choice - '1' + 1);
					// Uart_Printf("Enter filename to delete: ");
					// Lcd_Printf(100, 570, WHITE, BLACK, 1, 1, "Enter filename to delete: ");	
					// Lcd_Printf(0, 560, WHITE, BLACK, 2, 2, "Enter filename to delete: ");	
					writeFilename(&memo, filename, command);
					deleteFile(&memo, filename);
					break;
				case '4':
					// Uart_Printf("%d. Close App\n", choice - '1' + 1);
					Toggle_Input_Flag();
					break;
				default:
					Uart_Printf("Invalid choice. Please try again.\n");
					break;
			}
		}

		// Lcd_Printf(600, 300, WHITE, BLACK, 1, 1, "test lcd_printf");
		// Lcd_Printf(600, 350, WHITE, BLACK, 1, 1, "test2 lcd_printf");
//		Delay(DELAY);
		// Uart_Printf("print many args %d%d%d%d%d%d\n", 1,2,3,4,5,6);
		// Delay(DELAY);
		// Uart_Printf("2\n");
		// Delay(DELAY);
		// Uart_Printf("3\n");
		// Delay(DELAY);
		// Uart_Printf("4\n");
		// Delay(DELAY);
		// Uart_Printf("5\n");
	}

}
