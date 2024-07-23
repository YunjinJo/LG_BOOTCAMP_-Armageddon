#include "./MemoMgr.h"

void addFile(MemoList *memo, const char *filename, const char *content) {
    if (memo->file_cnt < MAX_FILES) {
        strncpy(memo->files[memo->file_cnt].filename, filename, MAX_FILENAME_LENGTH);
        strncpy(memo->files[memo->file_cnt].content, content, MAX_CONTENT_LENGTH);
        memo->file_cnt++;
        writeFile(memo, memo->file_cnt-1, ADD);
    } else {
        // Uart_Printf("Maximum file limit reached.\n");
        Lcd_Printf(0, 560, WHITE, BLACK, 2, 2, "Maximum file limit reached.");
    }
}

void deleteFile(MemoList *memo, const char *filename) {
    unsigned int i;
    unsigned int j;
    for (i = 0; i < memo->file_cnt; ++i) {
        if (strcmp(memo->files[i].filename, filename) == 0) {
            for (j = i; j < memo->file_cnt - 1; ++j) {
                memo->files[j] = memo->files[j + 1];
            }
            memo->file_cnt--;
            // Uart_Printf("File %s deleted.\n", filename);
            return;
        }
    }
    // Uart_Printf("File not found.\n");
    // Lcd_Printf(0, 560, WHITE, BLACK, 2, 2, "File not found.");
}

void printLines(char * content) 
{
    // Display content in chunks of 20 characters
    int line_height = 30; // Adjust this as needed
    int start_index = 0;
    while (start_index < strlen(content)) {
        char line[MAX_CONTENT_LENGTH];
        int length = 50; // Number of characters per line
        if (strlen(content) - start_index < length) {
            length = strlen(content) - start_index;
        }
        strncpy(line, content + start_index, length);
        line[length] = '\0'; // Ensure null-termination

        Lcd_Printf(100, 150 + (start_index / 50) * line_height, WHITE, BLACK, 2, 2, "%s", line);
        start_index += 50;
    }
}

