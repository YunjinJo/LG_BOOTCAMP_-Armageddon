#include "device_driver.h"

volatile unsigned long *ICCICR[4] = {&rICCICR_CPU0,&rICCICR_CPU1,&rICCICR_CPU2,&rICCICR_CPU3};

volatile unsigned long *ICDISER0[4] = {&rICDISER0_CPU0,&rICDISER0_CPU1,&rICDISER0_CPU2,&rICDISER0_CPU3};
volatile unsigned long *ICDISERn[4] = {(volatile unsigned long *)0,&rICDISER1,&rICDISER2,&rICDISER3};

volatile unsigned long *ICDICER0[4] = {&rICDICER0_CPU0,&rICDICER0_CPU1,&rICDICER0_CPU2,&rICDICER0_CPU3};
volatile unsigned long *ICDICERn[4] = {(volatile unsigned long *)0,&rICDICER1,&rICDICER2,&rICDICER3};

volatile unsigned long *ICDIPTR0[4] = {&rICDIPTR0_CPU0,&rICDIPTR0_CPU1,&rICDIPTR0_CPU2,&rICDIPTR0_CPU3};

volatile unsigned long *ICDICPR0[4] = {&rICDICPR0_CPU0,&rICDICPR0_CPU1,&rICDICPR0_CPU2,&rICDICPR0_CPU3};

volatile unsigned long *ICDIPR0[4] = {&rICDIPR0_CPU0,&rICDIPR0_CPU1,&rICDIPR0_CPU2,&rICDIPR0_CPU3};
volatile unsigned long *ICCPMR[4] = {&rICCPMR_CPU0,&rICCPMR_CPU1,&rICCPMR_CPU2,&rICCPMR_CPU3};

volatile unsigned long *ICCEOIR[4] = {&rICCEOIR_CPU0,&rICCEOIR_CPU1,&rICCEOIR_CPU2,&rICCEOIR_CPU3};
volatile unsigned long *ICCIAR[4] = {&rICCIAR_CPU0,&rICCIAR_CPU1,&rICCIAR_CPU2,&rICCIAR_CPU3};

void GIC_Distributor_Enable(int en)
{
	rICDDCR = (en)? 1 : 0;
}

void GIC_CPU_Interface_Enable(int cpuid, int value)
{
	*ICCICR[cpuid] = value;
}

void GIC_Set_Priority_Mask(int cpuid, int prio)
{
	*ICCPMR[cpuid] = prio;
}

void GIC_Interrupt_Enable(int cpuid,int intid)
{
	if(intid<32)
	{
		*ICDISER0[cpuid] = (1<<intid);
	}
	else
	{
		*ICDISERn[intid>>5] = 1<<(intid%32);
	}
}

void GIC_Interrupt_Disable(int cpuid,int intid)
{
	if(intid<32)
	{
		*ICDICER0[cpuid] = (1<<intid);
	}
	else
	{
		*ICDICERn[intid>>5] = 1<<(intid%32);
	}
}

void GIC_Set_Interrupt_Priority(int cpuid, int intid, int prio)
{
	if(intid<32)
	{
		Macro_Write_Block(*(ICDIPR0[cpuid]+(intid>>2)),0xff,prio,((intid%4)*8));
	}
	else
	{
		Macro_Write_Block(*(ICDIPR0[0]+(intid>>2)),0xff,prio,((intid%4)*8));
	}
}

void GIC_Set_Processor_Target(int cpuid, int intid, int cpubit)
{
	if(intid<32)
	{
		Macro_Write_Block(*(ICDIPTR0[cpuid]+(intid>>2)),0xff,cpubit & 0xf,((intid%4)*8));
	}
	else
	{
		Macro_Write_Block(*(ICDIPTR0[0]+(intid>>2)),0xff,cpubit & 0xf,((intid%4)*8));
	}
}

inline void GIC_Clear_Pending_Clear(int cpuid, int intid)
{
	if(intid<32)
	{
		*ICDICPR0[cpuid] = 1<<(intid);
	}
	else
	{
		*(ICDICPR0[0]+(intid>>5)) = 1<<(intid%32);
	}
}

unsigned int GIC_Read_INTACK(int cpuid)
{
	return *ICCIAR[cpuid] & 0x1fff;
}

void GIC_Write_EOI(int cpuid, int cpu_int_id)
{
	*ICCEOIR[cpuid] = cpu_int_id;
}

void GIC_Generate_SGI(int taregtfilter, int cpubit, int intid)
{
	rICDSGIR = (taregtfilter<<24)|(cpubit<<16)|(0<<15)|(intid<<0);
}

