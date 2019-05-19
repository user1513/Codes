#ifndef __DJIRECEIVER_DR16_H
#define __DJIRECEIVER_DR16_H

#include <STC8.H>
#include "uart2.h"
#include "uart1.h"
#include "pbdata.h"


void DR16DataAnalysis(u8 _data);
u16 ModBus_CRC(u8 *arg,u8 length);
void Send_dji_Data(u8 _cmd,u8 _data0,u8 _data1);
u8 Remote_Control(u16 ch0,u16 ch1,u8 data1,u8 data2);
int tabletennisdata(u16 ch2,u16 ch3,int _angleval);
void Artillerydata(u16 ch2,u16 ch3);
#endif