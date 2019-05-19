#include "HC_SR04.h"


void Delay20us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 52;
	while (--i);
}


void StartModule(void)
{
	flag = 0;
	TRIG=1;
	Delay20us();
	TRIG=0;
}

unsigned int Conut(void)
{
	unsigned int S = 0;
	unsigned int  time = 0; 
	time = T2H * 256 + T2L;
	T2H = 0;
	T2L = 0;
	S = ( time * 1.7 ) / 10;     //算出来是CM
	S = 400.0/380*S;
	if((S >= 7000) || flag == 1) //超出测量范围显示“-”
	{
		S = 0;
	}
	return S;
}
	



