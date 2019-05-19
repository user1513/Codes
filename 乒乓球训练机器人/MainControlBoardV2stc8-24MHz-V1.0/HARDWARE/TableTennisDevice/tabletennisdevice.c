#include "tabletennisdevice.h"

//FF090100FF·¢ÅÚ
void Artillery_Cmd(void)
{
	u8 _able[5];
	_able[0]=0XFF;
	_able[1]=0X09;
	_able[2]=0X01;
	_able[3]=0X00;
	_able[4]=0XFF;
	Uart3SendStrLen(_able,5);
}
//FF0107xxFF¿ØÖÆ¶æ»ú
void SteeringEngineAngle(u8 _angle)
{
	u8 _able[5];
	_able[0]=0XFF;
	_able[1]=0X01;
	_able[2]=0X07;
	_able[3]=_angle;
	_able[4]=0XFF;
	Uart3SendStrLen(_able,5);
}