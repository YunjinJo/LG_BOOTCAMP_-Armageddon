#include "./MemoMgr.h"

void addFile(MemoList *memo, const char *filename, const char *content) {
    if (memo->file_cnt < MAX_FILES) {
        strncpy(memo->files[memo->file_cnt].filename, filename, MAX_FILENAME_LENGTH);
        strncpy(memo->files[memo->file_cnt].content, content, MAX_CONTENT_LENGTH);
        memo->file_cnt++;
    } else {
        Uart_Printf("Maximum file limit reached.\n");
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
            Uart_Printf("File %s deleted.\n", filename);
            return;
        }
    }
    Uart_Printf("File not found.\n");
}

void readFile(const MemoList *memo, const char *filename) {
    unsigned int i;
    unsigned int command;
    char modified_content[MAX_CONTENT_LENGTH];
    for (i = 0; i < memo->file_cnt; ++i) {
        if (strcmp(memo->files[i].filename, filename) == 0) {
            // Uart_Printf("Content of %s:\n%s\n", filename, memo->files[i].content);
            Lcd_Clr_Screen();
            Lcd_Printf(100, 100, WHITE, BLACK, 2, 2, "<%s>", filename);
            Lcd_Printf(100, 150, WHITE, BLACK, 2, 2, "%s", memo->files[i].content);
            Lcd_Printf(850, 550, WHITE, BLACK, 1, 1, "0. Back");
            Lcd_Printf(850, 570, WHITE, BLACK, 1, 1, "1. Modify");
            
            while(1) {
                command = Uart1_GetIntNum();
                switch (command)
                {
                case 0:
                    return;
                    break;
                // case 1:
                //     Uart_Printf("Modify content: %s\n", memo->files[i].content);
				// 	Uart1_GetString(modified_content);
                //     deleteFile(&memo, filename);
                //     addFile(&memo, filename, modified_content);
                default:
                    // Uart_Printf("Invalid choice. Please try again.\n");
                    break;
                }
            }
        }
    }
    Uart_Printf("File not found.\n");
}

void listFiles(const MemoList *memo) {
    unsigned int i;
    for (i = 0; i < memo->file_cnt; ++i) {
        Uart_Printf("File %u: %s\n", i + 1, memo->files[i].filename);
    }
}

void displayFiles(const MemoList *memo) {
    Lcd_Draw_Back_Color(BLACK);

    unsigned int i;
    for (i = 0; i < memo->file_cnt; ++i) {
        Lcd_Printf(0, 40*i, WHITE, BLACK, 2, 2, "File %u: %s", i + 1, memo->files[i].filename);
    }
}

void printFileCnt(const MemoList *memo) {
    Uart_Printf("File Count: %d\n", memo->file_cnt);
}
