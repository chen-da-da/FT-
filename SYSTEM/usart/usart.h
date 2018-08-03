#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "sys.h"
#include "string.h"

void uart1_init(u32 bound);		//´®¿Ú³õÊ¼»¯
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void uart4_init(u32 bound);
void uart5_init(u32 bound);

void SendData(USART_TypeDef * USARTx,u8 *String,u16 len);
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch );
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);

#endif


