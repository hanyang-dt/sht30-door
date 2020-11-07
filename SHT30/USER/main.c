#include "stm32f10x.h"
#include "myiic.h"
#include "stdlib.h"
#include "string.h"
#include "bc26.h"
#include "wdg.h"
#include "led.h"
#include "delay.h"
#include "usart.h"	
#include "sht30.h"
void Mcu_Init(void);
void send_data(void);
void receive_data(void);
void inttohex(int temp,u16 humi);
char *Hex2Str ( uint8_t *pAddr, uint8_t len );//�����ݱ���ַ���
int StrToHex(char *Hex,char *Str,int len);//�����ݴ��ַ������16����
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
				send_data();
		}		

}
void Mcu_Init(void)
{
		delay_init();
		LED_Init();
	  //Relay_Init();
		BC26CTR_Init();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		uart_init(115200);
		uart2_init(115200);
		SHT30_Init();	
		OPEN_BC26();
		BC26_Init();
		BC26_RegIOT();

	  
}
//�������ݲ����Ƽ̵���
//void receive_data(void)
//{
//	u8 i;
//	while(RxBuffer[0] == 0 && RxBuffer[1] == 0)
//	{
//			if(RxBuffer[0] != 0 && RxBuffer[1] != 0)
//				break;
//	}
//	delay_ms(300);
//	delay_ms(300);
//	delay_ms(300);
//	delay_ms(300);
//	//Uart1_SendStr(RxBuffer);
//	memcpy(receive_buffer,RxBuffer+26,sizeof(RxBuffer));
//	for(i=0;i<30;i++)
//	{
//			if(receive_buffer[i] == '2')
//				{
//						Relay = 0;
//					 Uart1_SendStr("�̵�����\r\n");
//				}
//			if(receive_buffer[i] == '1')
//				{
//						Relay = 1;
//						Uart1_SendStr("�̵����ر�\r\n");
//			  }		
//	}
//	delay_ms(300);
//	Clear_Buffer();
//	for(i=0;i<30;i++)
//	{
//	   receive_buffer[i]=0;
//	}
//}
//��������
void send_data(void)
{
		char *str;
		//�õ���ַ��
		buffer[0] = 0x31;
		buffer[1] = 0x01;
	  //��ʪ������
		SHT30_Read_Humiture(&temp,&humi);
		buffer[2] = temp;
		buffer[3] = humi;
//    //�̵�������
//	  if(Relay == 0)
//         buffer[4] = 0x01; //��
//    else
//         buffer[4] = 0x00;	//�ر�		
		//���ݴ���
		str = Hex2Str(buffer,4);
//	printf("str0:%c%c,str1:%c%c\r\n",str[0],str[1],str[2],str[3]);
//  printf("temp:%c%c,humi:%c%c\r\n",str[4],str[5],str[6],str[7]);
//	
//		printf("relay:%c%c\r\n",str[8],str[9]);
		//��������	
//		for(i=0;i<10;i++)
//		{
//		    printf("%s",str[i]);
//		}
		BC26_IOTSenddata(str,4);
		delay_ms(1000);
}



//��16����hex����ת�����ַ���������ӡ.
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
//���ַ�����������
int StrToHex(char *Hex,char *Str,int len)
{
   int i = 0;
   int j = 0;
   int temp = 0;
   if(len == 0)
   {
       return 1;
   }
   while(i<len)
   {
       if(Str[i]>='0' && Str[i]<='F')
        {
            if((Str[i]>='0' && Str[i]<='9'))
            {
                temp = (Str[i] & 0x0f)<<4;
            }
            else if(Str[i]>='A' && Str[i]<='F')
            {
                temp = ((Str[i] + 0x09) & 0x0f)<<4;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
        i++;
        if(Str[i]>='0' && Str[i]<='F')
        {
            if(Str[i]>='0' && Str[i]<='9')
            {
                temp |= (Str[i] & 0x0f);
            }
            else if(Str[i]>='A' && Str[i]<='F')
            {
                temp |= ((Str[i] + 0x09) & 0x0f);
            }
            else
            {
                return 1; 
            }
        }
        else
        {
            return 1;
        }
        i++;
        Hex[j] = temp;
        j++;
    }
    return 0 ;
}
