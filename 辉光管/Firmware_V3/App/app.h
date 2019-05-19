/**
 * ��������ر�������
 * ���ߣ�hhelib
 * �����£�2016-03-13
 * http://blanboom.org
 */

#ifndef __APP_H
#define __APP_H

// ����ģʽ
// �ֱ�Ϊ����ģʽ���Զ�����ʾģʽ
#define MODE_CLOCK    0
#define MODE_DISPLAY  1

#define POWER    0xA2
#define MENU     0xE2
#define TEST     0x22
#define PLUS     0x02
#define MINUS    0x98
#define RETURN   0xC2
#define PREVIOUS 0xE0
#define NEXT     0x90
#define PLAY     0xA8
#define CLEAR    0xB0
#define ZERO     0x68
#define ONE      0x30
#define TWO      0x18
#define THREE    0x7A
#define FOUR     0x10
#define FIVE     0x38
#define SIX      0x5A
#define SEVEN    0x42
#define EIGHT    0x4A
#define NINE     0x52 

#define uint unsigned int
#define uchar unsigned char

extern uint8_t mode;

// ʱ�䡢���ӡ��Զ�����ʾģʽ��Ҫ��ʾ�����ݣ� �� "main.c" �ж���
extern uint8_t hour;
extern uint8_t minute;
extern uint8_t second;
extern uint8_t per25ms;

extern uint8_t hour_alarm;
extern uint8_t minute_alarm;
extern uint8_t second_alarm;
extern uint8_t alarmEnable;

extern uint8_t sleepMode;

#endif