#ifndef _DS12C887_H
#define _DS12C887_H

#include <REGX51.H>


#define uchar unsigned char 
#define uint unsigned int 
sbit dscs=P2^4;
sbit dsas=P2^5;
sbit dsrw=P2^6;
sbit dsds=P2^7;
sbit dsirq=P3^2;

#define CMD_START_DS12C887 0x20 		/* ����ʱ��оƬ */ 
#define CMD_START_OSCILLATOR 0x70 	/* ���������� ��������״̬ */ 
#define CMD_CLOSE_DS12C887 0x30 		/* �ص�ʱ��оƬ */


/* ���е���λʹ�û���������ʹ������� */ 
#define MASK_SETB_SET 0x80				  /* ��ֹˢ�� */ 
#define MASK_CLR_SET 0x7f 				  /* ʹ��ˢ�� */ 
#define MASK_SETB_DM 0x04 					/* ʹ��HEX��ʽ */

#define MASK_CLR_DM 0xfb 						/* ʹ ��BCD���ʽ */ 
#define MASK_SETB_2412 0x02 				/* ʹ��24Сʱģʽ */ 
#define MASK_CLR_2412 0xfd 					/* ʹ��12Сʱģʽ */ 
#define MASK_SETB_DSE 0x01 					/* ʹ������ʱ */ 
#define MASK_CLR_DSE 0xfe 					/* ��ʹ������ʱ */ 

void StartDs12c887(void);
void CloseDs12c887(void);
void InitDs12c887(void);
void write_ds12c887(uchar add,uchar date);
uchar read_ds12c887(uchar add);
void set_alarm(uchar ashi,uchar afen,uchar amiao);
void SetTime(uchar shi,uchar fen,uchar miao);
void SetDate(uchar year,uchar month,uchar date);
void SetDay(uchar day);
#endif




