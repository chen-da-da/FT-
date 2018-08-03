#ifndef __WIFI_H
#define __WIFI_H

#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "stm32f10x_it.h"

void EnterATMode(void);	//����AT����ģʽ
void ExitATMode(void);	//�˳�AT����ģʽ
void GetIP(void);		//��ȡIP
void GetTCP_STA(void);	//��ȡTCP����״̬
void WIFIRestart(void);	//����WiFiģ��
void Send_AT(uint8_t *str);

#endif

