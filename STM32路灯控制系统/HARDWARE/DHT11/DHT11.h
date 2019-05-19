#ifndef _DHT11_H
#define _DHT11_H

#include "sys.h"
#include "delay.h"

#define dht11_In() {GPIOB->CRH&=0xFFF0FFFF;GPIOB->CRH|=8<<16 ;}
#define dht11_Out(){GPIOB->CRH&=0xFFF0FFFF;GPIOB->CRH|=3<<16 ;}

#define dht11_DQ_In PBin(12)
#define dht11_DQ_Out PBout(12)

void Dht11_Init(void);
void Dht11_Reset(void);
u8 Dht11_Check(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);
u8 DHT11_Read_Byte(void) ;
u8 Dht11_data(void);

#endif
