/*
 * global.c
 *
 *  Created on: 2024. 7. 17.
 *      Author: yunjin.jo
 */
#include "device_driver.h"
#include "global.h"

REG_INFO *reg_info_app0;
REG_INFO *reg_info_app1;

REG_INFO r_info_0;
REG_INFO r_info_1;

/* �� app�� PCB �ʱ�ȭ  */
void pcb_init(unsigned int RAM_APP0, unsigned int STACK_BASE_APP0, unsigned int STACK_BASE_APP1) {
	reg_info_app0 = &r_info_0;
	reg_info_app1 = &r_info_1;

	// app0�� PCB �ʱ�ȭ
	reg_info_app0->CPSR = Get_CPSR(); // �̹� T bit�� 0
	reg_info_app0->CPSR |= 0x1F; //sys mode�� ���� ��ȯ
	reg_info_app0->registers[13] = STACK_BASE_APP0;
	reg_info_app0->PC = RAM_APP0; //VA ����

	// app1�� PCB �ʱ�ȭ
	reg_info_app1->CPSR = Get_CPSR(); // �̹� T bit�� 0
	reg_info_app1->CPSR |= 0x1F; //sys mode�� ���� ��ȯ
	reg_info_app1->registers[13] = STACK_BASE_APP1;
	reg_info_app1->PC = RAM_APP0; //VA ����
}
