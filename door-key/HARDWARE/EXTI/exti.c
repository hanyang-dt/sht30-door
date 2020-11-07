#include "exti.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "bc26.h"
u8 buf[4];
//�ⲿ�ж�0�������
void EXTI0_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    Door_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
}

//�ⲿ�ж�0������� 
void EXTI4_IRQHandler(void)
{
	delay_ms(10);//����

	 send();	
	EXTI_ClearITPendingBit(EXTI_Line4); //���LINE0�ϵ��жϱ�־λ  
}
void send(void)
{
		char *str;
		//�õ���ַ��
		buf[0] = 0x32;
		buf[1] = 0x01;
    //�ſ�������
	  if(Door == 1)
         buf[2] = 0x00; //�ر�
    else
         buf[2] = 0x01;	//��
		//���ݴ���
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
