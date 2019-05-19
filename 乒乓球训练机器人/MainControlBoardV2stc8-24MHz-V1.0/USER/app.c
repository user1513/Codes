#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "timer1.h"
#include "djiReceiver_dr16.h"
#include "pbdata.h"
#include <intrins.h>
#include "ccp.h"
#include "tabletennisdevice.h"
#define MotorMode P17

u16 G_dr16_ch0_value = 0;
u16 G_dr16_ch1_value = 0;
u16 G_dr16_ch2_value = 0;
u16 G_dr16_ch3_value = 0;

int tabletennisangle=tabletennisangleInit;

u8 Artilleryflag=0;//发炮标志位
extern u8 flag;


void Delay1000ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 122;
	j = 193;
	k = 128;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main(void)
{
	Uart1_Init();
	//ccp_Init();//没有使用
	Uart2Init();
	Timer0Init();
	Uart3Init();
	Delay1000ms();
	Delay1000ms();
	SteeringEngineAngle(tabletennisangle);//设置初始角度90度
	while(1)
	{
		if(UsartFlag == Enable&&flag==Enable)
		{
			Send_dji_Data(0x01,0x00,Remote_Control(G_dr16_ch0_value,G_dr16_ch1_value,(u8)MotorMode,0));//发送给小车的串口指令
			if(tabletennisangle>=180)tabletennisangle=180;
			if(tabletennisangle<=0)tabletennisangle=0;
			tabletennisangle=tabletennisdata(G_dr16_ch2_value,G_dr16_ch3_value,tabletennisangle);
			SteeringEngineAngle(tabletennisangle);
			Artillerydata(G_dr16_ch2_value,G_dr16_ch3_value);
			if(Artilleryflag&0x01==0x01&&Artilleryflag&0x80==0x80)
			{
				Artilleryflag=0;
				Artillery_Cmd();
			}
			flag = Disable;
			UsartFlag=Disable;
			ES=1;
		}		
	}
}
