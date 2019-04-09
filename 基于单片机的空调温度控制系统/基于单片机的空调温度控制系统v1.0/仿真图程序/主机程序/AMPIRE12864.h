#ifndef _AMPIRE12864_H
#define _AMPIRE12864_H

#include "app.h"

#define LcdDataPort P0  //数据端口


sbit Reset = P2^3;  //复位
sbit RS = P2^4;  //指令数据选择
sbit E = P2^6;  //指令数据控制
sbit CS1 = P3^2;  //左屏幕选择，低电平有效
sbit CS2 = P3^3;  //右屏幕选择
sbit RW = P2^5;  //读写控制
sbit busy = P0^7;  //忙标志

void SetOnOff(uint8_t onoff); //开关显示
void SelectScreen(uint8_t screen);//选择屏幕
void ClearScreen(uint8_t screen); //清屏
void Show1616(uint8_t lin,uint8_t colum,uint8_t *address);//显示一个汉字
void CheckState();  //判断状态
void LcdDelay(uint16_t time); //延时
void WriteData(uint8_t dat); //写数据
void SendCommand(uint8_t command); //写指令
void SetLine(uint8_t line);  //置行地址
void SetColum(uint8_t colum);//置列地址
void SetStartLine(uint8_t startline);//置显示起始行
void InitLcd();  //初始化
void ResetLcd();  //复位
void Show1608(uint8_t lin,uint8_t column,uint8_t *address);
void LCD_ShowChinaLanguage(uint8_t lin,uint8_t column,uint8_t *p, uint8_t Language_num);
void SetClear(void);
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63			 
void LCD_ShowChar(uint8_t lin,uint8_t column,uint8_t chr);
//m^n函数
unsigned long LCD_pow(uint8_t m,uint8_t n);
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//num:数值(0~4294967295);	 		  
void LCD_ShowNumber(uint8_t lin,uint8_t column,unsigned long num,uint8_t len);
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowString(uint8_t lin,uint8_t column,const uint8_t *p);
void LCD_ShowString_length(uint8_t lin,uint8_t column, uint8_t *p, uint8_t length);

#endif