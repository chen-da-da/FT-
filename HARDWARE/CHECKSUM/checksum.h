#ifndef __CHECKSUM_H
#define __CHECKSUM_H

#include "sys.h"
#include "stdio.h"

u32 checksum_32(u8 *buff, u16 len);
u8 Check_Data(u8 * buff,u16 len);

#endif

