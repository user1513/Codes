#include "hc595.h"
#include "delay.h"
#include "main.h"


void HC595_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOD, &GPIO_InitStructure);					 
					
	Date_OE = 0;
	Date_SHCP = 0;
	Date_STCP = 0;
	Date_DS = 0;
}


u8 display[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
extern u8 temperature;

void HC595TransmitData(void)
{
	u8 i,j;
	u8 HC595_Data[17];
	HC595_Data[16] = display[sys_clock_dev.hour / 16];	//时
	HC595_Data[15] = display[sys_clock_dev.hour % 16];
	
	HC595_Data[14] = display[sys_clock_dev.min / 16];	//分
	HC595_Data[13] = display[sys_clock_dev.min % 16];
	
	HC595_Data[14] = display[sys_clock_dev.min / 16];	//分
	HC595_Data[13] = display[sys_clock_dev.min % 16];

	HC595_Data[12] = ~display[2];
	HC595_Data[11] = ~display[0];
	HC595_Data[10] = ~display[sys_clock_dev.year / 16];	//年
	HC595_Data[9] = ~display[sys_clock_dev.year % 16];

	HC595_Data[8] = ~display[sys_clock_dev.month / 16];	//月
	HC595_Data[7] = ~display[sys_clock_dev.month % 16];

	HC595_Data[6] = ~display[sys_clock_dev.day / 16];	//日
	HC595_Data[5] = ~display[sys_clock_dev.day % 16];

	HC595_Data[4] = ~display[sys_clock_dev.tweek / 16];	//日
	HC595_Data[3] = ~display[sys_clock_dev.tweek % 16];
	
	HC595_Data[2] = ~display[sys_clock_dev.week];	//周
	
	HC595_Data[1] = ~display[temperature / 10];
	HC595_Data[0] = ~display[temperature % 10];
	for(j=0; j<13; j++)
	{
//		data = ~0x3f;
		for(i=0;i<8;i++)
		{
			Date_DS = HC595_Data[j]>>7;	//从高位到低位
			HC595_Data[j] <<= 1;
			Date_SHCP = 1;
			Date_SHCP = 0;
		}		
	}
	for(j=13; j<17; j++)
	{
		for(i=0;i<8;i++)
		{
			Date_DS = HC595_Data[j]>>7;	//从高位到低位
			HC595_Data[j] <<= 1;
			Date_SHCP = 1;
			Date_SHCP = 0;
		}		
	}
	
	//--输出--//
	Date_STCP = 0; //set dataline low
	Date_STCP = 1; //片选
	Date_STCP = 0; //set dataline low
}

