#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 	 

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)//读取按键1

 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 

void KEY_Init(void);//IO初始化				    
#endif
