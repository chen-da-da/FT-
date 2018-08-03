#ifndef __LCD_H
#define __LCD_H
//#include "hw_config.h"
#include "sys.h"
#include "delay.h"

#define LCD_ALL			GPIOD
#define LCD_ALL_PORT	(GPIO_Pin_8 |GPIO_Pin_9|GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_12 |GPIO_Pin_13)

#define BitBand(Addr, Bit) 				*((volatile int*)(((int)(Addr) & 0x60000000) + 0x02000000 + (int)(Addr) * 0x20 + (Bit) * 4))

#define	LCD_CS		BitBand(&GPIOD->ODR, 11)			//LCD片选信号 :PD11
#define	LCD_RESET	BitBand(&GPIOD->ODR, 10)			//LCD复位:PD10
#define	LCD_RS		BitBand(&GPIOD->ODR, 9)				//LCD CMD/Data数据选择:PD9

#define	LCD_CLK		BitBand(&GPIOD->ODR, 13)			//LCD CLK:PD13				
#define	LCD_SDA		BitBand(&GPIOD->ODR, 12)			//LCD SDA:PD12	

#define Rom_IN		BitBand(&GPIOA->ODR, 7)		  		//字库IC接口定义:Rom_IN就是字库IC的SI:PA7  
#define Rom_OUT		BitBand(&GPIOA->IDR, 6)  			//字库IC接口定义:Rom_OUT就是字库IC的SO:PA6
#define Rom_SCK		BitBand(&GPIOA->ODR, 5)   			//字库IC接口定义:Rom_SCK就是字库IC的SCK:PA5
#define Rom_CS		BitBand(&GPIOA->ODR, 4)   			//字库IC接口定义Rom_CS就是字库IC的CS#:PA4
#define	LCD_GPower	BitBand(&GPIOD->ODR, 8)				//背光NPN控制:PD8


//宏定义
//----------------------LCD宏定义------------------------------------------------------
#define	Dummy						0xff
#define	ASCII_5_8_Inital_Adress		0x3bfc0
#define ASCII_8_16_Inital_Adress	0x3cf80
#define	ASCII_5_8_Wdith				5
#define	ASCII_8_16_Wdith			8
#define	GB2312_Wdith				16
//----------------------------------------------------------------------------------------
//----------------------字库宏定义--------------------------------
#define		CMD_EXTENSION1		0x30					//扩展命令1

#define		CMD_SETCOLUMN		0x15					//设置列地址：Start->0<Xs<FFh  End->Xs<Xe<FF
#define		CMD_SETPAGE			0x75					//设置页地址：Start->0<Ys<28h  End->Ys<Ye<28h
#define		CMD_WDDRAM			0x5C					//写入DDRAM
#define		CMD_RDDRAM			0x5D					//读DDRAM（只支持IIC）
#define		CMD_POWERUNSLEEP	0x94					//Power no sleep
#define		CMD_POWERSLEEP		0x95					//Power sleep
#define		CMD_DSD				0xBC					//数据扫描方向
#define		CMD_DFSML			0x08					//数据格式选择MSB-LSB
#define		CMD_DFSLM			0x0C					//数据格式选择LSB-MSB
#define		CMD_DC				0xCA					//显示控制命令
#define		CMD_DM				0xF0					//显示模式
#define     CMD_SVOP			0x81					//Set Vop
#define		CMD_POWER			0x20					//电源控制
#define		CMD_DISON			0xAF 					//显示开
#define		CMD_DISOFF			0xAE					//显示关闭
#define		CMD_IVDIS 			0xA7					//反显示
#define		CMD_NRDIS			0xA6					//常规显示
//-------------------------------------------------------------------
#define		CMD_EXTENSION2		0x31					//扩展命令2
#define		CMD_ARC				0xD7					//自动读取控制
#define		CMD_ARC_DISABLE		0x9F					//使能自动读取
#define		CMD_ARC_ENABLE		0x8F					//关闭自动读取
#define		CMD_ACS				0x32					//模拟电路设置
#define		CMD_GLS				0x20					//Gray leve set

//---------------------------------------------------------------------
typedef	enum	{LcdCmd,LcdData}Command_Data;
typedef	enum	{Dec,Hex}Dec_Hex;

//----------------函数声明---------------------
void LCD_Init(void);
void LCD_GPIO_Init(void);
void Clear_Screen(u8 column,u8 Page,u16 column_Total,u8 Page_Total);
u8 Display_String_GB2312(u8 column,u8 page,u8 *text,u8 reverse);
void LCD_Write_8Bit(u8 byte);
void LCD_Write_Byte(Command_Data Cmd_Dat,u8 Data);
void LCD_Address(u8 column,u8 page,u16 column_total,u8 page_total);
void Write_Read_16x16(u32 fontaddr,u8 page,u8 column,u8 reverse);
void Write_Read_8x16(u32 fontaddr,u8 page,u8 column,u8 reverse);
void Write_Read_5x8(u32 fontaddr,u8 page,u8 column,u8 reverse);
u8 SPI_ROM_SendByte( u8 byte );
u8 Display_5x8_Data(u8 column,u8 page,u32 ndata,Dec_Hex HEXorDEC,u8 reverse);
u8 Dispaly_5x8_char(u8 column,u8 page,u8 ascii,u8 reverse);
u8 Dispaly_5x8_string(u8 column,u8 page,u8 *text,u8 reverse);
void Display_String_Data_5x8(u8 column,u8 page,u8 *text,u32 ndata,Dec_Hex HEXorDEC,u8 reverse);
void Display_Data_String_5x8(u8 column,u8 page,u8 *text,u32 ndata,Dec_Hex HEXorDEC,u8 reverse);
void Display_Roll(u8 column,u8 page,u8 ascii,u8 percentage,u8 reverse);

u8 Display_8x16_Data(u8 column,u8 page,u32 ndata,Dec_Hex HEXorDEC,u8 reverse);
u8 Dispaly_8x16_char(u8 column,u8 page,u8 ascii,u8 reverse);
u8 Dispaly_8x16_string(u8 column,u8 page,u8 *text,u8 reverse);
void Display_String_Data_8x16(u8 column,u8 page,u8 *text,u32 ndata,Dec_Hex HEXorDEC,u8 reverse);
void Display_Data_String_8x16(u8 column,u8 page,u8 *text,u32 ndata,Dec_Hex HEXorDEC,u8 reverse);
 

void LCD_Display_Test(void);
void LCD_Display_Roll(void);

#endif
