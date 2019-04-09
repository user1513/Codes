#ifndef __DS1302_H_
#define __DS1302_H_

//---����ͷ�ļ�---//
#include <app.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---����ds1302ʹ�õ�IO��---//
sbit DSIO=P2^2;
sbit RST=P2^0;
sbit SCLK=P2^1;

//---����ȫ�ֺ���---//
void Ds1302Write(uchar addr, uchar dat);
uchar Ds1302Read(uchar addr);
void Ds1302Init();
void Ds1302ReadTime();
uint8_t * Ds1302_datapros(void) ;	 

//---����ȫ�ֱ���--//
extern uchar TIME[7];	//����ȫ�ֱ���

#endif