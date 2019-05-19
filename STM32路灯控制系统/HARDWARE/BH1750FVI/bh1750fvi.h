#ifndef _BH1750FVI_H
#define _BH1750FVI_H

#include "sys.h"
#include "myiic.h"
#include "delay.h"

void bh1750fvi_Init(void);
void bh1750fvi_Write(u8 Salve_addr,u8 Mode_addr);
u16 bh1750fvi_Read(u8 Salve_addr);

#endif
