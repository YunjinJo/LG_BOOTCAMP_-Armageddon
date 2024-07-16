#include "device_driver.h"

caddr_t _sbrk(int inc)
{
	return 0;
}

void Delay(unsigned int v)
{
	volatile int i, j;

	for(i = 0; i < v; i++)
	{
		for(j = 0; j < v; j++);
	}
}

