#include "ComputerAck.h"


void ReplyAck(u8 _CmdVal)//�յ���λ��д������󷵻�ACK
{
	xdata u8 _Dataable[7];
	u16 _ModbusCrc=0;
	_Dataable[0]=0XA5;
	_Dataable[1]=0X5A;
	_Dataable[2]=0X04;
	_Dataable[3]=_CmdVal;
	_Dataable[4]=0XFF;
	_ModbusCrc=ModBus_CRC(_Dataable,5);
	_Dataable[5]=(u8)_ModbusCrc;
	_Dataable[6]=_ModbusCrc>>8;
	Uart1SendStrLen(_Dataable,7);//����7λACK
}

void ReplyReadCmd(u8 _CmdVal,u16 _Data)//�յ���λ������������
{
	xdata u8 _Dataable[9];
	u16 _ModbusCrc=0;
	_Dataable[0]=0XA5;
	_Dataable[1]=0X5A;
	_Dataable[2]=0X06;
	_Dataable[3]=_CmdVal;
	_Dataable[6]=0XFF;
	_Dataable[4]=_Data>>8;
	_Dataable[5]=(u8)_Data;
	_ModbusCrc=ModBus_CRC(_Dataable,7);
	_Dataable[7]=(u8)_ModbusCrc;
	_Dataable[8]=_ModbusCrc>>8;
	Uart1SendStrLen(_Dataable,9);//����9λ����
}
void ReplyError(u16 _errortype)
{
	ReplyReadCmd(0x80,_errortype);
}



