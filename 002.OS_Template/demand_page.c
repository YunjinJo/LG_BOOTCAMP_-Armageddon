#include "demand_page.h"
#include "option.h"
#include "device_driver.h"

PAGE_INFO_T page_info[MAX_PAGE] = {0, };
unsigned int page_cnt = 0;

// 1차 TT Entry 주소 가져옴
unsigned int get_first_TT_entry_addr(unsigned int virtual_addr, unsigned int base)
{
    unsigned int ret = 0;
    unsigned int table_index = virtual_addr >> 20; // 20 ~ 31 bit
    //table_index <<= 2; // 마지막에 00 bit 붙임
    ret = base + table_index;

    return ret;
}

void demand_paging(unsigned int fault_addr)
{
    unsigned int asid = 0;
    unsigned int first_TT_base = 0;
    unsigned int *first_TT_entry_addr = NULL;
    unsigned int *second_TT_entry_addr = NULL;
    unsigned int PA = 0;
    unsigned int second_TT_base = 0;
    unsigned int secnod_page_table_index = 0;

	CoInvalidateMainTlb();
	call_isb();

    Uart_Printf("\nDEMANDING PAGE!!!\n");

    asid = Get_ASID();
    
    switch (asid)
    {
    case 1:
        first_TT_base = MMU_PAGE_TABLE_BASE;
        second_TT_base = SND_PAGE_TABLE_BASE_APP0;
        break;

    case 2:
        first_TT_base = MMU_PAGE_TABLE_BASE_APP1;
        second_TT_base = SND_PAGE_TABLE_BASE_APP1;
        break;
    
    default:
        break;
    }

    page_info[page_cnt].asid = asid;
    page_info[page_cnt].VA = fault_addr;

    first_TT_entry_addr = (unsigned int *) get_first_TT_entry_addr(fault_addr, first_TT_base);
    if (asid == 1) {
        *first_TT_entry_addr = fault_addr;
        *first_TT_entry_addr &= ~(0xfffff);// 0 ~ 19 bit clear
    }
    else if (asid == 2) {
        *first_TT_entry_addr = fault_addr + 0x400000; // APP 1 시작 주소 보정
        *first_TT_entry_addr &= ~(0xfffff);// 0 ~ 19 bit clear
    }
    // bit setting
    *first_TT_entry_addr |= (1 << 2) + (1 << 10) + (1 << 11);
    *first_TT_entry_addr &= ~(1 << 15);
    *first_TT_entry_addr |= WT_WBWA; // 캐시 설정

    PA = (DEMAND_PAGE_START_ADDR + (page_cnt * ONE_KB));
    PA &= ~(0xfff); // 0 ~ 12 bit clear


    memcpy((void *)PA, (void *)(fault_addr >> 12 << 12), ONE_KB);

    second_TT_base += ((fault_addr >> 20) - 0x441) * 0x400; // offset 수정
    *first_TT_entry_addr = second_TT_base;
    *first_TT_entry_addr |= 0x1;

    secnod_page_table_index = (fault_addr >> 12) % 0x100;
	second_TT_entry_addr = (unsigned int *) second_TT_base + secnod_page_table_index;
    *second_TT_entry_addr = PA;
    *second_TT_entry_addr &= (~(1 << 9));
    *second_TT_entry_addr |= (1 << 4) + (1 << 5) + (1 << 2);
    *second_TT_entry_addr |= WT_WBWA_PAGE;
    CoInvalidateMainTlb();
    call_isb();
	page_cnt++;
    Uart_Printf("\nDEMANDING PAGE END!!!\n");
}