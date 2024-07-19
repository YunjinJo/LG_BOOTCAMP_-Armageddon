#pragma once
#include "type_define.h"

#define DEMAND_PAGE_START_ADDR (0x44B00000)
#define MAX_PAGE (16)
#define ONE_KB (0x1000)

typedef struct{
    unsigned int asid;
    unsigned int VA;
} PAGE_INFO_T;

extern void demand_paging(unsigned int fault_addr);