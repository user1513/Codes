#ifndef _MAIN_H
#define _MAIN_H

#include "sys.h"

enum 
{
	Disable,
	Enable
};

typedef enum
{
	FALSE,
	TRUE
}bool;

typedef struct datalist
{
	bool Reg;//注冊
	u8 Addr;//从机地址
	struct datalist * pNext;//链表下一个地址指针
	u8 * pArray;//数组地址
}*pDATALIST,DATSLIST;


#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "McuDataSend.h"
#include "_usart_send.h"
#include "usart2.h"
#include "usart3.h"
#include "sortfunc.h"
//#include "stdlib.h"
#include "math.h"
#include "string.h"

#define MAININTERFACE 0x01  //主界面

extern u8 TestKeyVal;//主界面按鍵想應
extern u8 UsartData[];//串口獲取數據
extern bool uart3flag;//串口3接收標志位
extern bool touchflag;    //觸摸標志位
extern bool gohomeflag;   //回主界面標志位
extern u8 Receivebufferarea[];//默認定義40*7的數據存儲區
extern u16 ReceivebufferareaVal;//數據接受個數
extern pDATALIST pHard;    //创建头节点
#endif


