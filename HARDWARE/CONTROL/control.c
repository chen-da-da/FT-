#include "control.h"

u8 Sendbuff[4096];		//���ͻ���

/**
**����CMD����
**������pBuffָ�򴮿�֡���棬len���泤��
**/
void Deal_CMD(u8 *CacheBuff,u16 len)
{
	u8 *pBuff,cmd;
	pBuff = CacheBuff;
	cmd=pBuff[0];
	
    switch(cmd)
    {
    //ֻ������
    case CMD_READ:
		CMD_ReadHandle(pBuff,len);
        break;

    //����+���û�̨���
    case CMD_SET:
		CMD_SetHandle(pBuff,len);
        break;
	
	//��λ����ȡ��̨������Ϣ
	case CMD_READ_ALLOCATION:
		CMD_ReadAllocationHandle(pBuff,len);
		break;
	
	//��λ�����ͻ�̨���ø���
	case CMD_UPDATE_ALLOCATION:
		CMD_UpdateAllocationHandle(pBuff,len);
		break;

    //��ȡIP
    case CMD_WIFI1:
    case CMD_WIFI2:
		CMD_WiFiHandle(pBuff,len);
        break;
    }
}
/**
**��SD������
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
	data_len = f_size(&fil);		//��ȡSD�����ݳ���
	res = f_close(&fil);
	PASS = NG = NULL_= 0;
	
	slice = data_len/4096;		//��÷�Ƭ��
	last = data_len%4096;
	if(slice >0)
	{
		for(i = 0;i < slice; i++)
		{
			res = f_open(&fil,path, FA_OPEN_EXISTING |FA_READ);
			f_lseek(&fil,fil.fptr+4096*i);
			res = f_read(&fil,SDbuff,4096,&bww);  //��ȡSD��4096byte������
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
**��У��ͺ���
**���ܣ��ڴ����data������λ��У���
**/
void Add_Scheck(u8 *data,u16 len)
{
    u16 checksum;
    checksum = (u16)checksum_32(data,len);
    data[len] = checksum&0x00ff;
    data[len+1] = checksum>>8;
}

/**
**��ȡSD�����ݳ���
**/
u16 ReadDataLen()
{
	return 0;
}

/**
**��֡����
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
	Add_Scheck(Sendbuff,74);		//��У���
	Usart_SendStr_length(UART5, Sendbuff, 76);
}

/**
**��ȡSD������
**/
void ReadSD(u8 * buff)
{
}

/**
**����������
**/
void CMD_ReadHandle(u8 *pBuff,u16 len)
{
	u8 CMD_Type;
	CMD_Type = pBuff[1];
	 //�ж�У����Ƿ���ȷ
        if(Check_Data(pBuff,len))
        {
			CMD_ReadDataRight(CMD_Type);
        }
		//У�����
        else
        {
			CMD_ReadDataError();
        }
}

/**
**�������������
**/
void CMD_SetHandle(u8 *pBuff,u16 len)
{
	u8 CMD_Type;
	CMD_Type = pBuff[1];
	 //�ж�У����Ƿ���ȷ
        if(Check_Data(pBuff,len))
        {
			CMD_SetDataRight(CMD_Type);
        }
		//У�����
        else
        {
			CMD_SetDataError();
        }
}

/**
**WiFi֡������
**/
void CMD_WiFiHandle(u8 *pBuff,u16 len)
{
	u8 IPString[16];
	u8 i;
	 //��ȡIP
    if((pBuff[5] == 'D'&&pBuff[6] == 'H')||(pBuff[4] == 'D'&&pBuff[5] == 'H'))
      {
        do
         {
            IPString[i] = pBuff[i+10];
             i++;
         } while(pBuff[i+10] != 0x2c);
         Usart_SendString(USART1, IPString);
        }
        //�ж�WiFi����״̬
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
**����������ȷ����
**/
void CMD_ReadDataRight(u8 CMD_Type)
{
	switch(CMD_Type)
	{
		//ֻ�����ʻظ�
		case TYPE_SET:
			SDCount("0:/BANK2/FLAG.TXT");			//ͳ����Ʒ������Ʒ����
			Framing();			//��ɷ���֡��������
			break;
		
		//�յ���λ��������ȷ����
		case TYPE_REC_RIGHT:
			break;
		
		//�յ���λ�����մ�����
		case TYPE_REC_ERROR:
			CMD_ReadDataError();
			break;
	}
}

/**
**�������ݴ���
**/
void CMD_ReadDataError()
{
}

/**
**��ȡ��̨״̬
**/
u8 Get_FTMifstate()
{
	return 0x02;
}

/**
**��ȡFT������
**/
u8 Get_FTCount()
{
	return 0x04;
}

/**
**������ȷ
**/
void CMD_SetDataRight(u8 CMD_Type)
{
}

/**
**���ô���
**/
void CMD_SetDataError()
{
}

/**
**�����ú���
**/
void CMD_ReadAllocationHandle(u8 *pBuff,u16 len)
{
	u8 CMD_Type;
	CMD_Type = pBuff[1];
	 //�ж�У����Ƿ���ȷ
        if(Check_Data(pBuff,len))
        {
			CMD_ReadAllocationRight(CMD_Type);
        }
		//У�����
        else
        {
			CMD_ReadAllocationError();
        }
}

/**
**�������ú���
**/
void CMD_UpdateAllocationHandle(u8 *pBuff,u16 len)
{
		u8 CMD_Type;
		CMD_Type = pBuff[1];
	 //�ж�У����Ƿ���ȷ
        if(Check_Data(pBuff,len))
        {
			CMD_UpdateAllocationRight(CMD_Type);
        }
		//У�����
        else
        {
			CMD_UpdateAllocationError();
        }
}

/**
**��������ȷ
**/
void CMD_ReadAllocationRight(u8 CMD_Type)
{
}

/**
**�����ô���
**/
void CMD_ReadAllocationError()
{
}


/**
**����������ȷ
**/
void CMD_UpdateAllocationRight(u8 CMD_Type)
{
}

/**
**�������ô���
**/
void CMD_UpdateAllocationError()
{
}

