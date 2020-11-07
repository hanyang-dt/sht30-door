#include "exti.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "bc26.h"
u8 buf[4];
//外部中断0服务程序
void EXTI0_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    Door_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}

//外部中断0服务程序 
void EXTI4_IRQHandler(void)
{
	delay_ms(10);//消抖

	 send();	
	EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE0上的中断标志位  
}
void send(void)
{
		char *str;
		//得到地址域
		buf[0] = 0x32;
		buf[1] = 0x01;
    //门开关数据
	  if(Door == 1)
         buf[2] = 0x00; //关闭
    else
         buf[2] = 0x01;	//打开
		//数据处理
		str = Hex2Str(buf,3);
		BC26_IOTSenddata(str,3);
		delay_ms(1000);
}
char *Hex2Str ( uint8_t *pAddr, uint8_t len )
{
    uint8_t     charCnt;
    char        hex[] = "0123456789ABCDEF";
    static char str[50];
    char        *pStr = str;

    for ( charCnt = len; charCnt > 0; charCnt-- ) {
        *pStr++ = hex[*pAddr >> 4];
        *pStr++ = hex[*pAddr++ & 0x0F];
    }
    pStr = NULL;

    return str;
}
