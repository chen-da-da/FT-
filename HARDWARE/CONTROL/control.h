#ifndef __CONTROL_H
#define __CONTROL_H

#include "checksum.h"
#include "sys.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "integer.h"
#include "ff.h"
#include "fattester.h"
#include "lcd.h"

//CMD为命令，TYPE为命令类型
#define CMD_READ 0x16				//读命令
#define CMD_SET 0x10				//上位机握手+设置机台编号	
#define CMD_READ_ALLOCATION 0x11	//上位机读取机台配置信息
#define CMD_UPDATE_ALLOCATION 0x12	//上位机发送机台配置更新
#define CMD_WIFI1 'a'
#define CMD_WIFI2 '+'

#define TYPE_REC_RIGHT 0x51			//数据接收正确
#define TYPE_REC_ERROR 0xa1			//数据接收错误
#define TYPE_REC_UPDATE_RIGHT 0x52	//配置更新接收正确
#define TYPE_REC_UPDATE_ERROR 0xa2	//配置更新接收正确
#define TYPE_SET 0x01				//命令类型
#define TYPE_SET_ALLOCATION 0x02	//上位机发送配置更新

//帧头尾部联合体
union Tail
{
    u8 Cs[2];	//校验和
    u16 Mfstate_Count;	//机台状态和FT板数量
};


//帧头数据
__packed typedef struct
{
    u8 type;	//命令类型
    u8 pack;	//总包数
    u8 packindex;//包号
    u16 len;	//帧长度
    u16 MIFNO;	//机台编号
    //union Tail tail;
} Pack_Head;

//帧数据（FT板数据）
__packed typedef struct
{
    u8 index;	//机位
    u16 FTID;	//FT板编号
    u8 state;	//FT状态
    u32 OKNO;	//pass数量
    u32 NGNO;	//NG数量
    u32 null_;	//预留
} Data;

//只读良率
__packed typedef struct
{
    u8 cmd;	//命令
    Pack_Head packHeadData;	//帧头
    Data  currentData;	//帧数据
    u16 Cs;	//校验和
} Frame;


void ControlRec_Host(u8 *USART_RX_BUF);	//主控响应函数
void Deal_CMD(u8 *USART_RX_BUF,u16 len);
void Add_Scheck(u8 *data,u16 len);
u16 ReadDataLen(void);
void SDCount(const TCHAR *path);
void Framing(void);
void ReadSD(u8 * buff);
void CMD_ReadHandle(u8 *pBuff,u16 len);
void CMD_SetHandle(u8 *pBuff,u16 len);
void CMD_WiFiHandle(u8 *pBuff,u16 len);
void CMD_ReadDataRight(u8 CMD_Type);
void CMD_ReadDataError(void);
u8 Get_FTMifstate(void);
u8 Get_FTCount(void);
void CMD_SetDataRight(u8 CMD_Type);
void CMD_SetDataError(void);
void CMD_ReadAllocationHandle(u8 *pBuff,u16 len);
void CMD_UpdateAllocationHandle(u8 *pBuff,u16 len);
void CMD_ReadAllocationRight(u8 CMD_Type);
void CMD_ReadAllocationError(void);
void CMD_UpdateAllocationRight(u8 CMD_Type);
void CMD_UpdateAllocationError(void);

#endif

