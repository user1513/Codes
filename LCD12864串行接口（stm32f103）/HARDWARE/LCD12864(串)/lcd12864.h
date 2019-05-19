#ifndef _LCD_12846_H_
#define _LCD_12864_H_

#include "sys.h"
#include "delay.h"



///*LCD 12864*/
//#define RS_H GPIO_SetBits(GPIOB,GPIO_Pin_12) //RS=1   4PIN
//#define RS_L GPIO_ResetBits(GPIOB,GPIO_Pin_12) //RS=0

//#define RW_H GPIO_SetBits(GPIOB,GPIO_Pin_13) //RW=1    5PIN
//#define RW_L GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //RW = 0

//#define EN_H GPIO_SetBits(GPIOB,GPIO_Pin_14);    //EN = 1   6PIN
//#define EN_L GPIO_ResetBits(GPIOB,GPIO_Pin_14);  //EN = 0


/*LCD 12864*/
#define LCD12864_CS_H GPIO_SetBits(GPIOB,GPIO_Pin_12) //RS=1   4PIN
#define LCD12864_CS_L GPIO_ResetBits(GPIOB,GPIO_Pin_12) //RS=0

#define LCD12864_SID_H GPIO_SetBits(GPIOB,GPIO_Pin_13) //RW=1    5PIN
#define LCD12864_SID_L GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //RW = 0

#define LCD12864_SCLK_H GPIO_SetBits(GPIOB,GPIO_Pin_14);    //EN = 1   6PIN
#define LCD12864_SCLK_L GPIO_ResetBits(GPIOB,GPIO_Pin_14);  //EN = 0

#define LCD12864_RST_H GPIO_SetBits(GPIOB,GPIO_Pin_15);    //EN = 1   6PIN
#define LCD12864_RST_L GPIO_ResetBits(GPIOB,GPIO_Pin_15);  //EN = 0

#define LCD12864_PSB_H GPIO_SetBits(GPIOA,GPIO_Pin_8);    //EN = 1   6PIN
#define LCD12864_PSB_L GPIO_ResetBits(GPIOA,GPIO_Pin_8);  //EN = 0

void LCD12864SendByte(u8 _dat);
void LCD12864delay(u16 _n) ;
void LCD12864SendCmd(u8 _dat);
void LCD12864SendDate(u8 _dat);
void Lcd12864Display(u8 x_add,u8 *ptr);
void Lcd12864init(void);
void Lcd12864_IoInit(void);
#endif 



//#ifndef _LCD_12846_H_
//#define _LCD_12864_H_

//#include "sys.h"
//#include "delay.h"






//void LCD12864SendByte(u8 _dat);
//void LCD12864delay(u16 _n) ;
//void LCD12864SendCmd(u8 _dat);
//void LCD12864SendDate(u8 _dat);
//void Lcd12864Display(u8 x_add,u8 *ptr);
//void Lcd12864init(void);
//void Lcd12864_IoInit(void);
//void Lcd12864String(u8 x,u8 y,u8 *ptr );
//void LCD12864_SetPos(u8 x,u8 y)  ;
//#endif 



