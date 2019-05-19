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
void SendListInit(u8 _num,u16 addr);//代表發送多少組數據
void SendListT(u8 _seataddr,u8 * _seatmaxpeople,u8 * _seatpeople,u8 *_Surplus);//發送列表數據
void SendList(u8 _seataddr,u8 * _seatmaxpeople,u8 * _seatpeople,u8 *_Surplus);
void LightSet(u8 _lightval);
//DEMO

void Send_China_Demo(void);


#endif


