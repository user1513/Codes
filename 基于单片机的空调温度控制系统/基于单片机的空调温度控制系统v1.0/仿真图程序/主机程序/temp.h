#ifndef __TEMP_H_
#define __TEMP_H_

#include <app.h>
//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--����ʹ�õ�IO��--//
sbit DQ=P3^4;

//--����ȫ�ֺ���--//
void delay_TEMP(unsigned char time)	;
//*******DS18B20��λ********* 
void Init_DS18B20(void);
//*******��Ƭ����DS18B20дһ���ֽ�******
unsigned char ReadOneChar(void);
//******��Ƭ����DS18B20һ���ֽ�**********
void WriteOneChar(unsigned char dat);
//*******��Ƭ����ȡDS18B20���¶�*********
float ReadTemperature(void);

uint8_t * Ds18b20_datapros(float temp) 	 ;


#endif
