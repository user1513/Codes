#ifndef _USART_SEND_H
#define _USART_SEND_H

#include "sys.h"
#include "usart2.h"
#include "Data_Check.h"
#include "gps.h"

void Data_Return(u8 num);
void Send_Number_Data(u16 addr,u16 num);
void Real_Time_Data(void);
void Send_China_Data(u16 addr,u8 *data);
void Send_Mcu_Zigbee(u16 addr,u8 num,u8 *data);
void Send_Data_Page(u8 cState_Value);
void Send_Number_Data_u32 (u16 addr,u32 num);
void Send_Page_Switch(u8 addr,u16 num);

#endif


