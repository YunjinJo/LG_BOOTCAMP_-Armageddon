MEMORY
{
	RAM  (rwx) : ORIGIN = 0x44100000, LENGTH = 56M
}

REGION_ALIAS("REGION_TEXT", RAM);
REGION_ALIAS("REGION_RODATA", RAM);
REGION_ALIAS("REGION_DATA", RAM);
REGION_ALIAS("REGION_BSS", RAM);

SECTIONS
{
	.text :
	{
		crt0.o(.text)
		*(.text)
    		. = ALIGN(4); 
	} > REGION_TEXT
	
	.rodata :
	{
		__RO_BASE__ = .;	
		*(.rodata)
   		*(.rodata.*)			
   		. = ALIGN(4); 
   		__RO_LIMIT__ = .;
	} > REGION_RODATA

	.data :
	{
		__RW_BASE__ = .;
		*(.data)
   		*(.data.*)		
		. = ALIGN(4); 		
		__RW_LIMIT__ = .;			
	} > REGION_DATA
	
	.bss :
	{
		__ZI_BASE__ = .;
		*(.bss)
   		*(.bss.*)		
		. = ALIGN(4); 		
		__ZI_LIMIT__ = .;
	} > REGION_BSS
}