void writeFile(MemoList * memo, int index, ECommand command)
{
    char new_content[MAX_CONTENT_LENGTH];
    
    // Initialize new_content with current content
    strncpy(new_content, memo->files[index].content, MAX_CONTENT_LENGTH);
    new_content[MAX_CONTENT_LENGTH - 1] = '\0'; // Ensure null-termination

    // Display the current content
    Lcd_Clr_Screen();
    Lcd_Printf(100, 100, WHITE, BLACK, 2, 2, "<%s>", memo->files[index].filename);
    // Lcd_Printf(100, 150, WHITE, BLACK, 2, 2, "%s", new_content);
    printLines(new_content);
    // Lcd_Printf(850, 550, WHITE, BLACK, 1, 1, ":q    Cancel Changes");
    // Lcd_Printf(850, 570, WHITE, BLACK, 1, 1, ":w    Save Changes");
    Lcd_Printf(20, 560, WHITE, BLACK, 2, 2, "Insert Mode");
    Lcd_Printf(680, 520, WHITE, BLACK, 2, 2, ":q  Cancel Changes");
    Lcd_Printf(680, 560, WHITE, BLACK, 2, 2, ":w  Save Changes");
    
    char x;
    while (1) {
        x = 0;
        // x = Uart1_Get_Char();
        while (!x) {
            x = Uart1_Get_Pressed();
        }
        // Uart_Printf("input char : %c\n", x);
        
        if (x == '\x1B') {
            if (command == ADD)
                deleteFile(memo, memo->files[index].filename);
            return;
        } else if (x == '\b') { // Backspace key
            // Uart_Printf("backspace entered\n");
            size_t len = strlen(new_content);
            if (len > 0) {
                new_content[len - 1] = '\0'; // Remove the last character
                // Update LCD
                Lcd_Clr_Screen();
                Lcd_Printf(100, 100, WHITE, BLACK, 2, 2, "<%s>", memo->files[index].filename);
                // Lcd_Printf(100, 150, WHITE, BLACK, 2, 2, "%s", new_content);
                printLines(new_content);
                // Lcd_Printf(850, 550, WHITE, BLACK, 1, 1, ":q    Cancel Changes");
                // Lcd_Printf(850, 570, WHITE, BLACK, 1, 1, ":w    Save Changes");
                Lcd_Printf(20, 560, WHITE, BLACK, 2, 2, "Insert Mode");
                Lcd_Printf(680, 520, WHITE, BLACK, 2, 2, ":q  Cancel Changes");
                Lcd_Printf(680, 560, WHITE, BLACK, 2, 2, ":w  Save Changes");
            }
        } else if (x == ':') { 
            // Lcd_Printf(100, 510, WHITE, BLACK, 1, 1, "Command Mode");
            // Lcd_Printf(100, 530, WHITE, BLACK, 1, 1, "Click q to cancel changes");
            // Lcd_Printf(100, 550, WHITE, BLACK, 1, 1, "Click w to save changes");
            // Lcd_Printf(100, 570, WHITE, BLACK, 1, 1, "Click other to return to edit mode");
            Lcd_Clr_Screen();
            Lcd_Printf(100, 100, WHITE, BLACK, 2, 2, "<%s>", memo->files[index].filename);
            printLines(new_content);
            Lcd_Printf(400, 480, WHITE, BLACK, 2, 2, "Click 'q' to cancel changes");
            Lcd_Printf(400, 520, WHITE, BLACK, 2, 2, "Click 'w' to save changes");
            Lcd_Printf(400, 560, WHITE, BLACK, 2, 2, "Click other to return to Insert mode");
            Lcd_Printf(20, 560, WHITE, BLACK, 2, 2, "Command Mode");
            x = 0;
            // x = Uart1_Get_Char();
            while (!x) {
                x = Uart1_Get_Pressed();
            }
            if (x == 'q') {
                if (command == ADD) {
                    deleteFile(memo, memo->files[index].filename);
                }
                return;
            } else if (x == 'w') { 
                break;
            } else {
                Lcd_Clr_Screen();
                Lcd_Printf(100, 100, WHITE, BLACK, 2, 2, "<%s>", memo->files[index].filename);
                printLines(new_content);
                Lcd_Printf(20, 560, WHITE, BLACK, 2, 2, "Insert Mode");
                Lcd_Printf(680, 520, WHITE, BLACK, 2, 2, ":q  Cancel Changes");
                Lcd_Printf(680, 560, WHITE, BLACK, 2, 2, ":w  Save Changes");
            }
        } else { // Other characters
            size_t len = strlen(new_content);
            if (len < MAX_CONTENT_LENGTH - 1) {
                new_content[len] = x; // Add new character
                new_content[len + 1] = '\0'; // Null-terminate the string
                // Update LCD
                Lcd_Clr_Screen();
                Lcd_Printf(100, 100, WHITE, BLACK, 2, 2, "<%s>", memo->files[index].filename);
                // Lcd_Printf(100, 150, WHITE, BLACK, 2, 2, "%s", new_content);
                printLines(new_content);
                // Lcd_Printf(850, 550, WHITE, BLACK, 1, 1, ":q    Cancel Changes");
                // Lcd_Printf(850, 570, WHITE, BLACK, 1, 1, ":w    Save Changes");
                Lcd_Printf(20, 560, WHITE, BLACK, 2, 2, "Insert Mode");
                Lcd_Printf(680, 520, WHITE, BLACK, 2, 2, ":q  Cancel Changes");
                Lcd_Printf(680, 560, WHITE, BLACK, 2, 2, ":w  Save Changes");
            }
        }
    }

    // Save modified content back to memo->files[i].content
    strncpy(memo->files[index].content, new_content, MAX_CONTENT_LENGTH - 1);
    memo->files[index].content[MAX_CONTENT_LENGTH - 1] = '\0'; // Ensure null-termination
    // Uart_Printf("Content has been modified.\n");

    // Update the LCD with the new content
    // Lcd_Clr_Screen();
    // Lcd_Printf(100, 100, WHITE, BLACK, 2, 2, "<%s>", memo->files[index].filename);
    // Lcd_Printf(100, 150, WHITE, BLACK, 2, 2, "%s", memo->files[i].content);
    // printLines(memo->files[index].content);
    // Lcd_Printf(850, 550, WHITE, BLACK, 1, 1, ":q    Cancel Changes");
    // Lcd_Printf(850, 570, WHITE, BLACK, 1, 1, ":w    Save Changes");    
    // Lcd_Printf(680, 520, WHITE, BLACK, 2, 2, ":q  Cancel Changes");
    // Lcd_Printf(680, 560, WHITE, BLACK, 2, 2, ":w  Save Changes");
}

