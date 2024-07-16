#include "device_driver.h"

#define __STACK_LIMIT__		(STACK_BASE - STACK_SIZE)
#define __HEAP_LIMIT__ 		(__STACK_LIMIT__ - 8)
#define __HEAP_BASE__ 		((caddr_t)(((unsigned int)&__ZI_LIMIT__ + 0x7) & ~0x7))

extern unsigned char __ZI_LIMIT__;
static caddr_t heap =  NULL;

caddr_t _sbrk(int inc)
{
	caddr_t prevHeap;
	caddr_t nextHeap;

	if(heap == NULL) heap = __HEAP_BASE__;

	prevHeap = heap;
	nextHeap = (caddr_t)((((unsigned int)heap + inc) + 0x7) & ~0x7);

	if((unsigned int)nextHeap >= __HEAP_LIMIT__) return (caddr_t)NULL;

	heap = nextHeap;
	return prevHeap;
}

unsigned int Get_Stack_Base(void)
{
	return (unsigned int)STACK_BASE;
}

unsigned int Get_Stack_Limit(void)
{
	return (unsigned int)__STACK_LIMIT__;
}

unsigned int Get_Heap_Base(void)
{
	return (unsigned int)__HEAP_BASE__;
}

unsigned int Get_Heap_Limit(void)
{
	return (unsigned int)__HEAP_LIMIT__;
}

void Delay(unsigned int v)
{
	volatile int i;

	for(i = 0; i < v; i++);
}

