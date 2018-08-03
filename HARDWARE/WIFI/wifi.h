#ifndef __WIFI_H
#define __WIFI_H

#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "stm32f10x_it.h"

void EnterATMode(void);	//进入AT命令模式
void ExitATMode(void);	//退出AT命令模式
void GetIP(void);		//获取IP
void GetTCP_STA(void);	//获取TCP连接状态
void WIFIRestart(void);	//重启WiFi模块
void Send_AT(uint8_t *str);

#endif

