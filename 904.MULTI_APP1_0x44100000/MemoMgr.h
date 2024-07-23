#ifndef _MEMOMGR_H_
#define _MEMOMGR_H_

#include "Device_Driver.h"
#include "./MemoList.h"

void addFile(MemoList *memo, const char *filename, const char *content);
void deleteFile(MemoList *memo, const char *filename);
void readFile(MemoList *memo, const char *filename);
void listFiles(const MemoList *memo);
void writeFile(MemoList * memo, int index, int caller);
void printLines(char * content);
void displayFiles(const MemoList *memo);

#endif
