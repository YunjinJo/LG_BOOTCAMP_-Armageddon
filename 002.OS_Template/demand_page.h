#pragma once
#include "type_define.h"

#define DEMAND_PAGE_START_ADDR (0x44B00000)
#define MAX_PAGE (16)
#define FOUR_KB (0x1000)
#define ONE_KB (0x400)

#define PABT 1
#define DABT 2

typedef struct{
    unsigned int reason;
    unsigned int asid;
    unsigned int VA;
    unsigned int *backup_addr;
    unsigned int *second_TT_addr;
} PAGE_INFO_T;

extern void demand_paging(unsigned int fault_addr, unsigned int reason);
extern void demand_page_PABT(unsigned int fault_addr);
extern void demand_page_DABT(unsigned int fault_addr);
unsigned int *get_first_TT_addr(unsigned int virtual_addr, unsigned int base);
unsigned int *get_second_TT_addr(unsigned int virtual_addr, unsigned int base);