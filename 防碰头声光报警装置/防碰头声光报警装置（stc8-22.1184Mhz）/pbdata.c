#include "pbdata.h"


void Delayus(u32 _ustime)		//@22.1184MHz
{
	unsigned char i;
    while(_ustime--)
    {
    	_nop_();
    	i = 5;
    	while (--i);
    }
}

void Delayms(u32 _mstime)		//@22.1184MHz
{
	unsigned char i, j;
    while(_mstime--)
    {
    	i = 29;
    	j = 183;
    	do
    	{
    		while (--j);
    	} while (--i);
    }
}






