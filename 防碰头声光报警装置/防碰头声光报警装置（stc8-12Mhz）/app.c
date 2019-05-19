#include "app.h"



void main()
{
	Timer0Init();
	Uart1_Init();
	Uart2Init();
	while(1)
	{

		if(UltrasonicArray[8]==0XFF)
		{
			UltrasonicArray[8]=0X00;
			//Uart1SendStrLen(UltrasonicArray,9);
		}
	}
}
	
