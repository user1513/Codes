#include "Ultrasonic.h"

u8 UltrasonicCheck_Add8(u8 *buf,u8 length)
{
	int i=0;
	u8 sum;
	for(i=0;i<length;i++)
	{
		sum +=*(buf+i);
	}
	return (u8)sum;
}

void UltrasonicDataPage(u16 UltrasonicDistance)
{
	int i=0;
	u8 DataPage[8];
	DataPage[0]=0x5A;
	DataPage[1]=0xA5;
	DataPage[2]=0x15;
	DataPage[3]=0x03;
	DataPage[4]=(u8)(UltrasonicDistance>>8);
	DataPage[5]=(u8)UltrasonicDistance;
	DataPage[6]=0x00;
	DataPage[7]=UltrasonicCheck_Add8(DataPage,8-1);
	UserSendString(DataPage,8);
}

