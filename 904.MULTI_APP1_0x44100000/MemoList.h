#ifndef _MEMOLIST_H_
#define _MEMOLIST_H_

#define MAX_FILES 15
#define MAX_FILENAME_LENGTH 50
#define MAX_CONTENT_LENGTH 500

typedef struct
{
    char filename[MAX_FILENAME_LENGTH];
    char content[MAX_CONTENT_LENGTH];
} File;

typedef struct
{
    File files[MAX_FILES];
    unsigned int file_cnt;
} MemoList;

#endif
