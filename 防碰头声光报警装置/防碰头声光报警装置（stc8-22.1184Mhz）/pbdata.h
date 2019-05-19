#ifndef _PBDATA_H_
#define _PBDATA_H_




typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

//UartCmdArray[3];��/д
#define COMREADCMD 0X00    //��λ����ָ��
#define COMWRITECMD 0X01   //��λ��дָ��
//UartCmdArray[4];����
#define COMWORD1 0X01      //���ó�������������
#define COMWORD2 0X02      //���ó����������Ƿ�ʹ��
#define COMWORD3 0X04      //���ó��������������Ƿ���
#define COMWORD4 0X08      //����������������
#define COMWORD5 0X10      //����
#define COMWORD6 0X20      //����
#define COMWORD7 0X40      //����
#define COMWORD8 0X80      //����
//UartCmdArray[7];�Ƿ�д��E2prom
#define E2PROMDISABLE 0X00 //��д��EE2PROM
#define E2PROMENABLE 0X01  //д��E2PROM

///////////////////////////////////////////////////////////
#define E2PROMFLAGADDR    0X0000//E2PROMд�����ݺ�Ĭ������λ0X01���������0X01�豸����ǰ������ݺ�Ĭ������д��
#define H_ALARMRANGEADDR  0X0001//��������߰�λ
#define L_ALARMRANGEADDR  0X0002//���������8λ
#define ALARMENABLEADDR   0X0003//���ó����������Ƿ�ʹ��
#define UARTENABLEADDR    0X0004//���ó��������������Ƿ���
#define VOLUMEADDR        0X0005//����������������
//#define E2PROMFLAGADDR    0X0006//����
//#define E2PROMFLAGADDR    0X0007//����
//#define E2PROMFLAGADDR    0X0008//����

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
void Delayus(u32 _ustime);//��ʱus;
void Delayms(u32 _mstime);//��ʱms;




#endif
