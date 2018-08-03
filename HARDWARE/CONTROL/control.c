#include "control.h"

u8 Sendbuff[4096];		//发送缓存

/**
**处理CMD函数
**参数：pBuff指向串口帧缓存，len缓存长度
**/
void Deal_CMD(u8 *CacheBuff,u16 len)
{
	u8 *pBuff,cmd;
	pBuff = CacheBuff;
	cmd=pBuff[0];
	
    switch(cmd)
    {
    //只读良率
    case CMD_READ:
		CMD_ReadHandle(pBuff,len);
        break;

    //握手+设置机台编号
    case CMD_SET:
		CMD_SetHandle(pBuff,len);
        break;
	
	//上位机读取机台配置信息
	case CMD_READ_ALLOCATION:
		CMD_ReadAllocationHandle(pBuff,len);
		break;
	
	//上位机发送机台配置更新
	case CMD_UPDATE_ALLOCATION:
		CMD_UpdateAllocationHandle(pBuff,len);
		break;

    //获取IP
    case CMD_WIFI1:
    case CMD_WIFI2:
		CMD_WiFiHandle(pBuff,len);
        break;
    }
}
/**
**读SD卡函数
**
**/
void SDCount(const TCHAR *path)
{
	u8 res;
	u16 i,j;
	u32 PASS,NG,NULL_;
	FIL fil;
	UINT bww;
	u32 data_len,slice,last;
	u8 SDbuff[4096*2];
	
	res = f_open(&fil,path, FA_OPEN_EXISTING |FA_READ);
	data_len = f_size(&fil);		//获取SD卡数据长度
	res = f_close(&fil);
	PASS = NG = NULL_= 0;
	
	slice = data_len/4096;		//获得分片数
	last = data_len%4096;
	if(slice >0)
	{
		for(i = 0;i < slice; i++)
		{
			res = f_open(&fil,path, FA_OPEN_EXISTING |FA_READ);
			f_lseek(&fil,fil.fptr+4096*i);
			res = f_read(&fil,SDbuff,4096,&bww);  //读取SD卡4096byte到缓存
			res = f_close(&fil);
			for(j = 0; j < 4096; j++)
			{
				if(SDbuff[j] == 0x55) PASS++;
				else if(SDbuff[j] == 0xaa) NG++;
				else if(SDbuff[j] == 0x88) NULL_++;
			}
		}
	}
	res = f_open(&fil,path, FA_OPEN_EXISTING |FA_READ);
	f_lseek(&fil,fil.fptr+4096*slice);
	res = f_read(&fil,SDbuff,last,&bww);
	res = f_close(&fil);
	for(i = 0;i < last;i++)
	{
		if(SDbuff[i] == 0x55) PASS++;
		else if(SDbuff[i] == 0xaa) NG++;
		else if(SDbuff[i] == 0x88) NULL_++;
	}
}

/**
**加校验和函数
**功能：在传入的data后面两位加校验和
**/
void Add_Scheck(u8 *data,u16 len)
{
    u16 checksum;
    checksum = (u16)checksum_32(data,len);
    data[len] = checksum&0x00ff;
    data[len+1] = checksum>>8;
}

/**
**获取SD卡数据长度
**/
u16 ReadDataLen()
{
	return 0;
}

/**
**组帧函数
**/
void Framing()
{
	Sendbuff[0] = CMD_READ;	
	Sendbuff[1] = TYPE_SET;
	Sendbuff[2] = 0x00;
	Sendbuff[3] = 0x00;
	Sendbuff[4] = 0x4c;
	Sendbuff[5] = 0x00;
	Sendbuff[6] = 0x55;
	Sendbuff[7] = 0x22;
	Sendbuff[8] = Get_FTMifstate();
	Sendbuff[9] = Get_FTCount();
	//.....
	Add_Scheck(Sendbuff,74);		//加校验和
	Usart_SendStr_length(UART5, Sendbuff, 76);
}

