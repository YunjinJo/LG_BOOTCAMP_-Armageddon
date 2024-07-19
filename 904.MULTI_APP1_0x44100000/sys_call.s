	.code	32
	.text

	.global Uart_Printf
Uart_Printf:
	svc 2
	bx lr
