#ifndef __TEMP_H_
#define __TEMP_H_

#include <app.h>
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--定义使用的IO口--//
sbit DQ=P3^4;

//--声明全局函数--//
void delay_TEMP(unsigned char time)	;
//*******DS18B20复位********* 
void Init_DS18B20(void);
//*******单片机向DS18B20写一个字节******
unsigned char ReadOneChar(void);
//******单片机读DS18B20一个字节**********
void WriteOneChar(unsigned char dat);
//*******单片机读取DS18B20的温度*********
float ReadTemperature(void);

uint8_t * Ds18b20_datapros(float temp) 	 ;


#endif