void readFile(MemoList *memo, const char *filename) {
    int i;
    char command;
    for (i = 0; i < memo->file_cnt; ++i) {
        if (strcmp(memo->files[i].filename, filename) == 0) {
            Lcd_Clr_Screen();
            Lcd_Printf(100, 100, WHITE, BLACK, 2, 2, "<%s>", memo->files[i].filename);
            printLines(memo->files[i].content);
            // Lcd_Printf(850, 550, WHITE, BLACK, 1, 1, "0. Back");
            // Lcd_Printf(850, 570, WHITE, BLACK, 1, 1, "1. Modify");
            Lcd_Printf(780, 520, WHITE, BLACK, 2, 2, "0. Back");
            Lcd_Printf(780, 560, WHITE, BLACK, 2, 2, "1. Modify");
            while (1) {
                command = 0;
                // command = Uart1_Get_Char();
                while (!command) {
                    command = Uart1_Get_Pressed();
                }
                switch (command)
                {
                case '0':
                    return;
                case '1':
                    writeFile(memo, i, READ);    
                    return;
                default:
                    // Uart_Printf("Invalid choice. Please try again.\n");
                    break;
                }
            }
        }
    }
    // Uart_Printf("File not found.\n");
    // Lcd_Printf(0, 560, WHITE, BLACK, 2, 2, "File not found.");
}

void listFiles(const MemoList *memo) {
    unsigned int i;
    for (i = 0; i < memo->file_cnt; ++i) {
        Uart_Printf("File %u: %s\n", i + 1, memo->files[i].filename);
    }
}

void displayFiles(const MemoList *memo) {
    // Lcd_Draw_Back_Color(BLACK);
    Lcd_Clr_Screen();
    // Lcd_Clr_Screen_left_half();

    unsigned int i;
    for (i = 0; i < memo->file_cnt; ++i) {
        // Lcd_Printf(0, 40*i, WHITE, BLACK, 2, 2, "File %u: %s", i + 1, memo->files[i].filename);
        // Lcd_Printf(0, 40*i, BLACK, YELLOW, 2, 2, "%s", memo->files[i].filename);
        Lcd_Printf(0, 40*i, WHITE, BLACK, 2, 2, "%s", memo->files[i].filename);
    }
}

void printFileCnt(const MemoList *memo) {
    Uart_Printf("File Count: %d\n", memo->file_cnt);
}

void writeFilename(MemoList* memo, char * filename, ECommand e_command) {
	char *command;
	switch (e_command)
	{
	case ADD:
		command = "add";
		break;
	case READ:
		command = "read";
		break;
	case DELETE:
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
        x = 0;
        // x = Uart1_Get_Char();
        while (!x) {
            x = Uart1_Get_Pressed();
        }
        // Uart_Printf("input char : %c\n", x);
        
        if (x == '\x1B') {
            return;
        } else if (x == '\b') { // Backspace key
            // Uart_Printf("backspace entered\n");
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
