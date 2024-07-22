#pragma once
#include "type_define.h"

#define DEMAND_PAGE_START_ADDR (0x44B00000)
#define MAX_PAGE (16)
#define FOUR_KB (0x1000)
#define ONE_KB (0x400)

typedef struct{
    unsigned int asid;
    unsigned int VA;
    unsigned int *second_TT_addr;
} PAGE_INFO_T;

extern void demand_paging(unsigned int fault_addr);