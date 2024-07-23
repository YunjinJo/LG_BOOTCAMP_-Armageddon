	.global SVC_Print_Hello
SVC_Print_Hello:
	svc		1
	bx		lr

	.global SVC_Lcd_Clr_Screen
SVC_Lcd_Clr_Screen:
	svc		0
	bx		lr

	.global SVC_Lcd_Draw_BMP
SVC_Lcd_Draw_BMP:
	svc		1
	bx		lr

	.global SVC_Uart_Printf
SVC_Uart_Printf:
	svc		2
	bx		lr

	.global SVC_Lcd_Init
SVC_Lcd_Init:
	svc		3
	bx		lr

	.global SVC_Lcd_Win_Init
SVC_Lcd_Win_Init:
	svc 	4
	bx		lr

	.global SVC_Lcd_Brightness_Control
SVC_Lcd_Brightness_Control:
	svc 	5
	bx 		lr

	.global SVC_Lcd_Select_Display_Frame_Buffer
SVC_Lcd_Select_Display_Frame_Buffer:
	svc		6
	bx 		lr

	.global SVC_Lcd_Select_Draw_Frame_Buffer
SVC_Lcd_Select_Draw_Frame_Buffer:
	svc 	7
	bx 		lr

	.global SVC_Lcd_Draw_Back_Color
SVC_Lcd_Draw_Back_Color:
	svc		8
	bx		lr

	.global SVC_Lcd_Draw_Line
SVC_Lcd_Draw_Line:
	svc		9
	bx		lr

	.global Lcd_Printf
Lcd_Printf:
	svc 10
	bx lr

	.global Get_Input_Flag
Get_Input_Flag:
	svc 14
	bx lr

	.global Toggle_Input_Flag
Toggle_Input_Flag:
	svc 15
	bx lr

	.global SVC_Lcd_Draw_Bar
SVC_Lcd_Draw_Bar:
	svc		16
	bx		lr

	.global SVC_Uart1_Get_Char
SVC_Uart1_Get_Char:
	svc 	17
	bx		lr




