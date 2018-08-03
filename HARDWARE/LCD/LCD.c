#include "Lcd.h"

void LCD_Display_Roll(void)
{
	u8 percentage=0,Cnt=100;
	while(Cnt)
	{
		Cnt--;
		delay_ms(100);
		Display_Roll(1,10,' ',++percentage,1);
	}
	
}
void LCD_Display_Test(void)
{
	Clear_Screen(1,1,256,12);
	Dispaly_5x8_string(1,1,"01 MYTEST_V0.1 eb12b239 110000 00001",0);
	Dispaly_5x8_string(1,2,"01 MYTEST_V0.1 eb12b239 110000 00001",0);
	Dispaly_5x8_string(1,3,"01 MYTEST_V0.1 eb12b239 110000 00001",0);
	Dispaly_5x8_string(1,4,"01 MYTEST_V0.1 eb12b239 110000 00001",0);
	Dispaly_5x8_string(1,5,"01 MYTEST_V0.1 eb12b239 110000 00001",0);
	Dispaly_5x8_string(1,6,"01 MYTEST_V0.1 eb12b239 110000 00001",0);
	Dispaly_5x8_string(1,7,"01 MYTEST_V0.1 eb12b239 110000 00001",0);
	//LCD_Display_Roll();
	//Dispaly_8x16_string(1,1,"01 MYTEST_V0.1",0);
	//Dispaly_8x16_string(1,3,"01 MYTEST_V0.1",0);
	//Dispaly_8x16_char(1,1,'A',0);
	//Clear_Screen(1,1,256,12);
	
}
//-----------------------------------------------
//*************************************************************
// 函数名	：void Clear_Screen(INT8U Page,INT8U Page_Total)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：从Page地址开始，连续清Page_Total长度
//**************************************************************
void Clear_Screen(u8 column,u8 Page,u16 column_Total,u8 Page_Total)
{
	u16 i,j;	
	LCD_Address(column,Page,column_Total,Page_Total);
	for(i=(Page-1);i<(Page_Total+Page-1);i++)
	{
		for(j=(column-1);j<(column_Total+column-1);j++)
		{
			LCD_Write_Byte(LcdData,0x00);
		}
	}
}
//*************************************************************
// 函数名	：INT8U Display_String_GB2312(INT8U column,INT8U page,INT8U *text,INT8U reverse)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：显示字符串
//**************************************************************
u8 Display_String_GB2312(u8 column,u8 page,u8 *text,u8 reverse)
{
	u8 i= 0,Currentcolumn=0;
	u32 fontaddr=0;	
	while((text[i]>0x00))
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			//国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (u32)(fontaddr*32);
			Write_Read_16x16(fontaddr,page,column,reverse);	 //从指定地址读出数据写到液晶屏指定（column,page)座标中
			column+=GB2312_Wdith;
			Currentcolumn=column;
			i+=2;					
		}

		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			//国标简体（GB2312）15x16点的字符在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (u32)(fontaddr*32);
			Write_Read_16x16(fontaddr,page,column,reverse);	 //从指定地址读出数据写到液晶屏指定（column,page)座标中
			column+=GB2312_Wdith;
			Currentcolumn=column;
			i+=2;		
		}
			
		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{									
			fontaddr = (text[i]- 0x20);
			fontaddr = (u32)(fontaddr*16);
			fontaddr = (u32)(fontaddr+0x3cf80);			
			Write_Read_8x16(fontaddr,page,column,reverse);	 //从指定地址读出数据写到液晶屏指定（column,page)座标中
			column+=ASCII_8_16_Wdith;
			Currentcolumn=column;
			i+=1;
		}
		else
			i++;
	}
	return Currentcolumn;
}


