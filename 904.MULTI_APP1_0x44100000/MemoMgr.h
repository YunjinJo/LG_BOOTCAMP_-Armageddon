#ifndef _MEMOMGR_H_
#define _MEMOMGR_H_

#include "Device_Driver.h"
#include "./MemoList.h"

#define NULL ((void *)0)

typedef enum
{
	ADD = 1,
	READ,
	DELETE,
} ECommand;

void addFile(MemoList *memo, const char *filename, const char *content);
void deleteFile(MemoList *memo, const char *filename);
void printLines(char *content);
void writeFile(MemoList *memo, int index, ECommand caller);
void readFile(MemoList *memo, const char *filename);
void listFiles(const MemoList *memo);
void displayFiles(const MemoList *memo);
void printFileCnt(const MemoList *memo);
void writeFilename(MemoList *memo, char *filename, ECommand e_command);

#endif
