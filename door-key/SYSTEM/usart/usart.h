#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
	
void uart_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void UART1_send_byte(char data);
void UART2_send_byte(char data);
void Usart2_SendNByte(u8 *Data,u8 length);
void Uart1_SendStr(char*SendBuf);
#endif


