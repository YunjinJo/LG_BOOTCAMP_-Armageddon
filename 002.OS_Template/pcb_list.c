#include "pcb_list.h"

unsigned int pcb_list[2][17];
unsigned int current_pcb[17];

void pcb_list_init(unsigned int RAM_APP0, unsigned int STACK_BASE_APP0, unsigned int STACK_BASE_APP1)
{
	pcb_list[0][13] = STACK_BASE_APP0; // sp
	pcb_list[0][14] = RAM_APP0; // pc

	pcb_list[1][13] = STACK_BASE_APP1;
	pcb_list[1][14] = RAM_APP0;

}
