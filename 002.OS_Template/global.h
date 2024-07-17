/*
 * global.h
 *
 *  Created on: 2024. 7. 17.
 *      Author: yunjin.jo
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "type_define.h"

#define MAX_REG_INFO 3

extern REG_INFO *reg_info_list[MAX_REG_INFO];

extern REG_INFO *sel_reg_info;
extern REG_INFO *notsel_reg_info;

extern REG_INFO *reg_info_app0;
extern REG_INFO *reg_info_app1;

void pcb_init(unsigned int RAM_APP0, unsigned int STACK_BASE_APP0, unsigned int STACK_BASE_APP1);

#endif /* GLOBAL_H_ */
