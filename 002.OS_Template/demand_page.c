#include "demand_page.h"
#include "option.h"
#include "device_driver.h"

#define bit_clear(VAL, CNT) (VAL >> CNT << CNT)
#define PAGE_BIT_OPTION 0x2 |(WT_WBWA_PAGE)

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
    if (asid == 1) {
        return (unsigned int *) virtual_addr;
    }
    else if (asid == 2) {
        return (unsigned int *) (virtual_addr + 0x400000);
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

void demand_paging(unsigned int fault_addr)
{
    unsigned int asid = 0;
    unsigned int *first_TT_addr = NULL;
    unsigned int *second_TT_addr = NULL;
    unsigned int PA = 0;
    unsigned int *original_code_addr = NULL;

	CoInvalidateMainTlb();
	call_isb();

    asid = Get_ASID(); // 현재 ASID

    if (swap_flag) {
        page_cnt %= MAX_PAGE;
        // 기존 2nd TT 에 접근 불가로 설정
        second_TT_addr = page_info[page_cnt].second_TT_addr;
        *second_TT_addr = PAGE_BIT_OPTION;
    }

    // 현재 정보 저장    
    page_info[page_cnt].asid = asid;
    page_info[page_cnt].VA = fault_addr;

    // 1차 T/T 주소
    first_TT_addr = get_first_TT_addr(fault_addr, MMU_PAGE_TABLE_BASE);

    // 2차 T/T 주소
    second_TT_addr = get_second_TT_addr(fault_addr, *first_TT_addr);
    page_info[page_cnt].second_TT_addr = second_TT_addr;

    PA = (DEMAND_PAGE_START_ADDR + (page_cnt * FOUR_KB));
    original_code_addr = get_original_code_addr(fault_addr, asid);
    *second_TT_addr = (bit_clear(PA, 12) | PAGE_BIT_OPTION);

    memcpy((unsigned int *)PA, original_code_addr, FOUR_KB);

    page_cnt++;

    if(page_cnt >= MAX_PAGE) {
        swap_flag = 1;
    }
    CoInvalidateMainTlb();
}   