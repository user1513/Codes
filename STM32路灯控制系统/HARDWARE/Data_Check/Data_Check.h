#ifndef _DATA_CHECK_H
#define _DATA_CHECK_H


#include "sys.h"
#include "usart3.h"

u16 ModBus_CRC(u8 *arg,u8 length);//Modbus_CRC16У�麯��

u8 SUMCheck(u8 *Msg, u16 DatLen);//0-ADD8У�麯��

u8 SUMCheck_Add8(u8 *Msg, u16 DatLen);//ADD8У�麯��

u16 SUMCheck_Add16(u8 *Msg, u16 DatLen);//ADD16У�麯��

void HeartBeat_Send(u16 data);//���������ͺ���

#endif

