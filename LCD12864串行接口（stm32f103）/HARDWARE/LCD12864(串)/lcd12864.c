#include "lcd12864.h"


void lcd12864_IoInit(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStructure;					//定义GPIO初始化结构体GPIO_InitStructure 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);										
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;		//设置相应小灯管脚为推挽输出,最大翻转频率为50MHz | GPIO_Pin_4 | GPIO_Pin_5
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOB, &GPIO_InitStructure);										
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//设置相应小灯管脚为推挽输出,最大翻转频率为50MHz | GPIO_Pin_4 | GPIO_Pin_5
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}

void LCD12864delay(u16 _n) 
{
	unsigned int i;
	for(i=0; i<_n; i++) {;}
}
//串行发送一字节数据
void LCD12864SendByte(u8 _dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		LCD12864_SCLK_L;
		if(_dat&0x80) 
			LCD12864_SID_H;
		else LCD12864_SID_L;
		LCD12864_SCLK_H;
		_dat <<=1;
	}
	delay_ms(1);
}

void LCD12864SendCmd(u8 _dat)
{
//      while(ReadBF){;}
	LCD12864_CS_H;		
	LCD12864SendByte(0xF8);//11111,00,0 RW=0,RS=0   同步标志
	LCD12864SendByte(_dat&0xF0);//高四位
	LCD12864SendByte((_dat&0x0F)<<4);//低四位
	LCD12864_CS_L;
}

//写显示数据或单字节字符
void LCD12864SendDate(u8 _dat)
{
//      while(ReadBF){;}
	LCD12864_CS_H;
	LCD12864SendByte(0xFA);//11111,01,0 RW=0,RS=1
	LCD12864SendByte(_dat&0xF0);//高四位
	LCD12864SendByte((_dat&0x0F)<<4);//低四位
	LCD12864_CS_L;
} 

void Lcd12864Display(u8 x_add,u8 *ptr)
{
	LCD12864SendCmd(x_add);//1xxx,xxxx 设定DDRAM 7位地址xxx,xxxx到地址计数器AC
	while(*ptr != '\0')
	{
		LCD12864SendDate(*ptr);
		++ptr;
	}
}
//初始化 LCM
void Lcd12864init(void)
{
	lcd12864_IoInit();
	LCD12864_PSB_L;
	LCD12864_RST_L;
	delay_ms(100);
	LCD12864_RST_H;
	LCD12864_CS_L;
	LCD12864delay(100);
	LCD12864SendCmd(0x30);//功能设置，一次送8位数据，基本指令集
	LCD12864SendCmd(0x0C);//0000,1100 整体显示，游标off，游标位置off
	LCD12864SendCmd(0x01);//0000,0001 清DDRAM
	LCD12864SendCmd(0x02);//0000,0010 DDRAM地址归位
	LCD12864SendCmd(0x80);//1000,0000 设定DDRAM 7位地址000，0000到地址计数器AC
//      SendCMD(0x04);//点设定，显示字符/光标从左到右移位，DDRAM地址加 一
//      SendCMD(0x0F);//显示设定，开显示，显示光标，当前显示位反白闪动
}

/******************************************************************** 
* 名称 :  LCD12864_pos() 
* 功能 : 设定显示位置 
* 输入 :  x,y 
* 输出 : 无 ***********************************************************************/  

void LCD12864_SetPos(u8 x,u8 y)  
{
	u8 pos;
	if(x == 0)
		x = 0x80;
	else if(x == 1)
		x = 0x90;
	else if(x == 2)
		x = 0x88;
	else if(x == 3)
		x = 0x98; 
	pos = x + y;
	LCD12864SendCmd(pos);//显示地址
}  


void Lcd12864String(u8 x,u8 y,u8 *ptr )
{
	LCD12864_SetPos(x,y);
	while(*ptr != '\0')
	{
		LCD12864SendDate(*ptr);
		++ptr;
	}
}


//#include "lcd12864.h"


//void lcd12864_IoInit(void) 
//{ 
//	GPIO_InitTypeDef GPIO_InitStructure;					//定义GPIO初始化结构体GPIO_InitStructure 
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);										
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14||GPIO_Pin_15;		//设置相应小灯管脚为推挽输出,最大翻转频率为50MHz | GPIO_Pin_4 | GPIO_Pin_5
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);										
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//设置相应小灯管脚为推挽输出,最大翻转频率为50MHz | GPIO_Pin_4 | GPIO_Pin_5
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);
//}

//void LCD12864delay(u16 _n) 
//{
//	unsigned int i;
//	for(i=0; i<_n; i++) {;}
//}
////串行发送一字节数据
//void LCD12864SendByte(u8 _dat)
//{
//	unsigned char i;
//	for(i=0;i<8;i++)
//	{
//		LCD12864_SCLK_L;
//		if(_dat&0x80) 
//			LCD12864_SID_H;
//		else LCD12864_SID_L;
//		LCD12864_SCLK_H;
//		_dat <<=1;
//		delay_ms(1);
//	}
//}

//void LCD12864SendCmd(u8 _dat)
//{
////      while(ReadBF){;}
//	LCD12864_LCD12864_LCD12864_CS_H;		
//	LCD12864SendByte(0xF8);//11111,00,0 RW=0,RS=0   同步标志
//	LCD12864SendByte(_dat&0xF0);//高四位
//	LCD12864SendByte((_dat&0x0F)<<4);//低四位
//	LCD12864_CS_L;
//}

////写显示数据或单字节字符
//void LCD12864SendDate(u8 _dat)
//{
////      while(ReadBF){;}
//	LCD12864_LCD12864_LCD12864_CS_H;
//	LCD12864SendByte(0xFA);//11111,01,0 RW=0,RS=1
//	LCD12864SendByte(_dat&0xF0);//高四位
//	LCD12864SendByte((_dat&0x0F)<<4);//低四位
//	LCD12864_CS_L;
//} 

////初始化 LCM
//void Lcd12864init(void)
//{
//	lcd12864_IoInit();

//	LCD12864_CS_L;
//	 
//	LCD12864delay(100);
//	LCD12864SendCmd(0x30);//功能设置，一次送8位数据，基本指令集
//	LCD12864SendCmd(0x0C);//0000,1100 整体显示，游标off，游标位置off
//	LCD12864SendCmd(0x01);//0000,0001 清DDRAM
//	LCD12864SendCmd(0x02);//0000,0010 DDRAM地址归位
//	LCD12864SendCmd(0x80);//1000,0000 设定DDRAM 7位地址000，0000到地址计数器AC
////      SendCMD(0x04);//点设定，显示字符/光标从左到右移位，DDRAM地址加 一
////      SendCMD(0x0F);//显示设定，开显示，显示光标，当前显示位反白闪动
//}


