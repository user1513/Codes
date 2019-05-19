#ifndef _DATA_CHECK_H
#define _DATA_CHECK_H


#include "sys.h"
#include "usart3.h"

u16 ModBus_CRC(u8 *arg,u8 length);//Modbus_CRC16校验函数

u8 SUMCheck(u8 *Msg, u16 DatLen);//0-ADD8校验函数

u8 SUMCheck_Add8(u8 *Msg, u16 DatLen);//ADD8校验函数

u16 SUMCheck_Add16(u8 *Msg, u16 DatLen);//ADD16校验函数

void HeartBeat_Send(u16 data);//心跳包发送函数

#endif

