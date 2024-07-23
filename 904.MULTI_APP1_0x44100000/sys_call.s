	.code	32
	.text

	.global Uart_Printf
Uart_Printf:
	svc 2
	bx lr

	.global Lcd_Printf
Lcd_Printf:
	svc 10
	bx lr

	.global Uart1_GetIntNum
Uart1_GetIntNum:
	svc 11
	bx lr
	
	.global Uart1_GetString
Uart1_GetString:
	svc 12
	bx lr

	.global Lcd_Clr_Screen
Lcd_Clr_Screen:
	svc 13
	bx lr

	.global Lcd_Clr_Screen_left_half
Lcd_Clr_Screen_left_half:
	svc 18
	bx lr
	
	.global Get_Input_Flag
Get_Input_Flag:
	svc 14
	bx lr

	.global Toggle_Input_Flag
Toggle_Input_Flag:
	svc 15
	bx lr

	.global Lcd_Draw_Back_Color
Lcd_Draw_Back_Color:
	svc 8
	bx lr

	.global Uart1_Get_Char
Uart1_Get_Char:
	svc 17
	bx lr