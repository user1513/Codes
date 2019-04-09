#include "app.h"
#include "NRF24L01.h"
#include "ds1302.h"
#include "usart.h"
#include "timer.h"
#include "lcd12864.h"
#include "stdio.h"
#include "temp.h"
#include "VS1838B.h"


extern xdata unsigned char IRCOM[];
extern uint8_t xdata RX_BUF[];




uint32_t STATE_FLAG = 0; //状态标志位


bit vs1838b_flag = 0;
void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
xdata uint8_t able[16] = {0};
void main()
{
	
	uint8_t data_ = 0;
	uint16_t temp = 0;
	P3M0=0x08;
	P33 = 0;
	UartInit();
	Delay1000ms();
	Lcm_Init();
	Delay1000ms();
	Lcm_Init();
	Delay1000ms();
	Lcm_Init();
	Wr_Data_String(0x80, (unsigned char *)"基于单片机的无线");
	Wr_Data_String(0x90, (unsigned char *)"红外防盗报警系统");
	Wr_Data_String(0x88, (unsigned char *)"姓名：管荣龙    ");
	Wr_Data_String(0x98, (unsigned char *)"学号：150401211 ");
	Delay1000ms();
	Delay1000ms();
	Delay1000ms();
	//NRF24L01初始化
	init_io();		           //无线初始化IO
	RX_Mode();		           //设置为接收模式
	
	Lcd12864Clean();
	//Ds1302Init();
	Timer0Init();
	vs1838b_Init();
	while(1)
	{
		if(STATE_FLAG & (1 << 0 ))
		{
			Ds1302ReadTime();
			STATE_FLAG &= ~(1 << 0);
			sprintf(able, "日期：%02d-%02d-%02d  ",(uint16_t)TIME[6],(uint16_t)TIME[4],(uint16_t)TIME[3]);
			//Send_String(able);
			Wr_Data_String(0x80, able);
			sprintf(able, "时间：%02d-%02d-%02d  ",(uint16_t)TIME[2],(uint16_t)TIME[1],(uint16_t)TIME[0]);
			//Send_String(able);
			Wr_Data_String(0x90, able);
			temp = LcdDisplay(Ds18b20ReadTemp());
			sprintf(able, "温度：%02d.%d    度",(uint16_t)temp/10, (uint16_t)temp%10);
			//Send_String(able);
			Wr_Data_String(0x88, able);
			if(IRCOM[6] == 0x34 && IRCOM[5] == 0x36)
			{
				IRCOM[6] = 0;
				IRCOM[5] = 0;
				vs1838b_flag = ~vs1838b_flag;
			}
			if(vs1838b_flag == 0)
				{
					//Send_String("模式：布防      ");
					Wr_Data_String(0x98, "模式：布防      ");
				}
				else
				{
					//Send_String("模式：撤防      ");
					Wr_Data_String(0x98, "模式：撤防      ");
				}
			if(vs1838b_flag == 0 && RX_BUF[2] == 0x01)
			{
				SendByte(0x88);
				P33 = ~P33;
			}	
			else
			{
				P33 = 0;
			}
			
			NRF24L01_Send();
		}
			
			
//		Delay1000ms();
		
	}
}