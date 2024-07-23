#include "demand_page.h"
#include "option.h"
#include "device_driver.h"

#define bit_clear(VAL, CNT) (VAL >> CNT << CNT)

#define PAGE_TYPE_SMALL      0x2 // 비트 1:0 
#define PAGE_BUFFERABLE      (0x0 << 2) // 비트 2 (B)
#define PAGE_CACHEABLE       (0x1 << 3) // 비트 3 (C)
#define PAGE_AP_FULL_ACCESS  (0x3 << 4) // 비트 4, 5, 9 (AP[2:0] = 011)
#define PAGE_TEX_WRITE_BACK  (0x5 << 6) // 비트 6, 7, 8 (TEX = 101)
#define PAGE_SHAREABLE       (0x0)      // 비트 10 (S)
#define PAGE_GLOBAL          (0x1 << 11) // 비트 11 (nG = 0)

#define PAGE_BIT_OPTION (PAGE_TYPE_SMALL | PAGE_BUFFERABLE | PAGE_CACHEABLE | PAGE_AP_FULL_ACCESS | PAGE_TEX_WRITE_BACK | PAGE_GLOBAL)

PAGE_INFO_T page_info[MAX_PAGE] = {0, };
unsigned int page_cnt = 0;
unsigned int swap_flag = 0;

void set_second_descriptor(unsigned int *second_TT_addr, unsigned int PA, unsigned int option)
{
    unsigned int new_PA = bit_clear(PA, 12);
    *second_TT_addr = new_PA | option;
}

unsigned int *get_original_code_addr(unsigned int virtual_addr, unsigned int asid)
{
    virtual_addr = virtual_addr & ~0xFFF;
    if (asid == 1) {
        return (unsigned int *) (virtual_addr - 0x30000000 + 0x44100000);
    }
    else if (asid == 2) {
        return (unsigned int *) (virtual_addr - 0x30000000 + 0x44500000);
    }
    else {
        Uart_Printf("ERROR\n");
        return (unsigned int *) virtual_addr;
    }
}

// Physical Address
unsigned int *get_PA(unsigned int virtual_addr, unsigned int second_descriptor)
{
    unsigned int small_page_base = bit_clear(second_descriptor, 12);
    unsigned int page_index = virtual_addr & (0xfff); // 하위 12bit 만 살림
    return (unsigned int *) small_page_base + page_index;
}

// 2차 TT Entry 주소
unsigned int *get_second_TT_addr(unsigned int virtual_addr, unsigned int base)
{
    unsigned int ret = 0;
    unsigned int new_base = bit_clear(base, 10);
    unsigned int table_index = virtual_addr >> 12;
    table_index &= (0xff); // 하위 8bit 제외 전부 clear
    table_index <<= 2; // 마지막에 00 bit 붙임
    ret = new_base + table_index;
    return (unsigned int *) ret;
}

// 1차 TT Entry 주소
unsigned int *get_first_TT_addr(unsigned int virtual_addr, unsigned int base)
{
    unsigned int ret = 0;
    unsigned int table_index = virtual_addr >> 20; // 20 ~ 31 bit
    table_index <<= 2; // 마지막에 00 bit 붙임
    ret = base + table_index;

    return (unsigned int *) ret;
}

void demand_paging(unsigned int fault_addr, unsigned int reason)
{
    unsigned int asid = 0;
    unsigned int *first_TT_addr = NULL;
    unsigned int *second_TT_addr = NULL;
    unsigned int free_mem_PA = 0;
    unsigned int *original_code_addr = NULL;

    asid = Get_ASID(); // 현재 ASID

    if (swap_flag) {
        page_cnt %= MAX_PAGE;
        // 기존 2nd TT 에 접근 불가로 설정
        if (page_info[page_cnt].asid == 1) {
            first_TT_addr = get_first_TT_addr(page_info[page_cnt].VA, MMU_PAGE_TABLE_BASE);
        }
        else if (page_info[page_cnt].asid == 2){
            first_TT_addr = get_first_TT_addr(page_info[page_cnt].VA, MMU_PAGE_TABLE_BASE_APP1);
        }

        second_TT_addr = get_second_TT_addr(page_info[page_cnt].VA, *first_TT_addr);
        *second_TT_addr = 0;

        // page_info reason이 D ABT라면 backup
        if (page_info[page_cnt].reason == DABT) {
            free_mem_PA = (DEMAND_PAGE_START_ADDR + (page_cnt * FOUR_KB));
            memcpy((void *)page_info[page_cnt].backup_addr, (void *) free_mem_PA, FOUR_KB);
        }
    }

    // 1차 T/T 주소
    if (asid == 1) {
        first_TT_addr = get_first_TT_addr(fault_addr, MMU_PAGE_TABLE_BASE);
    }
    else if (asid == 2) {
        first_TT_addr = get_first_TT_addr(fault_addr, MMU_PAGE_TABLE_BASE_APP1);
    }
    
    // 2차 T/T 주소
    second_TT_addr = get_second_TT_addr(fault_addr, *first_TT_addr);

    free_mem_PA = (DEMAND_PAGE_START_ADDR + (page_cnt * FOUR_KB));
    original_code_addr = get_original_code_addr(fault_addr, asid);
    *second_TT_addr = (bit_clear(free_mem_PA, 12) | PAGE_BIT_OPTION);

    // 현재 정보 저장    
    page_info[page_cnt].reason = reason;
    page_info[page_cnt].asid = asid;
    page_info[page_cnt].VA = fault_addr;
    page_info[page_cnt].second_TT_addr = second_TT_addr;
    page_info[page_cnt].backup_addr = original_code_addr;

    memcpy((void *)free_mem_PA, original_code_addr, FOUR_KB);

    page_cnt++;

    if(page_cnt >= MAX_PAGE) {
        swap_flag = 1;
    }
    CoInvalidateMainTlb();
    CoInvalidateICache();
}   


void demand_page_PABT(unsigned int fault_addr)
{
    // unsigned int *temp = get_first_TT_addr(fault_addr, MMU_PAGE_TABLE_BASE);
    // unsigned int *second = get_second_TT_addr(fault_addr, *temp);
    CoInvalidateMainTlb();
	call_isb();
    // Uart_Printf("\nDEMAND PAGE PABT : %X\n", fault_addr);
    // Uart_Printf("\nDEMAND PAGE *PABT : %X\n", *temp);
    // Uart_Printf("\nSECOND!!! PABT : %X\n", (unsigned int) second);
    // Uart_Printf("\nSECOND!!! *PABT : %X\n", *second);
    demand_paging(fault_addr, PABT);
}

void demand_page_DABT(unsigned int fault_addr)
{
    // unsigned int *temp = get_first_TT_addr(fault_addr, MMU_PAGE_TABLE_BASE);
    // unsigned int *second = get_second_TT_addr(fault_addr, *temp);
    CoInvalidateMainTlb();
	call_isb();
    // Uart_Printf("\nDEMAND PAGE DABT : %X\n", fault_addr);
    // Uart_Printf("\nDEMAND PAGE *DABT : %X\n", *temp);
    // Uart_Printf("\nSECOND!!! DABT : %X\n", (unsigned int) second);
    // Uart_Printf("\nSECOND!!! *DABT : %X\n", *second);
    demand_paging(fault_addr, DABT);
}