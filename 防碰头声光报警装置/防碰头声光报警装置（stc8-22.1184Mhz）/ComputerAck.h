#ifndef _COMPUTERACK_H
#define _COMPUTERACK_H

#include "pbdata.h"




void ReplyAck(u8 _CmdVal);//收到上位机写的命令后返回ACK

void ReplyReadCmd(u8 _Cmdval,u16 _Data);//收到上位机读命令后操作

void ReplyError(u16 _errortype);//返回error type





#endif