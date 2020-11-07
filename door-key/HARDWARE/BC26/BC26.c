#include "bc26.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern char  RxBuffer[100],RxCounter;
BC26 BC26_Status;
unsigned char socketnum=0;//��ǰ��socket����
void Clear_Buffer(void)//��ջ���
{
		u8 i;
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//����
		RxCounter=0;
		//IWDG_Feed();//ι��
	
}
void WakeUp_BC26(void)
{
		char *strx;
		u8 num=3;
		Clear_Buffer();
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		while(strx==NULL&&num)
		{
        PWRKEY=1;//����
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);	
        PWRKEY=0;//������������
				Clear_Buffer();
				num--;
				printf("AT\r\n"); 
				delay_ms(300);
				strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		}
		if(num == 0)
		{
				Uart1_SendStr("����ʧ�ܣ� \r\n");	
		}
		else
		{
				Uart1_SendStr("���ѳɹ��� \r\n");	
		}
}
void OPEN_BC26(void)
{
		char *strx;
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		//IWDG_Feed();//ι��
		if(strx==NULL)
		{
        PWRKEY=1;//����
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);	
        PWRKEY=0;//������������
        //IWDG_Feed();//ι��
		}
    printf("AT\r\n"); 
    delay_ms(300);
    //IWDG_Feed();//ι��
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    if(strx==NULL)//����豸�����ˣ��͸�λģ��
    {
        RESET=1;//����
        delay_ms(300);
        delay_ms(300);	
        RESET=0;//��λģ��
    }
    printf("ATE0&W\r\n"); //�رջ���
    delay_ms(300); 
    LED=0;
    //IWDG_Feed();//ι��
}
void BC26_Init(void)
{
    u8 num=3;
		Clear_Buffer();	
		printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    while(strx==NULL&&num)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
				num--;
    }
		if(num == 0)
		{
				Uart1_SendStr("ͨ��ʧ�ܣ� \r\n");	
		}
		else
		{
				Uart1_SendStr("ͨ�������� \r\n");	
		}	
    printf("ATE0&W\r\n"); //�رջ���
    delay_ms(300);
    printf("AT+CFUN=1\r\n"); //����Ƶ����
    delay_ms(300);
		Uart1_SendStr("���ڼ�⿨״̬...��\r\n");
		Clear_Buffer();	
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
    num = 3 ;
    while(strx==NULL&&num)
    {
        Uart1_SendStr("����Ϣ�쳣�����鿨״̬�������Զ�������\r\n");
				printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
				num--;
    }
		if(num == 0)
		{
				Uart1_SendStr("����Ϣ��ȡʧ�ܣ�\r\n");
		}
		else
		{
				Uart1_SendStr("����Ϣ��ȡ�ɹ���\r\n");
		}
		Clear_Buffer();	
		printf("AT+CGPADDR=1\r\n");//��ѯIP��ȷ��ģ��ע���ɹ�
		delay_ms(300);
		Clear_Buffer();	
		printf("AT+CGATT?\r\n");//��ѯ����״̬
		delay_ms(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//��1
		num = 3;
		while(strx==NULL&&num)
		{
				Clear_Buffer();	
				printf("AT+CGATT?\r\n");//��ȡ����״̬
				delay_ms(300);
				strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//����1,����ע���ɹ�
				num--;
		}
		if(num == 0)
		{
				Uart1_SendStr("ע��ʧ�ܣ�\r\n");
		}
		else
		{
				Uart1_SendStr("ע���ɹ���\r\n");
		}
		Clear_Buffer();	
		printf("AT+CESQ\r\n");//�鿴��ȡCSQֵ
		delay_ms(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CESQ");//����CSQ
		if(strx)
		{
				BC26_Status.CSQ=(strx[7]-0x30)*10+(strx[8]-0x30);//��ȡCSQ
				if((BC26_Status.CSQ==99)||((strx[7]-0x30)==0))//˵��ɨ��ʧ��
				{
						while(1)
						{
								BC26_Status.netstatus=0;
								Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
								RESET=1;//����
								delay_ms(300);
								delay_ms(300);	
								RESET=0;//��λģ��
								delay_ms(300);//û���źž͸�λ             
						}
				}
        else
        {
						BC26_Status.netstatus=1;
						Uart1_SendStr("�ź�ǿ��\r\n");
        }        
     }
}
void BC26_RegIOT(void)//ƽ̨ע��
{
    char  BC26_IMEI[16] = {0};//IMEIֵ
		u8 num = 3;
    printf("AT+QLWCLOSE\r\n");//�ر���һ������
		Uart1_SendStr("�ر��ϴ�����\r\n");
    delay_ms(300);
    printf("AT+QLWDEL\r\n");//ɾ�����
    delay_ms(300);
    printf("AT+QLWSERV=\"180.101.147.115\",5683\r\n");//��¼����ƽ̨
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
    }
    Clear_Buffer();	
    printf("AT+CGSN=1\r\n");//��ȡģ���IMEI��
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGSN:");//��+CGSN:
      while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+CGSN:");//��+CGSN:
    }
		memcpy(BC26_IMEI,strx+7,15);
		//Uart1_SendStr(BC26_IMEI);
    Clear_Buffer();	
    printf("AT+QLWCONF=%s\r\n",BC26_IMEI);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
    }
    Clear_Buffer();	   
    printf("AT+QLWADDOBJ=19,0,1,\"0\"\r\n");//ע����������
      delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
    }
    Clear_Buffer();
    printf("AT+QLWADDOBJ=19,1,1,\"0\"\r\n");//ע����������
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
    }
		Uart1_SendStr("ע��ɹ�\r\n");
    Clear_Buffer();
    printf("AT+QLWOPEN=0\r\n");//��������
    delay_ms(1000);
    strx=strstr((const char*)RxBuffer,(const char*)"CONNECT OK");//��CONNECT OK 
		while(strx==NULL&&num)
    {
				Clear_Buffer();
				printf("AT+QLWOPEN=0\r\n");//��������
				delay_ms(3000);
				strx=strstr((const char*)RxBuffer,(const char*)"CONNECT OK");//��CONNECT OK
				num--;
    }
		if(num == 0)
		{
				Uart1_SendStr("���ӵ�����ƽ̨ʧ�ܣ�\r\n");
		}
		else
		{
				Uart1_SendStr("���ӵ�����ƽ̨�ɹ���\r\n");
		}
    strx=strstr((const char*)RxBuffer,(const char*)"+QLWOBSERVE: 0,19,0,0");//��+QLWOBSERVE: 0,19,0,0 һ��Ҫ�жϣ��������ݻᱨ��
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QLWOBSERVE: 0,19,0,0");//��+QLWOBSERVE: 0,19,0,0
    }
    Clear_Buffer();
    printf("AT+QLWCFG=\"dataformat\",1,1\r\n");//�����������ݰ���ʮ������
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
    }
    Clear_Buffer();
}

void BC26_IOTSenddata(u8 *Data,u8 Datalen)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
{	
   printf("AT+QLWDATASEND=19,0,0,%d,%s,0x0100\r\n",Datalen,Data);//�Ϸ�����,
   delay_ms(300);
   strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//��SEND OK 
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//��SEND OK 
    }
    Uart1_SendStr(RxBuffer);
    RxCounter=0;
		Clear_Buffer();
    //IWDG_Feed();//ι��  
}
