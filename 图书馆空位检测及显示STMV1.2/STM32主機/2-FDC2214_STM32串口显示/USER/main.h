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
	bool Reg;//ע��
	u8 Addr;//�ӻ���ַ
	struct datalist * pNext;//������һ����ַָ��
	u8 * pArray;//�����ַ
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

#define MAININTERFACE 0x01  //������

extern u8 TestKeyVal;//�����水�I�둪
extern u8 UsartData[];//���ګ@ȡ����
extern bool uart3flag;//����3���՘�־λ
extern bool touchflag;    //�|����־λ
extern bool gohomeflag;   //���������־λ
extern u8 Receivebufferarea[];//Ĭ�J���x40*7�Ĕ����惦�^
extern u16 ReceivebufferareaVal;//�������܂���
extern pDATALIST pHard;    //����ͷ�ڵ�
#endif