//*************************************************************
// 函数名	： void Display_String_Data_5x8(INT8U column,INT8U page,INT8U *text,INT32U ndata,Dec_Hex HEXorDEC,INT8U reverse)
// 作者		：
// 功能		：写16进制数据
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：*text:显示的文本   ndata显示的数据
//**************************************************************
 void Display_String_Data_5x8(u8 column,u8 page,u8 *text,u32 ndata,Dec_Hex HEXorDEC,u8 reverse)
{
	u8 tempcolumn;
	tempcolumn=Dispaly_5x8_string(column,page,text,reverse);
	if(HEXorDEC==Hex)	Display_5x8_Data(tempcolumn,page,ndata,Hex,reverse);
	else	Display_5x8_Data(tempcolumn,page,ndata,Dec,reverse);	
}
//*************************************************************
// 函数名	： void Display_String_Data_5x8(INT8U column,INT8U page,INT8U *text,INT32U ndata,Dec_Hex HEXorDEC,INT8U reverse)
// 作者		：
// 功能		：写16进制数据
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：
//**************************************************************
 void Display_Data_String_5x8(u8 column,u8 page,u8 *text,u32 ndata,Dec_Hex HEXorDEC,u8 reverse)
{
	u8 tempcolumn;
	if(HEXorDEC==Hex)	tempcolumn=Display_5x8_Data(column,page,ndata,Hex,reverse);
	else	tempcolumn=Display_5x8_Data(column,page,ndata,Dec,reverse);
	Dispaly_5x8_string(tempcolumn,page,text,reverse);
}

//*************************************************************
// 函数名	： void Display_String_Data_5x8(INT8U column,INT8U page,INT8U *text,INT32U ndata,Dec_Hex HEXorDEC,INT8U reverse)
// 作者		：
// 功能		：写16进制数据
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：*text:显示的文本   ndata显示的数据
//**************************************************************
 void Display_String_Data_8x16(u8 column,u8 page,u8 *text,u32 ndata,Dec_Hex HEXorDEC,u8 reverse)
{
	u8 tempcolumn;
	tempcolumn=Dispaly_8x16_string(column,page,text,reverse);
	if(HEXorDEC==Hex)	Display_8x16_Data(tempcolumn,page,ndata,Hex,reverse);
	else	Display_8x16_Data(tempcolumn,page,ndata,Dec,reverse);	
}
//*************************************************************
// 函数名	： void Display_String_Data_5x8(INT8U column,INT8U page,INT8U *text,INT32U ndata,Dec_Hex HEXorDEC,INT8U reverse)
// 作者		：
// 功能		：写16进制数据
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：
//**************************************************************
 void Display_Data_String_8x16(u8 column,u8 page,u8 *text,u32 ndata,Dec_Hex HEXorDEC,u8 reverse)
{
	u8 tempcolumn;
	if(HEXorDEC==Hex)	tempcolumn=Display_8x16_Data(column,page,ndata,Hex,reverse);
	else	tempcolumn=Display_8x16_Data(column,page,ndata,Dec,reverse);
	Dispaly_8x16_string(tempcolumn,page,text,reverse);
}

//*************************************************************
// 函数名	：INT8U Display_String_5x8(INT8U page,INT8U column,INT8U *text)
// 作者		：
// 功能		：写16进制数据
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：
//**************************************************************
u8 Display_8x16_Data(u8 column,u8 page,u32 ndata,Dec_Hex HEXorDEC,u8 reverse)
{
	u32  temp,length;
	u8  i,Currentcolumn=0;
	length=0;
	temp=ndata;
	if(ndata==0)
	{
		Currentcolumn=Dispaly_8x16_char(column,page,'0',reverse);	
		return (Currentcolumn);
	}
	while(temp)
	{
		if(HEXorDEC==Hex)	temp>>=4;
		else	temp/=10;
		length++;	
		column+=ASCII_8_16_Wdith;
		Currentcolumn=column;
	}
	for(i=0;i<length;i++)
	{
		column-=ASCII_8_16_Wdith;
		if(HEXorDEC==Hex)
		{
			if((ndata&0x0f)<=9)	Dispaly_8x16_char(column,page,(ndata&0x0f)+'0',reverse);  //0~9
			else	Dispaly_8x16_char(column,page,((ndata&0x0f)-10)+'A',reverse);   		//A~F
			ndata>>=4;
		}
		else
		{
			Dispaly_8x16_char(column,page,(ndata%10)+'0',reverse);
			ndata/=10;
		}
		
	}
	return Currentcolumn;
}

