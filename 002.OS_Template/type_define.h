/*
 * type_define.h
 *
 *  Created on: 2024. 7. 17.
 *      Author: yunjin.jo
 */

#ifndef TYPE_DEFINE_H_
#define TYPE_DEFINE_H_

#define NULL ((void *)0)

typedef struct {
	unsigned int PC;
	unsigned int CPSR;
	unsigned int registers[15]; // r0-r14
}REG_INFO;

typedef struct{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
	unsigned int e;
	unsigned int f;
}TEST_T;



#endif /* TYPE_DEFINE_H_ */
