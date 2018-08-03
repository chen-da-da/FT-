/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/


#include "stm32f10x_it.h"
#include "control.h"


u8  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u8  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u8  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u8  USART4_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u8  USART5_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符

u16 USART1_RX_STA=0;       //接收状态标记
u16 USART2_RX_STA=0;       //接收状态标记
u16 USART3_RX_STA=0;       //接收状态标记
u16 USART4_RX_STA=0;       //接收状态标记
u16 USART5_RX_STA=0;       //接收状态标记

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}


void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
    u16 len;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
    {
        USART1_RX_BUF[USART1_RX_STA]=USART_ReceiveData(USART1);
        USART1_RX_STA++;
    }

    else if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE)!=RESET)
    {
        USART1->SR;
        USART1->DR;
        len = USART1_RX_STA;
		//USART1_RX_STA|=0x8000;
		USART1_RX_STA = 0;
		Deal_CMD(USART1_RX_BUF,len);
		
    }
}

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
    u16 len = 0;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
    {

        USART2_RX_BUF[USART2_RX_STA&0X3FFF]=USART_ReceiveData(USART2);
        USART2_RX_STA++;
    }

    else if(USART_GetFlagStatus(USART2,USART_FLAG_IDLE)!=RESET)
    {
        USART2->SR;
        USART2->DR;
        len = USART2_RX_STA;
		USART2_RX_STA = 0;
		USART2_RX_BUF[len] = '\0';
		Deal_CMD(USART2_RX_BUF,len);
		
    }
}

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u16 len = 0;
    if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
    {

        USART3_RX_BUF[USART3_RX_STA&0X3FFF]=USART_ReceiveData(USART3);
        USART3_RX_STA++;
    }

    else if(USART_GetFlagStatus(USART3,USART_FLAG_IDLE)!=RESET)
    {
        USART3->SR;
        USART3->DR;
        len = USART3_RX_STA;
		USART3_RX_STA = 0;
		Deal_CMD(USART3_RX_BUF,len);
    }
}

void UART4_IRQHandler(void)                	//串口4中断服务程序
{
    u16 len;
    if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)
    {

        USART4_RX_BUF[USART4_RX_STA&0X3FFF]=USART_ReceiveData(UART4);
        USART4_RX_STA++;
    }

    else if(USART_GetFlagStatus(UART4,USART_FLAG_IDLE)!=RESET)
    {
        UART4->SR;
        UART4->DR;
        len = USART4_RX_STA;
		USART4_RX_STA = 0;
		Deal_CMD(USART4_RX_BUF,len);
    }
}

void UART5_IRQHandler(void)                	//串口5中断服务程序
{
    u16 len;
    if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)
    {

        USART5_RX_BUF[USART5_RX_STA&0X3FFF]=USART_ReceiveData(UART5);
        USART5_RX_STA++;
    }

    else if(USART_GetFlagStatus(UART5,USART_FLAG_IDLE)!=RESET)
    {
        UART5->SR;
        UART5->DR;
        len = USART5_RX_STA;
		USART5_RX_STA = 0;
		Deal_CMD(USART5_RX_BUF,len);
		//Usart_SendString(USART1,USART2_RX_BUF);

    }
}

