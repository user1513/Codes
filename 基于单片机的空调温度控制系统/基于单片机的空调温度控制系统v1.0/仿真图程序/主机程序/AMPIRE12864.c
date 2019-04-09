#include "AMPIRE12864.h"
#include "ascllfont.h"


void CheckState()
{
	 E = 1;
	 RS = 0;
	 RW = 1;
	 LcdDataPort = 0xff;
	 while(!busy);
}

void LcdDelay(unsigned int time)
{
	while(time --);
}

void WriteData(uint8_t dat)
{
 CheckState();
 E = 1;
 RS = 1;
 RW = 0;
 LcdDataPort = dat;
 E = 0;
}
void SendCommand(uint8_t command)
{
 CheckState();
 E = 1;
 RW = 0;
 RS = 0;
 LcdDataPort = command;
 E = 0;
}

void SelectScreen(uint8_t screen) //0-全屏，1—左屏，2-右屏
{
 switch(screen)
 {
	 case 0 :
	 CS1 = 0;
	 LcdDelay(2);
	 CS2 = 1;
	 LcdDelay(2);
	 break;
	 case 1 :
	 CS1 = 1;
	 LcdDelay(2);
	 CS2 = 0;
	 LcdDelay(2);
	 break;
	 case 2 :
	 CS1 = 0;
	 LcdDelay(2);
	 CS2 = 0;
	 LcdDelay(2);
	 break;
 }
}

void ClearScreen(uint8_t screen)  // screen 0-全屏，1—左屏，2-右屏
{
 uint8_t i,j;
 SelectScreen(screen);
 for(i = 0;i < 8;i ++)
 {
	 SetLine(i);
	 SetColum(0);
	 for(j = 0;j < 64; j ++)
	 WriteData(0);
 }
}

void SetLine(uint8_t line)  //line -> 0 : 7
{
	 line = line & 0x07;
	 line = line | 0xb8; //1011 1xxx
	 SendCommand(line);
}

void SetColum(uint8_t column) //column -> 0 :63
{
	 column = column & 0x3f;
	 column = column | 0x40;  //01xx xxxx
	 SendCommand(column);
}

void SetStartLine(uint8_t startline) //startline -> 0 : 63
{
	 startline = startline & 0x3f;
	 startline = startline | 0xc0; //11xxxxxx
	 SendCommand(startline);
}

void SetOnOff(uint8_t onoff) //1-开显示 0-关
{
	 if(onoff == 1)
	 SendCommand(0x3f);  //0011 111x
	 else
	 SendCommand(0x3e);
}

void SetClear(void)
{
	LCD_ShowString(0, 0, "                ");
	LCD_ShowString(2, 0, "                ");
	LCD_ShowString(4, 0, "                ");
	LCD_ShowString(6, 0, "                ");
}
void ResetLcd()
{
	 Reset = 0;
	 LcdDelay(2);
	 Reset = 1;
	 LcdDelay(2);
	 RS0 = 0;
	 LcdDelay(2);
	 RS1 = 0;
	 LcdDelay(2);
	 SetOnOff(1);
}

void InitLcd()
{
	 ResetLcd();
	SendCommand(0x30);
	 SetOnOff(0);
	 ClearScreen(2);
	 SetLine(0);
	 SetColum(0);
	 SetStartLine(0);
	 SetOnOff(1);
}

void Show1616(uint8_t lin,uint8_t column,uint8_t *address)
{
 uint8_t i;
	if(column < 64)
	{
		SelectScreen(0);
	}
	else
	{
		SelectScreen(1);
	}
	 SetLine(lin);
	 SetColum(column);
	 for(i = 0;i < 16;i ++)
	 WriteData(*(address ++));
	 SetLine(lin + 1);
	 SetColum(column);
	 for(i = 0;i < 16;i ++)
	 WriteData(*(address ++));
}

void Show1608(uint8_t lin,uint8_t column,uint8_t *address)
{
	 uint8_t i;
	 SetLine(lin);
	 SetColum(column);
	 for(i = 0;i < 8;i ++)
	 WriteData(*(address ++));
	 SetLine(lin + 1);
	 SetColum(column);
	 for(i = 0;i < 8;i ++)
	 WriteData(*(address ++));
}

void LCD_ShowChinaLanguage(uint8_t lin,uint8_t column,uint8_t *p, uint8_t Language_num)
{
	uint8_t i = 0;
	column *= 16;
	while(Language_num > i ++)
	{
		Show1616(lin, column, p);
		 p += 32;
		column += 16;
	}
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63			 
void LCD_ShowChar(uint8_t lin,uint8_t column,uint8_t chr)
{ 
	if(column * 8 < 64)
	{
		SelectScreen(0);
	}
	else
	{
		SelectScreen(1);
	}
	chr = chr - ' ';//得到偏移后的值				   
  Show1608(lin, column * 8, &led_asc2_1608[chr][0]);
}
//m^n函数
unsigned long LCD_pow(uint8_t m,uint8_t n)
{
	unsigned long result=1;	 
	while(n--)result*=m;    
	return result;
}				

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//num:数值(0~4294967295);	 		  
void LCD_ShowNumber(uint8_t lin,uint8_t column,unsigned long num,uint8_t len)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t = 0; t < len; t++)
	{
		temp=(num/LCD_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(lin, column, ' ');
				column += 1;
				continue;
			}else enshow=1; 
		}
	 	LCD_ShowChar(lin, column, temp + '0');
		column += 1;
	}
} 

//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowString(uint8_t lin,uint8_t column,const uint8_t *p)
{      
    while(*p!='\0')
    {       
      LCD_ShowChar(lin, column, *p); 
      column++;
			p++;
    }  
}

void LCD_ShowString_length(uint8_t lin,uint8_t column, uint8_t *p, uint8_t length)
{      
    while(length-- > 0)
    {       
      LCD_ShowChar(lin, column, *p); 
      column++;
			p++;
    }  
}
