	.global SVC_Print_Hello
SVC_Print_Hello:
	svc		0
	bx		lr

	.global SVC_Lcd_Clr_Screen
SVC_Lcd_Clr_Screen:
	svc		1
	bx		lr

	.global SVC_Lcd_Draw_BMP
SVC_Lcd_Draw_BMP:
	svc		2
	bx		lr

	.global SVC_Uart_Printf
SVC_Uart_Printf:
	svc		3
	bx		lr

