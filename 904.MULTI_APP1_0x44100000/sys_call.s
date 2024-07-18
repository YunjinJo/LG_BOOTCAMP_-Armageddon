	.code	32
	.text

	.global call_svc_0
call_svc_0:
	svc 0
	bx lr

	.global Uart_Printf
Uart_Printf:
	svc 4
	bx lr