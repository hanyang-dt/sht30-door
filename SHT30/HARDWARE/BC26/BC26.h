#ifndef __BC26_H
#define __BC26_H	
#include "usart.h"
#include <stm32f10x.h>
#include "delay.h"
void Clear_Buffer(void);//��ջ���
void OPEN_BC26(void);
void WakeUp_BC26(void);
void BC26_Init(void);
void BC26_RECData(void);
void BC26_RegIOT(void);
void BC26_IOTSenddata(u8 *Data,u8 Datalen);
typedef struct
{
    uint8_t CSQ;    
    uint8_t Socketnum;   //���
    uint8_t reclen[10];   //��ȡ�����ݵĳ���
    uint8_t res;      
    uint8_t recdatalen[10];
    uint8_t recdata[100];
    uint8_t netstatus;//����ָʾ��
} BC26;
#endif
