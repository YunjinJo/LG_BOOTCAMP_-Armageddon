#define	Macro_Set_Bit(dest, position)					((dest) |= ((unsigned)0x1<<(position)))
#define	Macro_Clear_Bit(dest, position)					((dest) &= ~((unsigned)0x1<<(position)))
#define	Macro_Invert_Bit(dest, position)				((dest) ^=  ((unsigned)0x1<<(position)))

#define	Macro_Clear_Area(dest, bits, position)			((dest) &= ~(((unsigned)bits)<<(position)))
#define	Macro_Set_Area(dest, bits, position)			((dest) |=  (((unsigned)bits)<<(position)))
#define	Macro_Invert_Area(dest, bits, position)			((dest) ^=  (((unsigned)bits)<<(position)))

#define	Macro_Write_Block(dest, bits, data, position)	((dest) = (((unsigned)dest) & ~(((unsigned)bits)<<(position))) | (((unsigned)data)<<(position)))

#define Macro_Check_Bit_Set(dest, position)				((((unsigned)dest)>>(position)) & 0x1)
#define Macro_Check_Bit_Clear(dest, position)			(!((((unsigned)dest)>>(position)) & 0x1))

#define Macro_Extract_Area(dest, bits, position)		((((unsigned)dest)>>(position)) & (bits))
