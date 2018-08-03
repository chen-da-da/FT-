#include "checksum.h"

/**
**计算校验和
**/
u32 checksum_32(u8 *buff, u16 len)
{
    int i;
    u32 cs;

    for(cs = 0 , i = 0 ; i < len-2; i++) {
        cs += buff[i];
    }
    return (cs);
}

/**
**检查校验和
**/
u8 Check_Data(u8 * buff,u16 len)
{
	u8 ture = 1;
	u8 flase = 0;
	u16 checksum;
	u16 temp;
	if(len<3)	return 0;
	temp = (u16)checksum_32(buff,len);
	checksum = buff[len-1];
	checksum = checksum<<8;
	checksum |= buff[len-2];
	if(checksum == temp)
		return ture;
	else 
		return flase;
}

