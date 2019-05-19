#ifndef _PBDATA_H_
#define _PBDATA_H_




typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

//UartCmdArray[3];读/写
#define COMREADCMD 0X00    //上位机读指令
#define COMWRITECMD 0X01   //上位机写指令
//UartCmdArray[4];命令
#define COMWORD1 0X01      //设置超声波报警距离
#define COMWORD2 0X02      //设置超声波报警是否使能
#define COMWORD3 0X04      //设置超声波串口数据是否发送
#define COMWORD4 0X08      //设置语音报警音量
#define COMWORD5 0X10      //待定
#define COMWORD6 0X20      //待定
#define COMWORD7 0X40      //待定
#define COMWORD8 0X80      //待定
//UartCmdArray[7];是否写入E2prom
#define E2PROMDISABLE 0X00 //不写入EE2PROM
#define E2PROMENABLE 0X01  //写入E2PROM

///////////////////////////////////////////////////////////
#define E2PROMFLAGADDR    0X0000//E2PROM写入数据后默认设置位0X01，如果不是0X01设备工作前清空数据后将默认数据写入
#define H_ALARMRANGEADDR  0X0001//报警距离高八位
#define L_ALARMRANGEADDR  0X0002//报警距离低8位
#define ALARMENABLEADDR   0X0003//设置超声波报警是否使能
#define UARTENABLEADDR    0X0004//设置超声波串口数据是否发送
#define VOLUMEADDR        0X0005//设置语音报警音量
//#define E2PROMFLAGADDR    0X0006//待定
//#define E2PROMFLAGADDR    0X0007//待定
//#define E2PROMFLAGADDR    0X0008//待定

#define ERROR
typedef enum
{
   Disable=0,
    Enable=1
}myenum;


typedef enum
{
	FALSE,
	TRUE
}_bool;



#include <STC8.H>
#include "timer0.h"
#include "uart1.h"
#include "uart2.h"
#include "UltrasonicDataHandle.h"
#include "intrins.h"
#include "ComputerAck.h"
#include "E2PROM.h"
#include "MY1680U.h"
#include "stddef.h"
#include "string.h"
#include "stdio.h"
#include "mydemo.h"
void Delayus(u32 _ustime);//延时us;
void Delayms(u32 _mstime);//延时ms;




#endif
