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

#define CMD_START_DS12C887 0x20 		/* 开启时钟芯片 */ 
#define CMD_START_OSCILLATOR 0x70 	/* 开启振荡器， 处于抑制状态 */ 
#define CMD_CLOSE_DS12C887 0x30 		/* 关掉时钟芯片 */


/* 所有的置位使用或操作，清除使用与操作 */ 
#define MASK_SETB_SET 0x80				  /* 禁止刷新 */ 
#define MASK_CLR_SET 0x7f 				  /* 使能刷新 */ 
#define MASK_SETB_DM 0x04 					/* 使用HEX格式 */

#define MASK_CLR_DM 0xfb 						/* 使 用BCD码格式 */ 
#define MASK_SETB_2412 0x02 				/* 使用24小时模式 */ 
#define MASK_CLR_2412 0xfd 					/* 使用12小时模式 */ 
#define MASK_SETB_DSE 0x01 					/* 使用夏令时 */ 
#define MASK_CLR_DSE 0xfe 					/* 不使用夏令时 */ 

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




