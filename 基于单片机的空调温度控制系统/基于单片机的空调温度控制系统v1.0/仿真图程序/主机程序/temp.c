#include "temp.h"
//********带返回值的延时子程序***********				
void delay_TEMP(unsigned char time)	
{
	unsigned char n;
	n=0;	 
	while(n<time)n++;
	return;
} 
//********DS18B20复位********* 
void Init_DS18B20(void)
{
	DQ=1;		
	delay_TEMP(8);
	DQ=0;		
	delay_TEMP(85);	
	DQ=1;		
	delay_TEMP(34);	
}
//*******单片机向DS18B20写一个字节******
unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat=0;
	for(i=8;i>0;i--)
	{
		DQ=1;
		delay_TEMP(1);
		DQ=0;
		dat>>=1;
		DQ=1;		
		delay_TEMP(1);
		if(DQ) dat|=0x80;
		delay_TEMP(10);
	}
	return (dat);
}
//******单片机读DS18B20一个字节**********
void WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		DQ=0;
		DQ=dat&0x01;
		delay_TEMP(5);
		DQ=1;
		dat>>=1;
	}
	delay_TEMP(10);
}
//*******单片机读取DS18B20的温度*********
float ReadTemperature(void)
{
	unsigned char tempL=0;
	unsigned char tempH=0;
	float temperature;
	Init_DS18B20();
	WriteOneChar(0xcc);
	WriteOneChar(0x44);
	delay_TEMP(30);
	Init_DS18B20();
	WriteOneChar(0xcc);
	WriteOneChar(0xbe);
	tempL=ReadOneChar();
	tempH=ReadOneChar();
	temperature=((tempH*256)+tempL)*0.0625;
	delay_TEMP(30);
	return(temperature);    //带返回值的程序，返回的值为温度值
}
/*******************************************************************************
* 函 数 名         : datapros()
* 函数功能		   : 温度读取处理转换函数
* 输    入         : temp
* 输    出         : 无
*******************************************************************************/
uint8_t Ds18b20DisplayData[4] = {0};

uint8_t * Ds18b20_datapros(float temp) 	 
{
   	int n = 10 * temp;			                  //浮点型的m变成整型的n
	Ds18b20DisplayData[0] = n/100 + '0';
	Ds18b20DisplayData[1] = n%100/10 + '0';
	Ds18b20DisplayData[2] = '.';
	Ds18b20DisplayData[3] = n%10 + '0';	      //整型的n拆位
	return Ds18b20DisplayData;
}


