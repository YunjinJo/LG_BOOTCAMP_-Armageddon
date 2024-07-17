/*
 * type_define.h
 *
 *  Created on: 2024. 7. 17.
 *      Author: yunjin.jo
 */

#ifndef TYPE_DEFINE_H_
#define TYPE_DEFINE_H_

typedef struct {
	unsigned int PC;
	unsigned int CPSR;
	unsigned int registers[15]; // r0-r14
}REG_INFO;

#endif /* TYPE_DEFINE_H_ */
