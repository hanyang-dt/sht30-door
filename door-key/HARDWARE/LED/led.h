#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

void LED_Init(void);//��ʼ��
void Door_Init(void);
void BC26CTR_Init(void);
#define LED     PAout(1)
#define Door    PBin(4)
#define NETLED  PAout(15)
#define PWRKEY  PBout(8)
#define RESET   PAout(8)

#endif
