#ifndef PCB_LIST_H_
#define PCB_LIST_H_

extern unsigned int pcb_list[2][17];
extern unsigned int current_pcb[17];

void pcb_list_init(unsigned int RAM_APP0, unsigned int STACK_BASE_APP0, unsigned int STACK_BASE_APP1);

#endif // __PCB_LIST_H__
