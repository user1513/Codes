#ifndef _AMPIRE12864_H
#define _AMPIRE12864_H

#include "app.h"

#define LcdDataPort P0  //���ݶ˿�


sbit Reset = P2^3;  //��λ
sbit RS = P2^4;  //ָ������ѡ��
sbit E = P2^6;  //ָ�����ݿ���
sbit CS1 = P3^2;  //����Ļѡ�񣬵͵�ƽ��Ч
sbit CS2 = P3^3;  //����Ļѡ��
sbit RW = P2^5;  //��д����
sbit busy = P0^7;  //æ��־

void SetOnOff(uint8_t onoff); //������ʾ
void SelectScreen(uint8_t screen);//ѡ����Ļ
void ClearScreen(uint8_t screen); //����
void Show1616(uint8_t lin,uint8_t colum,uint8_t *address);//��ʾһ������
void CheckState();  //�ж�״̬
void LcdDelay(uint16_t time); //��ʱ
void WriteData(uint8_t dat); //д����
void SendCommand(uint8_t command); //дָ��
void SetLine(uint8_t line);  //���е�ַ
void SetColum(uint8_t colum);//���е�ַ
void SetStartLine(uint8_t startline);//����ʾ��ʼ��
void InitLcd();  //��ʼ��
void ResetLcd();  //��λ
void Show1608(uint8_t lin,uint8_t column,uint8_t *address);
void LCD_ShowChinaLanguage(uint8_t lin,uint8_t column,uint8_t *p, uint8_t Language_num);
void SetClear(void);
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63			 
void LCD_ShowChar(uint8_t lin,uint8_t column,uint8_t chr);
//m^n����
unsigned long LCD_pow(uint8_t m,uint8_t n);
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//num:��ֵ(0~4294967295);	 		  
void LCD_ShowNumber(uint8_t lin,uint8_t column,unsigned long num,uint8_t len);
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void LED_ShowString(uint8_t lin,uint8_t column,const uint8_t *p);
void LED_ShowString_length(uint8_t lin,uint8_t column, uint8_t *p, uint8_t length);

#endif