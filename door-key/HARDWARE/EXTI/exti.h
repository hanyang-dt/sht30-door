#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 
void EXTI0_Init(void);//外部中断初始化	
char *Hex2Str ( uint8_t *pAddr, uint8_t len );//将数据变成字符串
int StrToHex(char *Hex,char *Str,int len);//将数据从字符串变成16进制
void send(void);
char *Hex2Str ( uint8_t *pAddr, uint8_t len );
#endif

