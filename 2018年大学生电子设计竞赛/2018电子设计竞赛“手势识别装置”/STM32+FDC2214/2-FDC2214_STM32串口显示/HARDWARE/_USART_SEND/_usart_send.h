#ifndef _USART_SEND_H
#define _USART_SEND_H

#include "sys.h"
#include "usart2.h"
#include "Data_Check.h"
#include "main.h"
#include "delay.h"

void Data_Return(u8 num);
void Send_Number_Data(u16 addr,u16 num);
void Real_Time_Data(void);
void Send_China_Data(u16 addr,u8 *data);
void Send_Number_Data_u32 (u16 addr,u32 num);
void Send_Page_Switch(u8 addr ,u16 Pageaddr);
void Test_result(u8 TestMode,u8 PageNum);
void SendFdc2214Data(void);

//DEMO

void Send_China_Demo(void);
#endif


