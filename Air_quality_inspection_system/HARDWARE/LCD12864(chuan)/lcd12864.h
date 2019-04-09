#ifndef _LCD_12846_H_
#define _LCD_12864_H_

#include "main.h"

#define ISPAINT_ENABLE 0 //1使能画图模式，0不使能画图模式

extern u8 OLED_GRAM[][16];


#define LCD12864_SID_IN()  {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=(u32)8<<24;}
#define LCD12864_SID_OUT() {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=(u32)3<<24;}

/*LCD 12864*/
#define LCD12864_CS_H GPIO_SetBits(GPIOA,GPIO_Pin_5) //RS=1   4PIN
#define LCD12864_CS_L GPIO_ResetBits(GPIOA,GPIO_Pin_5) //RS=0

#define LCD12864_SID_H GPIO_SetBits(GPIOA,GPIO_Pin_6) //RW=1    5PIN
#define LCD12864_SID_L GPIO_ResetBits(GPIOA,GPIO_Pin_6);  //RW = 0

#define LCD12864_SCLK_H GPIO_SetBits(GPIOA,GPIO_Pin_7);    //EN = 1   6PIN
#define LCD12864_SCLK_L GPIO_ResetBits(GPIOA,GPIO_Pin_7);  //EN = 0

#define LCD12864_RST_H GPIO_SetBits(GPIOA,GPIO_Pin_4);   
#define LCD12864_RST_L GPIO_ResetBits(GPIOA,GPIO_Pin_4); 

//#define LCD12864_PSB_H GPIO_SetBits(GPIOA,GPIO_Pin_4);    
//#define LCD12864_PSB_L GPIO_ResetBits(GPIOA,GPIO_Pin_4); 




#define LCD12864Display_ON() 	LCD12864DisplayMode(1)
#define LCD12864Display_OFF() 	LCD12864DisplayMode(0)



void OLED_DrawPoint(u8 x,u8 y,u8 t);
void LCD12864SendByte(u8 _dat);
void LCD12864delay(u16 _n) ;
void LCD12864SendCmd(u8 _dat);
void LCD12864SendDate(u8 _dat);
void Lcd12864Display(u8 x_add,u8 *ptr);
void Lcd12864init(void);
void Lcd12864_IoInit(void);
void Lcd12864Clean_mode(u8 mode);
void Lcd12864Clean(void);
void Lcd12864Decimal(u8 x,u8 y,double _data);
void LCD12864_SetPos(u8 x,u8 y);  
void Lcd12864String(u8 x,u8 y,u8 *ptr );
void show_Pic(uint8_t *address);//显示图片函数
void LCD12864DisplayMode(uint8_t mode);
void Lcd_Clear(unsigned char Mode);
void Lcd_PutPixel(unsigned char x,unsigned char y,unsigned char Color,uint8_t data_H,uint8_t data_L);

#endif 


