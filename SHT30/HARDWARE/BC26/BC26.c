#include "bc26.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern char  RxBuffer[100],RxCounter;
BC26 BC26_Status;
unsigned char socketnum=0;//当前的socket号码
void Clear_Buffer(void)//清空缓存
{
		u8 i;
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//缓存
		RxCounter=0;
		//IWDG_Feed();//喂狗
	
}
void WakeUp_BC26(void)
{
		char *strx;
		u8 num=3;
		Clear_Buffer();
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		while(strx==NULL&&num)
		{
        PWRKEY=1;//拉低
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);	
        PWRKEY=0;//拉高正常开机
				Clear_Buffer();
				num--;
				printf("AT\r\n"); 
				delay_ms(300);
				strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		}
		if(num == 0)
		{
				Uart1_SendStr("唤醒失败！ \r\n");	
		}
		else
		{
				Uart1_SendStr("唤醒成功！ \r\n");	
		}
}
void OPEN_BC26(void)
{
		char *strx;
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		//IWDG_Feed();//喂狗
		if(strx==NULL)
		{
        PWRKEY=1;//拉低
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);	
        PWRKEY=0;//拉高正常开机
        //IWDG_Feed();//喂狗
		}
    printf("AT\r\n"); 
    delay_ms(300);
    //IWDG_Feed();//喂狗
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    if(strx==NULL)//如果设备休眠了，就复位模块
    {
        RESET=1;//拉低
        delay_ms(300);
        delay_ms(300);	
        RESET=0;//复位模块
    }
    printf("ATE0&W\r\n"); //关闭回显
    delay_ms(300); 
    LED=0;
    //IWDG_Feed();//喂狗
}
void BC26_Init(void)
{
    u8 num=3;
		Clear_Buffer();	
		printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    while(strx==NULL&&num)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
				num--;
    }
		if(num == 0)
		{
				Uart1_SendStr("通信失败！ \r\n");	
		}
		else
		{
				Uart1_SendStr("通信正常！ \r\n");	
		}	
    printf("ATE0&W\r\n"); //关闭回显
    delay_ms(300);
    printf("AT+CFUN=1\r\n"); //打开射频功能
    delay_ms(300);
		Uart1_SendStr("正在检测卡状态...！\r\n");
		Clear_Buffer();	
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//返460，表明识别到卡了
    num = 3 ;
    while(strx==NULL&&num)
    {
        Uart1_SendStr("卡信息异常，请检查卡状态，即将自动重启！\r\n");
				printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
				num--;
    }
		if(num == 0)
		{
				Uart1_SendStr("卡信息获取失败！\r\n");
		}
		else
		{
				Uart1_SendStr("卡信息获取成功！\r\n");
		}
		Clear_Buffer();	
		printf("AT+CGPADDR=1\r\n");//查询IP，确保模组注网成功
		delay_ms(300);
		Clear_Buffer();	
		printf("AT+CGATT?\r\n");//查询激活状态
		delay_ms(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返1
		num = 3;
		while(strx==NULL&&num)
		{
				Clear_Buffer();	
				printf("AT+CGATT?\r\n");//获取激活状态
				delay_ms(300);
				strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返回1,表明注网成功
				num--;
		}
		if(num == 0)
		{
				Uart1_SendStr("注网失败！\r\n");
		}
		else
		{
				Uart1_SendStr("注网成功！\r\n");
		}
		Clear_Buffer();	
		printf("AT+CESQ\r\n");//查看获取CSQ值
		delay_ms(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CESQ");//返回CSQ
		if(strx)
		{
				BC26_Status.CSQ=(strx[7]-0x30)*10+(strx[8]-0x30);//获取CSQ
				if((BC26_Status.CSQ==99)||((strx[7]-0x30)==0))//说明扫网失败
				{
						while(1)
						{
								BC26_Status.netstatus=0;
								Uart1_SendStr("信号搜索失败，请查看原因!\r\n");
								RESET=1;//拉低
								delay_ms(300);
								delay_ms(300);	
								RESET=0;//复位模块
								delay_ms(300);//没有信号就复位             
						}
				}
        else
        {
						BC26_Status.netstatus=1;
						Uart1_SendStr("信号强！\r\n");
        }        
     }
}
void BC26_RegIOT(void)//平台注册
{
    char  BC26_IMEI[16] = {0};//IMEI值
		u8 num = 3;
    printf("AT+QLWCLOSE\r\n");//关闭上一次连接
		Uart1_SendStr("关闭上次连接\r\n");
    delay_ms(300);
    printf("AT+QLWDEL\r\n");//删除句柄
    delay_ms(300);
    printf("AT+QLWSERV=\"180.101.147.115\",5683\r\n");//登录电信平台
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
    }
    Clear_Buffer();	
    printf("AT+CGSN=1\r\n");//获取模块的IMEI号
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGSN:");//返+CGSN:
      while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+CGSN:");//返+CGSN:
    }
		memcpy(BC26_IMEI,strx+7,15);
		//Uart1_SendStr(BC26_IMEI);
    Clear_Buffer();	
    printf("AT+QLWCONF=%s\r\n",BC26_IMEI);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
    }
    Clear_Buffer();	   
    printf("AT+QLWADDOBJ=19,0,1,\"0\"\r\n");//注册上行需求
      delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
    }
    Clear_Buffer();
    printf("AT+QLWADDOBJ=19,1,1,\"0\"\r\n");//注册下行需求
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
    }
		Uart1_SendStr("注册成功\r\n");
    Clear_Buffer();
    printf("AT+QLWOPEN=0\r\n");//开启连接
    delay_ms(1000);
    strx=strstr((const char*)RxBuffer,(const char*)"CONNECT OK");//返CONNECT OK 
		while(strx==NULL&&num)
    {
				Clear_Buffer();
				printf("AT+QLWOPEN=0\r\n");//开启连接
				delay_ms(3000);
				strx=strstr((const char*)RxBuffer,(const char*)"CONNECT OK");//返CONNECT OK
				num--;
    }
		if(num == 0)
		{
				Uart1_SendStr("连接电信云平台失败！\r\n");
		}
		else
		{
				Uart1_SendStr("连接电信云平台成功！\r\n");
		}
    strx=strstr((const char*)RxBuffer,(const char*)"+QLWOBSERVE: 0,19,0,0");//返+QLWOBSERVE: 0,19,0,0 一定要判断，否则发数据会报错
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QLWOBSERVE: 0,19,0,0");//返+QLWOBSERVE: 0,19,0,0
    }
    Clear_Buffer();
    printf("AT+QLWCFG=\"dataformat\",1,1\r\n");//设置上下数据按照十六进制
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
    }
    Clear_Buffer();
}

void BC26_IOTSenddata(u8 *Data,u8 Datalen)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
{	
   printf("AT+QLWDATASEND=19,0,0,%d,%s,0x0100\r\n",Datalen,Data);//上发数据,
   delay_ms(300);
   strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//返SEND OK 
		while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//返SEND OK 
    }
    Uart1_SendStr(RxBuffer);
    RxCounter=0;
		Clear_Buffer();
    //IWDG_Feed();//喂狗  
}
