#include "wifi.h"

/**
**����AT����ģʽ
**/
void EnterATMode(void)
{
	delay_ms(20);
	Send_AT("+++");
	delay_ms(20);				//������ʱ20ms
	Send_AT("a");
	delay_ms(20);
}

/**
**�˳�AT����ģʽ
**/
void ExitATMode(void)
{
	delay_ms(50);
	Send_AT("AT+ENTM\r");
	delay_ms(20);
}

/**
**��ȡWiFiģ��IP
**/
void GetIP(void)
{
	//delay_ms(100);
	EnterATMode();
	Send_AT("AT+WANN\r");
	ExitATMode();
}

/**
**��ȡTCP����״̬
**/
void GetTCP_STA()
{
	//delay_ms(100);
	EnterATMode();
	Send_AT("AT+TCPLK\n");
	ExitATMode();
}

/**
**����WiFiģ��
**/
void WIFIRestart()
{
	delay_ms(100);
	EnterATMode();
	Send_AT("AT+Z\n");
	ExitATMode();
}

/**
**����AT����
**/
void Send_AT(uint8_t *str)
{
	Usart_SendString(UART5, str);
}

