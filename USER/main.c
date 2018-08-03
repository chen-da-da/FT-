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
	
	//调试用，闪烁LED
	while(1)
	{	
		times++;
		if(times%30==0)
		{
			LED0=!LED0;//闪烁LED,提示系统正在运行.
			LED1 =! LED1;
		}
		delay_ms(10);    
	}
}

void SYS_Init()
{
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(115200);	 	//串口初始化
	uart2_init(115200);
	uart3_init(115200);
	uart4_init(115200);
	uart5_init(115200);
	EXTIX_Init();
	exfuns_init();			//为fatfs相关变量申请内存
	LED_Init();		  	 	//初始化与LED连接的硬件接口
	LCD_Init();
	usmart_dev.init(72);
	mem_init();				//初始化内存池
	f_mount(fs[0],"0:",1); 	//挂载SD卡	
	
	while(SD_Initialize())					//检测SD卡
	{
		Dispaly_5x8_string(1,1,"SD Error",0);
		delay_ms(200);
	}
}

