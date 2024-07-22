#ifndef _MEMOMGR_H_
#define _MEMOMGR_H_

#include "Device_Driver.h"
#include "./MemoList.h"

void addFile(MemoList *memo, const char *filename, const char *content);
void deleteFile(MemoList *memo, const char *filename);
void readFile(const MemoList *memo, const char *filename);
void listFiles(const MemoList *memo);

#endif
