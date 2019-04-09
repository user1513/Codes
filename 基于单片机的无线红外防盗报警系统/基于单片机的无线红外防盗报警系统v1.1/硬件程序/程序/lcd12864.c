#include "lcd12864.h"

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 54;
	j = 198;
	do
	{
		while (--j);
	} while (--i);
}
void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 24;
	while (--i);
}


void Wr_Data(unsigned char wrdata) //写数据 
{  
		RDbf();  
		RS=1;  
		RW=0;  
		E=1;  
		P0=wrdata;  
		E=0;  
}  

void Wr_Data_String(unsigned char wrcommand,unsigned char *str)
{
	Wr_Command(wrcommand,1);
	while(*str != '\0')		
	{
		Wr_Data(*str);			
		str++;		
	}		
}

void Wr_Command(unsigned char wrcommand,busyc) //写指令 
{  
		if(busyc)RDbf();  
		RS=0; 	
		RW=0;  
		E=1;  
		P0=wrcommand;  
		E=0; 
}  

void Lcd12864Clean(void)
{
	Wr_Command(0x01,1);
}

void RDbf(void) //读忙状态 
{ 
		while(1)
		{   
				RS=0;			
				RW=1; 
				E=0;   
				LCMdata=0xFF;   		
				E=1;   
				if((P0&Busy)==0)   break;  
		} 
} 

void Lcm_Init(void)//12864初始化
{     
		REST=1; 
//Delay5ms(); 	
//		REST=0; 
//Delay5ms(); 	
//	Delay5ms(); 
//	Delay5ms(); 
//	Delay5ms(); 
//		REST=1; 
//	Delay5ms(); 
//		Delay5ms();   
		Wr_Command(0x30,0);  
		Delay5ms();  
		Wr_Command(0x30,0); //2 次显示模式设置，不用判忙  
		Delay5ms();  
		Wr_Command(0x0C,1); //开显示及光标设置  
		Delay5ms();  
		Wr_Command(0x01,1); //显示清屏  
		Delay5ms();  
		Delay5ms();  
		Wr_Command(0x06,1); //显示光标移动设置  
		Delay5ms(); 
}
