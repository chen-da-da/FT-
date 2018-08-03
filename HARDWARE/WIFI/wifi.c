#include "wifi.h"

/**
**进入AT命令模式
**/
void EnterATMode(void)
{
	delay_ms(20);
	Send_AT("+++");
	delay_ms(20);				//必须延时20ms
	Send_AT("a");
	delay_ms(20);
}

/**
**退出AT命令模式
**/
void ExitATMode(void)
{
	delay_ms(50);
	Send_AT("AT+ENTM\r");
	delay_ms(20);
}

/**
**获取WiFi模块IP
**/
void GetIP(void)
{
	//delay_ms(100);
	EnterATMode();
	Send_AT("AT+WANN\r");
	ExitATMode();
}

/**
**获取TCP连接状态
**/
void GetTCP_STA()
{
	//delay_ms(100);
	EnterATMode();
	Send_AT("AT+TCPLK\n");
	ExitATMode();
}

/**
**重启WiFi模块
**/
void WIFIRestart()
{
	delay_ms(100);
	EnterATMode();
	Send_AT("AT+Z\n");
	ExitATMode();
}

/**
**发送AT命令
**/
void Send_AT(uint8_t *str)
{
	Usart_SendString(UART5, str);
}

