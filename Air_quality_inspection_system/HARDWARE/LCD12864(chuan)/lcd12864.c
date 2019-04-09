#include "lcd12864.h"

u8 OLED_GRAM[64][16] = {0};


void lcd12864_IoInit(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStructure;					//定义GPIO初始化结构体GPIO_InitStructure 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);										
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4;		//设置相应小灯管脚为推挽输出,最大翻转频率为50MHz | GPIO_Pin_4 | GPIO_Pin_5
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);										
}

void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=x/8;
	bx=x%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[y][pos]|=temp;
	else OLED_GRAM[y][pos]&=~temp;	    
}

void LCD12864DisplayMode(uint8_t mode)
{
	if(mode ==0)
		LCD12864SendCmd(0x08);
	else
		LCD12864SendCmd(0x0C);
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
	delay_us(10);
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


void Lcd12864Clean_mode(u8 mode)
{
	static u8 LastMode=0;
	if(LastMode != mode)
		LCD12864SendCmd(0x01);
	LastMode = mode;
}

void Lcd12864Clean(void)
{
	LCD12864SendCmd(0x01);
}
//初始化 LCM
void Lcd12864init(void)
{
	lcd12864_IoInit();
	//LCD12864_PSB_L;
	LCD12864_RST_L;
	delay_ms(100);
	LCD12864_RST_H;
	LCD12864_CS_L;
	delay_ms(10);
	#if ISPAINT_ENABLE
	 LCD12864SendCmd(0x36);//写指令函数，扩充指令集，绘图G=1打开
	 LCD12864SendCmd(0x3E);
	 LCD12864SendCmd(0x03);
	#else
	 LCD12864SendCmd(0x30);//功能设置，一次送8位数据，基本指令集
	#endif
	LCD12864SendCmd(0x0c);       //开显示(无游标、不反白)
	LCD12864SendCmd(0x01);       //清除显示，并且设定地址指针为00H
	LCD12864SendCmd(0x06);       //指定在资料的读取及写入时，设定游标的移动方向及指定显示的移位
	LCD12864SendCmd(0x80);//1000,0000 设定DDRAM 7位地址000，0000到地址计数器AC

//	  LCD12864SendCmd(0x3E);//8位数据传输，扩展指令操作，绘图开             
//    LCD12864SendCmd(0x01);//清屏 
//    LCD12864SendCmd(0x04);//点设定，显示字符/光标从左到右移位，DDRAM地址加 一
//    SendCMD(0x0F);//显示设定，开显示，显示光标，当前显示位反白闪动
}



void show_Pic(uint8_t *address)//显示图片函数
{          //address是是指向数组的指针，用法：show_Pic（XY）当中XY为数组名
   uint8_t i,j;
//	LCD12864Display_OFF();
   for(i=0;i<32;i++)     //上半屏的32排依次先写满
   {
	   
       for(j=0;j<8;j++)//每排128个点，所以一共要16个两位16进制数（也就是8位二进制数）才能全部控制
       {
          Lcd_PutPixel(j * 16,i,1,*address,*(address+1));
		  address += 2;
       }
   }
   for(i=0;i<32;i++)    //下半屏的32排操作原理和上半屏一样
   {
	   for(j=0;j<8;j++)//每排128个点，所以一共要16个两位16进制数（也就是8位二进制数）才能全部控制
       {
		  Lcd_PutPixel(j * 16 ,i + 32,1,*address,*(address+1));
		  address += 2;
	   }
   }
//   LCD12864Display_ON();
}


/***************************************
向LCD指定坐标写入一个象素,象素颜色有两种，0代表白（无显示），1代表黑（有显示）
****************************************/
void Lcd_PutPixel(unsigned char x,unsigned char y,unsigned char Color,uint8_t data_H,uint8_t data_L)
{
	unsigned char z;
	if(x>=128||y>=64)
		return;
	Color=Color%2;
	x=x/16;//确定为一行上的第几字
	if(y<32) //如果为上页
		z=0x80;
	else     //否则如果为下页
		z=0x88;
	y=y%32;
	LCD12864SendCmd(0x36);
	LCD12864SendCmd(y+0x80);        //行地址
	LCD12864SendCmd(x+z);     //列地址
   LCD12864SendDate(data_H);//先写入高8位，再写入低8位
   LCD12864SendDate(data_L);
	LCD12864SendCmd(0x30);
}

/*****************************************
清除Lcd全屏，如果清除模式Mode为0，则为全屏清除为颜色0（无任何显示）
否则为全屏清除为颜色1(全屏填充显示)
******************************************/
void Lcd_Clear(unsigned char Mode)
{
	unsigned char x,y,ii;
	unsigned char Temp;
	if(Mode%2==0)
		Temp=0x00;
	else
		Temp=0xff;
	LCD12864SendCmd(0x36);//扩充指令 绘图显示
	for(ii=0;ii<9;ii+=8)   
		for(y=0;y<0x20;y++)     
			for(x=0;x<8;x++)
			{ 	
				//EA=0;
				LCD12864SendCmd(y+0x80);        //行地址
				LCD12864SendCmd(x+0x80+ii);     //列地址     
				LCD12864SendDate(Temp); //写数据 D15－D8 
				LCD12864SendDate(Temp); //写数据 D7－D0 
				//EA=1;
			}
	LCD12864SendCmd(0x30);
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


void Lcd12864Display(u8 x_add,u8 *ptr)
{
	LCD12864SendCmd(0x30);
	delay_ms(1);
	LCD12864SendCmd(x_add);//1xxx,xxxx 设定DDRAM 7位地址xxx,xxxx到地址计数器AC
	delay_ms(1);
	while(*ptr != '\0')
	{
		LCD12864SendDate(*ptr);
		++ptr;
	}
	delay_ms(1);
}








