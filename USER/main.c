#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "control.h"
#include "checksum.h"
#include "stm32f10x_it.h"
#include "wifi.h"
#include "exti.h"
#include "usmart.h" 
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"

int main(void)
{	
	void SYS_Init();
	u16 times=0;
	SYS_Init();
	
	//�����ã���˸LED
	while(1)
	{	
		times++;
		if(times%30==0)
		{
			LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			LED1 =! LED1;
		}
		delay_ms(10);    
	}
}

void SYS_Init()
{
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart1_init(115200);	 	//���ڳ�ʼ��
	uart2_init(115200);
	uart3_init(115200);
	uart4_init(115200);
	uart5_init(115200);
	EXTIX_Init();
	exfuns_init();			//Ϊfatfs��ر��������ڴ�
	LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();
	usmart_dev.init(72);
	mem_init();				//��ʼ���ڴ��
	f_mount(fs[0],"0:",1); 	//����SD��	
	
	while(SD_Initialize())					//���SD��
	{
		Dispaly_5x8_string(1,1,"SD Error",0);
		delay_ms(200);
	}
}

