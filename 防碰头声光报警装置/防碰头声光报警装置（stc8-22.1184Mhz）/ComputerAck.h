#ifndef _COMPUTERACK_H
#define _COMPUTERACK_H

#include "pbdata.h"




void ReplyAck(u8 _CmdVal);//�յ���λ��д������󷵻�ACK

void ReplyReadCmd(u8 _Cmdval,u16 _Data);//�յ���λ������������

void ReplyError(u16 _errortype);//����error type





#endif