//*************************************************************
// 函数名	：INT8U Display_String_5x8(INT8U page,INT8U column,INT8U *text)
// 作者		：
// 功能		：写16进制数据
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：
//**************************************************************
u8 Display_5x8_Data(u8 column,u8 page,u32 ndata,Dec_Hex HEXorDEC,u8 reverse)
{
	u32  temp,length;
	u8  i,Currentcolumn=0;
	length=0;
	temp=ndata;
	if(ndata==0)
	{
		Currentcolumn=Dispaly_5x8_char(column,page,'0',reverse);	
		return (Currentcolumn);
	}
	while(temp)
	{
		if(HEXorDEC==Hex)	temp>>=4;
		else	temp/=10;
		length++;	
		column+=ASCII_5_8_Wdith;
		Currentcolumn=column;
	}
	for(i=0;i<length;i++)
	{
		column-=ASCII_5_8_Wdith;
		if(HEXorDEC==Hex)
		{
			if((ndata&0x0f)<=9)	Dispaly_5x8_char(column,page,(ndata&0x0f)+'0',reverse);  //0~9
			else	Dispaly_5x8_char(column,page,((ndata&0x0f)-10)+'A',reverse);   		//A~F
			ndata>>=4;
		}
		else
		{
			Dispaly_5x8_char(column,page,(ndata%10)+'0',reverse);
			ndata/=10;
		}
		
	}
	return Currentcolumn;
}

//*************************************************************
// 函数名	：uchar Dispaly_5x8_char(uchar column,uchar page,uchar ascii,uchar reverse)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：显示5X8字符
//**************************************************************
u8 Dispaly_8x16_char(u8 column,u8 page,u8 ascii,u8 reverse)
{
	u8  Currentcolumn=0;
	u32  FontInitalAdress;	
	if((ascii>=0x20)&&(ascii<=0x7E))	
	{						
		FontInitalAdress = ascii-' ';
		FontInitalAdress = (u32)(FontInitalAdress*16);		//8*16,16Byte存储一个ASCII
		FontInitalAdress = (u32)(FontInitalAdress+0x3b7c0);			
		Write_Read_8x16(FontInitalAdress,page,column,reverse);	 //从指定地址读出数据写到液晶屏相应（page,column)坐标
		Currentcolumn=column;
	}
	return Currentcolumn;
		
}
//*************************************************************
// 函数名	：uchar Dispaly_5x8_char(uchar column,uchar page,uchar ascii,uchar reverse)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：显示5X8字符
//**************************************************************
u8 Dispaly_5x8_char(u8 column,u8 page,u8 ascii,u8 reverse)
{
	u8  Currentcolumn=0;
	u32  FontInitalAdress;	
	if((ascii>=0x20)&&(ascii<=0x7E))	
	{						
		FontInitalAdress = ascii-' ';
		FontInitalAdress = (u32)(FontInitalAdress*8);		//5*8,8Byte存储一个ASCII
		FontInitalAdress = (u32)(FontInitalAdress+0x3bfc0);			
		Write_Read_5x8(FontInitalAdress,page,column,reverse);	 //从指定地址读出数据写到液晶屏相应（page,column)坐标
		Currentcolumn=column;
	}
	return Currentcolumn;
		
}

//*************************************************************
// 函数名	：INT8U Dispaly_5x8_string(uchar column,uchar page,uchar *text,uchar reverse)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：显示5X8字符串
//**************************************************************
u8 Dispaly_8x16_string(u8 column,u8 page,u8 *text,u8 reverse)
{
	u8  Currentcolumn=0;
	u32  FontInitalAdress;	
	while((*text>0x00))
	{
		if((*text>=0x20)&&(*text<=0x7E))	
		{						
			FontInitalAdress = (*text-0x20);
			FontInitalAdress = (u32)(FontInitalAdress*16);		//8*16,16Byte存储一个ASCII
			FontInitalAdress = (u32)(FontInitalAdress+0x3b7c0);			
			Write_Read_8x16(FontInitalAdress,page,column,reverse);	 //从指定地址读出数据写到液晶屏相应（page,column)坐标
			column+=ASCII_8_16_Wdith;
			Currentcolumn=column;
			text++;
		}
		else
		text++;	
	}
	return 	  Currentcolumn;
}
//*************************************************************
// 函数名	：INT8U Dispaly_5x8_string(uchar column,uchar page,uchar *text,uchar reverse)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：显示5X8字符串
//**************************************************************
u8 Dispaly_5x8_string(u8 column,u8 page,u8 *text,u8 reverse)
{
	u8  Currentcolumn=0;
	u32  FontInitalAdress;	
	while((*text>0x00))
	{
		if((*text>=0x20)&&(*text<=0x7E))	
		{						
			FontInitalAdress = (*text-0x20);
			FontInitalAdress = (u32)(FontInitalAdress*8);		//5*7,8Byte存储一个ASCII
			FontInitalAdress = (u32)(FontInitalAdress+0x3bfc0);			
			Write_Read_5x8(FontInitalAdress,page,column,reverse);	 //从指定地址读出数据写到液晶屏相应（page,column)坐标
			column+=ASCII_5_8_Wdith;
			Currentcolumn=column;
			text++;
		}
		else
		text++;	
	}
	return 	  Currentcolumn;
}

