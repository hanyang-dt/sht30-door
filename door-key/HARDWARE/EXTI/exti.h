#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 
void EXTI0_Init(void);//�ⲿ�жϳ�ʼ��	
char *Hex2Str ( uint8_t *pAddr, uint8_t len );//�����ݱ���ַ���
int StrToHex(char *Hex,char *Str,int len);//�����ݴ��ַ������16����
void send(void);
char *Hex2Str ( uint8_t *pAddr, uint8_t len );
#endif

