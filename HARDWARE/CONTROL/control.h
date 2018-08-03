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

//CMDΪ���TYPEΪ��������
#define CMD_READ 0x16				//������
#define CMD_SET 0x10				//��λ������+���û�̨���	
#define CMD_READ_ALLOCATION 0x11	//��λ����ȡ��̨������Ϣ
#define CMD_UPDATE_ALLOCATION 0x12	//��λ�����ͻ�̨���ø���
#define CMD_WIFI1 'a'
#define CMD_WIFI2 '+'

#define TYPE_REC_RIGHT 0x51			//���ݽ�����ȷ
#define TYPE_REC_ERROR 0xa1			//���ݽ��մ���
#define TYPE_REC_UPDATE_RIGHT 0x52	//���ø��½�����ȷ
#define TYPE_REC_UPDATE_ERROR 0xa2	//���ø��½�����ȷ
#define TYPE_SET 0x01				//��������
#define TYPE_SET_ALLOCATION 0x02	//��λ���������ø���

//֡ͷβ��������
union Tail
{
    u8 Cs[2];	//У���
    u16 Mfstate_Count;	//��̨״̬��FT������
};


//֡ͷ����
__packed typedef struct
{
    u8 type;	//��������
    u8 pack;	//�ܰ���
    u8 packindex;//����
    u16 len;	//֡����
    u16 MIFNO;	//��̨���
    //union Tail tail;
} Pack_Head;

//֡���ݣ�FT�����ݣ�
__packed typedef struct
{
    u8 index;	//��λ
    u16 FTID;	//FT����
    u8 state;	//FT״̬
    u32 OKNO;	//pass����
    u32 NGNO;	//NG����
    u32 null_;	//Ԥ��
} Data;

//ֻ������
__packed typedef struct
{
    u8 cmd;	//����
    Pack_Head packHeadData;	//֡ͷ
    Data  currentData;	//֡����
    u16 Cs;	//У���
} Frame;


void ControlRec_Host(u8 *USART_RX_BUF);	//������Ӧ����
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