//*************************************************************
// 函数名	：void Write_Read_16x16(INT32U fontaddr,INT8U column,INT8U page,INT8U reverse)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：显示16X16的汉字，reverse=1：反显  resverse=0:正显
//**************************************************************
void Write_Read_16x16(u32 fontaddr,u8 page,u8 column,u8 reverse) 
{
 	u8 i,j,disp_data;
	Rom_CS = 0;
	SPI_ROM_SendByte(0x03);
	SPI_ROM_SendByte((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	SPI_ROM_SendByte((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	SPI_ROM_SendByte(fontaddr&0xff);			//地址的低8位,共24位

	LCD_Address(column,page,16,2);				//16行2页
	for(j=0;j<2;j++)
	{
	   for(i=0; i<16; i++ )
	  {
			disp_data=SPI_ROM_SendByte(Dummy);
			if(reverse==1)		LCD_Write_Byte(LcdData,~disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
			else				LCD_Write_Byte(LcdData,disp_data);
	  }
	}
	Rom_CS=1;
}
//*************************************************************
// 函数名	：void Write_Read_8x16(INT32U fontaddr,INT8U column,INT8U page,INT8U reverse)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：显示8X16的ASCII码,即粗体ASCII，reverse=1：反显  resverse=0:正显
//**************************************************************
void Write_Read_8x16(u32 fontaddr,u8 page,u8 column,u8 reverse) 
{
	u8 i,j,disp_data;
	Rom_CS = 0;
	SPI_ROM_SendByte(0x03);
	SPI_ROM_SendByte((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	SPI_ROM_SendByte((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	SPI_ROM_SendByte(fontaddr&0xff);			//地址的低8位,共24位

	LCD_Address(column,page,8,2);				//8列2页
	for(j=0;j<2;j++)
	{
		for(i=0; i<8; i++ )
	  {
			disp_data=SPI_ROM_SendByte(Dummy);
			if(reverse==1)	LCD_Write_Byte(LcdData,~disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
			else			LCD_Write_Byte(LcdData,disp_data);
	  }
	}
	Rom_CS=1;
}
//*************************************************************
// 函数名	：INT8U Dispaly_5x8_string(uchar column,uchar page,uchar *text,uchar reverse)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：显示5X8字符串
//**************************************************************
void Write_Read_5x8(u32 fontaddr,u8 page,u8 column,u8 reverse) 
{
	u8  i,DispData;
	Rom_CS = 0;
	SPI_ROM_SendByte(0x03);
	SPI_ROM_SendByte((fontaddr&0xff0000)>>16);	 	//地址的高8位,共24位
	SPI_ROM_SendByte((fontaddr&0xff00)>>8);			//地址的中8位,共24位
	SPI_ROM_SendByte(fontaddr&0xff);					//地址的低8位,共24位
	LCD_Address(column,page,5,2);
	for(i=0; i<5; i++ )
	{
		DispData=SPI_ROM_SendByte(Dummy);
		if(reverse==1)	  	LCD_Write_Byte(LcdData,~DispData);			//写数据到LCD,每写完1字节的数据后列地址自动加1						
		else	 			LCD_Write_Byte(LcdData,DispData);			//写数据到LCD,每写完1字节的数据后列地址自动加1		
	}
	Rom_CS=1;
}


//*************************************************************
// 函数名	：void LCD_init(void)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：Cmd_Dat=0 写CMD; Cmd_Dat=1写数据
//**************************************************************
void LCD_Init(void)
{
	LCD_GPIO_Init();
 	//LCD_GPower=1;
	LCD_RESET=0;
	delay_ms(1);
	LCD_RESET=1;
	delay_ms(1);

	LCD_Write_Byte(LcdCmd,CMD_EXTENSION1);   			//外部扩展命令1
	LCD_Write_Byte(LcdCmd,CMD_POWERUNSLEEP);  			//Sleep out

	LCD_Write_Byte(LcdCmd,CMD_EXTENSION2);   			//外部扩展命令2
	LCD_Write_Byte(LcdCmd,CMD_ARC);   					//Autoread disable
	LCD_Write_Byte(LcdData,CMD_ARC_DISABLE);     
	LCD_Write_Byte(LcdCmd,CMD_ACS);   					//Analog SET
	LCD_Write_Byte(LcdData,0x00);  						//OSC Frequency adjustment
	LCD_Write_Byte(LcdData,0x01);  						//Frequency on booster capacitors->6KHz
	LCD_Write_Byte(LcdData,0x03);  						//Bias=1/11
	LCD_Write_Byte(LcdCmd,CMD_GLS);   					// Gray Level
	LCD_Write_Byte(LcdData,0x01); 
	LCD_Write_Byte(LcdData,0x03);
	LCD_Write_Byte(LcdData,0x05);
	LCD_Write_Byte(LcdData,0x07); 
	LCD_Write_Byte(LcdData,0x09);
	LCD_Write_Byte(LcdData,0x0b);
	LCD_Write_Byte(LcdData,0x0d);
	LCD_Write_Byte(LcdData,0x10);
	LCD_Write_Byte(LcdData,0x11);
	LCD_Write_Byte(LcdData,0x13);
	LCD_Write_Byte(LcdData,0x15);
	LCD_Write_Byte(LcdData,0x17);
	LCD_Write_Byte(LcdData,0x19);
	LCD_Write_Byte(LcdData,0x1b);
	LCD_Write_Byte(LcdData,0x1d);
	LCD_Write_Byte(LcdData,0x1f);
	LCD_Write_Byte(LcdCmd,CMD_EXTENSION1);   
	LCD_Write_Byte(LcdCmd,CMD_SETPAGE);   				//Page Address setting:原IC为256*160
	LCD_Write_Byte(LcdData,0X00);      					// XS=0
	LCD_Write_Byte(LcdData,0X13);      					// XE=159 0x14：0~19页
	LCD_Write_Byte(LcdCmd,CMD_SETCOLUMN);   			//Clumn Address setting
	LCD_Write_Byte(LcdData,0X00);      					// XS=0
	LCD_Write_Byte(LcdData,0Xff);      					// XE=256

	LCD_Write_Byte(LcdCmd,CMD_DSD);    	 				//Data scan direction
	LCD_Write_Byte(LcdData,0x00);  		 				//MX.MY=Normal

	LCD_Write_Byte(LcdCmd,CMD_NRDIS);		 			//常规显示	
	LCD_Write_Byte(LcdCmd,CMD_DFSLM);    				//数据格式选择,0x0C是低位在前D0-D7,0x08是高位在前D7-D0
	LCD_Write_Byte(LcdCmd,CMD_DC);    					//显示控制命令
	LCD_Write_Byte(LcdData,0X00);  		 				//CLD=0 
	LCD_Write_Byte(LcdData,0X9F);  		 				//Duty=160
	LCD_Write_Byte(LcdData,0X20);  		 				//Nline=off

	LCD_Write_Byte(LcdCmd,CMD_DM);    					//Display Mode 
	LCD_Write_Byte(LcdData,0X10);  						//10=Mono(默认) Mode,11=4Gray;没有选择灰度，灰度设置无意义

	LCD_Write_Byte(LcdCmd,CMD_SVOP);    				//EV control
	LCD_Write_Byte(LcdData,0x38);   					//VPR[5-0]
	LCD_Write_Byte(LcdData,0x03);  						//粗调对比度

	LCD_Write_Byte(LcdCmd,CMD_POWER);    				//Power control
	LCD_Write_Byte(LcdData,0x0B);   					//D0=regulator ; D1=follower ; D3=booste,  on:1 off:0

	delay_ms(1);           
	LCD_Write_Byte(LcdCmd,CMD_DISON);    				//Display on
	delay_ms(10);
	Clear_Screen(1,1,256,12);
}

void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|\
							RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF,ENABLE);
//LCD:PD8~PD13
//字库：PA4~PA7
	GPIO_InitStructure.GPIO_Pin = LCD_ALL_PORT ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_ALL, &GPIO_InitStructure);
	GPIO_SetBits(LCD_ALL, LCD_ALL_PORT);						//LCD驱动初始为高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, (GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7));
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
}

//*************************************************************
// 函数名	：void Display_Roll(INT8U page,INT8U column,INT8U *text,INT8U percentage)
// 作者		：
// 功能		：滚动|
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：
//**************************************************************
void Display_Roll(u8 column,u8 page,u8 ascii,u8 percentage,u8 reverse)
{
	u8 TempPercentage;
	TempPercentage=percentage/3;
	if(TempPercentage==0)	TempPercentage=1;
	while(TempPercentage--)
	{
		Dispaly_5x8_char(column,page,ascii,reverse);
		column+=ASCII_5_8_Wdith;
	}
	Display_Data_String_5x8(ASCII_5_8_Wdith*32+1,page,"%",percentage,Dec,0);
}
//*************************************************************
// 函数名	：void LCD_Address(INT8U column,INT8U page,INT8U column_total,INT8U page_total)
// 作者		：
// 功能		：设置写LCD 的地址
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：写LCD 行列地址：X为起始的列地址，Y为起始的行地址，x_total,y_total分别为列地址及行地址的起点到终点的差值
//**************************************************************
void LCD_Address(u8 column,u8 page,u16 column_total,u8 page_total)
{
 	column-=1;								//修正列地址:
	page=page+7;							//修正行地址：即第0页~第N页：每8行为一页；原IC是256*160，这颗IC应该是底部对齐。
	LCD_Write_Byte(LcdCmd,CMD_SETCOLUMN);	//Set Column Address
	LCD_Write_Byte(LcdData,column);			//初始的列地址	
	LCD_Write_Byte(LcdData,(u32)column+column_total-1);	//		

	LCD_Write_Byte(LcdCmd,CMD_SETPAGE);		//Set Page Address	
	LCD_Write_Byte(LcdData,page);		
	LCD_Write_Byte(LcdData,page+page_total-1);	
	LCD_Write_Byte(LcdCmd,CMD_EXTENSION1);	
	LCD_Write_Byte(LcdCmd,CMD_WDDRAM);
}


//*************************************************************
// 函数名	：void LCD_Write_Byte(INT8U Cmd_Dat,INT8U Data)
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：Cmd_Dat=0 写CMD; Cmd_Dat=1写数据
//**************************************************************

void LCD_Write_Byte(Command_Data Cmd_Dat,u8 Data)
{
	LCD_CS=0;			//片选拉低
	LCD_RS=Cmd_Dat;		//0:CMD  1:Data
	LCD_Write_8Bit(Data);
	LCD_CS=1;			//片选拉高
}

//*************************************************************
// 函数名	：void LCD_SPI_8Byte(INT8U byte)//发送一个字节节
// 作者		：
// 功能		：模拟SPI通讯LCD
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：
//**************************************************************
void LCD_Write_8Bit(u8 byte)//发送一个字节
{
	u8 i;									
	for(i=0;i<8;i++)
	{
		LCD_CLK=0;
		delay_us(20);
		if(byte&0x80)	LCD_SDA=1;
		else		LCD_SDA=0;
		LCD_CLK=1;
		delay_us(20);
		byte=byte<<1;
	}
}
//*************************************************************
// 函数名	：INT8U SPI_ROM_SendByte( INT8U byte )
// 作者		：
// 功能		：
// 参数		：
// 返回值	：
// 调用全局变量：无
// 修改全局变量：无
// 备注		：向字库写数据
//**************************************************************
u8 SPI_ROM_SendByte( u8 byte )
{
	u8 i;
	u8 ReceiveData=0;									
	for(i=0;i<8;i++)
	{
		Rom_SCK=0;
		if(byte&0x80)	Rom_IN=1;
		else	Rom_IN=0;
		byte<<=1;
		Rom_SCK=1;
		ReceiveData<<=1;
		if(Rom_OUT)	ReceiveData|=0x01;
		Rom_SCK=0;
	}
	return	ReceiveData;
}


