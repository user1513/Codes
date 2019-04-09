#ifndef _TEMP_H
#define _TEMP_H

#include <STC12C5A60S2.H>
#include "INTRINS.H"

sbit DQ = P1^6;                     //DS18B20�����ݿ�λP3.3
typedef unsigned char BYTE;
void DelayXus(BYTE n);
void DS18B20_Reset();
void DS18B20_WriteByte(BYTE dat);
BYTE DS18B20_ReadByte();
int Ds18b20ReadTemp();

float LcdDisplay(int temp);
#endif