/**
**读取SD卡数据
**/
void ReadSD(u8 * buff)
{
}

/**
**处理读命令函数
**/
void CMD_ReadHandle(u8 *pBuff,u16 len)
{
	u8 CMD_Type;
	CMD_Type = pBuff[1];
	 //判断校验和是否正确
        if(Check_Data(pBuff,len))
        {
			CMD_ReadDataRight(CMD_Type);
        }
		//校验错误
        else
        {
			CMD_ReadDataError();
        }
}

/**
**处理配置命令函数
**/
void CMD_SetHandle(u8 *pBuff,u16 len)
{
	u8 CMD_Type;
	CMD_Type = pBuff[1];
	 //判断校验和是否正确
        if(Check_Data(pBuff,len))
        {
			CMD_SetDataRight(CMD_Type);
        }
		//校验错误
        else
        {
			CMD_SetDataError();
        }
}

/**
**WiFi帧处理函数
**/
void CMD_WiFiHandle(u8 *pBuff,u16 len)
{
	u8 IPString[16];
	u8 i;
	 //获取IP
    if((pBuff[5] == 'D'&&pBuff[6] == 'H')||(pBuff[4] == 'D'&&pBuff[5] == 'H'))
      {
        do
         {
            IPString[i] = pBuff[i+10];
             i++;
         } while(pBuff[i+10] != 0x2c);
         Usart_SendString(USART1, IPString);
        }
        //判断WiFi连接状态
     else if(pBuff[5] == 'o'||pBuff[4] == 'o')
        {
            if(pBuff[6] == 'n'||pBuff[5] == 'n')
            {
                Usart_SendString(USART1, "Connect");
            }
            else
            {
                Usart_SendString(USART1, "Disconnect");
            }
        }
}

/**
**接收数据正确处理
**/
void CMD_ReadDataRight(u8 CMD_Type)
{
	switch(CMD_Type)
	{
		//只读良率回复
		case TYPE_SET:
			SDCount("0:/BANK2/FLAG.TXT");			//统计良品、不良品数量
			Framing();			//组成发送帧，并发送
			break;
		
		//收到上位机接收正确反馈
		case TYPE_REC_RIGHT:
			break;
		
		//收到上位机接收错误反馈
		case TYPE_REC_ERROR:
			CMD_ReadDataError();
			break;
	}
}

/**
**接收数据错误
**/
void CMD_ReadDataError()
{
}

/**
**获取机台状态
**/
u8 Get_FTMifstate()
{
	return 0x02;
}

/**
**获取FT板数量
**/
u8 Get_FTCount()
{
	return 0x04;
}

/**
**数据正确
**/
void CMD_SetDataRight(u8 CMD_Type)
{
}

/**
**配置错误
**/
void CMD_SetDataError()
{
}

/**
**读配置函数
**/
void CMD_ReadAllocationHandle(u8 *pBuff,u16 len)
{
	u8 CMD_Type;
	CMD_Type = pBuff[1];
	 //判断校验和是否正确
        if(Check_Data(pBuff,len))
        {
			CMD_ReadAllocationRight(CMD_Type);
        }
		//校验错误
        else
        {
			CMD_ReadAllocationError();
        }
}

/**
**更新配置函数
**/
void CMD_UpdateAllocationHandle(u8 *pBuff,u16 len)
{
		u8 CMD_Type;
		CMD_Type = pBuff[1];
	 //判断校验和是否正确
        if(Check_Data(pBuff,len))
        {
			CMD_UpdateAllocationRight(CMD_Type);
        }
		//校验错误
        else
        {
			CMD_UpdateAllocationError();
        }
}

/**
**读配置正确
**/
void CMD_ReadAllocationRight(u8 CMD_Type)
{
}

/**
**读配置错误
**/
void CMD_ReadAllocationError()
{
}


/**
**更新配置正确
**/
void CMD_UpdateAllocationRight(u8 CMD_Type)
{
}

/**
**更新配置错误
**/
void CMD_UpdateAllocationError()
{
}

