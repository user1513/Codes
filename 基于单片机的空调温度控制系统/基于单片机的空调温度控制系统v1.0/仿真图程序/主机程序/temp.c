#include "temp.h"
//********������ֵ����ʱ�ӳ���***********				
void delay_TEMP(unsigned char time)	
{
	unsigned char n;
	n=0;	 
	while(n<time)n++;
	return;
} 
//********DS18B20��λ********* 
void Init_DS18B20(void)
{
	DQ=1;		
	delay_TEMP(8);
	DQ=0;		
	delay_TEMP(85);	
	DQ=1;		
	delay_TEMP(34);	
}
//*******��Ƭ����DS18B20дһ���ֽ�******
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
//******��Ƭ����DS18B20һ���ֽ�**********
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
//*******��Ƭ����ȡDS18B20���¶�*********
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
	return(temperature);    //������ֵ�ĳ��򣬷��ص�ֵΪ�¶�ֵ
}
/*******************************************************************************
* �� �� ��         : datapros()
* ��������		   : �¶ȶ�ȡ����ת������
* ��    ��         : temp
* ��    ��         : ��
*******************************************************************************/
uint8_t Ds18b20DisplayData[4] = {0};

uint8_t * Ds18b20_datapros(float temp) 	 
{
   	int n = 10 * temp;			                  //�����͵�m������͵�n
	Ds18b20DisplayData[0] = n/100 + '0';
	Ds18b20DisplayData[1] = n%100/10 + '0';
	Ds18b20DisplayData[2] = '.';
	Ds18b20DisplayData[3] = n%10 + '0';	      //���͵�n��λ
	return Ds18b20DisplayData;
}


