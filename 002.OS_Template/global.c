/*
 * global.c
 *
 *  Created on: 2024. 7. 17.
 *      Author: yunjin.jo
 */
#include "device_driver.h"
#include "global.h"

REG_INFO *sel_reg_info;

REG_INFO *reg_info_app0;
REG_INFO *reg_info_app1;

REG_INFO r_info_0;
REG_INFO r_info_1;


void pcb_init(unsigned int RAM, unsigned int ST_BASE_APP0, unsigned int ST_BASE_APP1) {
	reg_info_app0 = &r_info_0;
	reg_info_app1 = &r_info_1;

	reg_info_app0->CPSR = 0x1f;
	reg_info_app0->registers[13] = ST_BASE_APP0;
	reg_info_app0->PC = RAM; 

	reg_info_app1->CPSR = 0x1f; 
	reg_info_app1->registers[13] = ST_BASE_APP1;
	reg_info_app1->PC = RAM;
}
