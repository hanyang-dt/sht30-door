#include "stm32f10x.h"
#include "myiic.h"
#include "stdlib.h"
#include "string.h"
#include "bc26.h"
#include "exti.h"
#include "wdg.h"
#include "led.h"
#include "delay.h"
#include "usart.h"	
#include "sht30.h"

void Mcu_Init(void);
void send_data(void);
void receive_data(void);
void inttohex(int temp,u16 humi);
//char *Hex2Str ( uint8_t *pAddr, uint8_t len );//将数据变成字符串
//int StrToHex(char *Hex,char *Str,int len);//将数据从字符串变成16进制
extern char RxCounter,RxBuffer[100];
u8 buffer[5];
char receive_buffer[30];
int temp;
u16 humi;


int main()
{
		Mcu_Init();
		while(1)
		{
		}		

}
void Mcu_Init(void)
{
		delay_init();
		LED_Init();
	  
	  
		BC26CTR_Init();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
	  uart_init(115200);
		uart2_init(115200);
	
	  Door_Init();
	  EXTI0_Init();
		OPEN_BC26();
		BC26_Init();
		BC26_RegIOT();	  
}

