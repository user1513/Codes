#include "lcd12864.h"

u8 OLED_GRAM[64][16] = {0};


void lcd12864_IoInit(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStructure;					//����GPIO��ʼ���ṹ��GPIO_InitStructure 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);										
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4;		//������ӦС�ƹܽ�Ϊ�������,���תƵ��Ϊ50MHz | GPIO_Pin_4 | GPIO_Pin_5
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);										
}

void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
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
//���з���һ�ֽ�����
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
	LCD12864SendByte(0xF8);//11111,00,0 RW=0,RS=0   ͬ����־
	LCD12864SendByte(_dat&0xF0);//����λ
	LCD12864SendByte((_dat&0x0F)<<4);//����λ
	LCD12864_CS_L;
}


//д��ʾ���ݻ��ֽ��ַ�
void LCD12864SendDate(u8 _dat)
{
//      while(ReadBF){;}
	LCD12864_CS_H;
	LCD12864SendByte(0xFA);//11111,01,0 RW=0,RS=1
	LCD12864SendByte(_dat&0xF0);//����λ
	LCD12864SendByte((_dat&0x0F)<<4);//����λ
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
//��ʼ�� LCM
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
	 LCD12864SendCmd(0x36);//дָ���������ָ�����ͼG=1��
	 LCD12864SendCmd(0x3E);
	 LCD12864SendCmd(0x03);
	#else
	 LCD12864SendCmd(0x30);//�������ã�һ����8λ���ݣ�����ָ�
	#endif
	LCD12864SendCmd(0x0c);       //����ʾ(���αꡢ������)
	LCD12864SendCmd(0x01);       //�����ʾ�������趨��ַָ��Ϊ00H
	LCD12864SendCmd(0x06);       //ָ�������ϵĶ�ȡ��д��ʱ���趨�α���ƶ�����ָ����ʾ����λ
	LCD12864SendCmd(0x80);//1000,0000 �趨DDRAM 7λ��ַ000��0000����ַ������AC

//	  LCD12864SendCmd(0x3E);//8λ���ݴ��䣬��չָ���������ͼ��             
//    LCD12864SendCmd(0x01);//���� 
//    LCD12864SendCmd(0x04);//���趨����ʾ�ַ�/����������λ��DDRAM��ַ�� һ
//    SendCMD(0x0F);//��ʾ�趨������ʾ����ʾ��꣬��ǰ��ʾλ��������
}



void show_Pic(uint8_t *address)//��ʾͼƬ����
{          //address����ָ�������ָ�룬�÷���show_Pic��XY������XYΪ������
   uint8_t i,j;
//	LCD12864Display_OFF();
   for(i=0;i<32;i++)     //�ϰ�����32��������д��
   {
	   
       for(j=0;j<8;j++)//ÿ��128���㣬����һ��Ҫ16����λ16��������Ҳ����8λ��������������ȫ������
       {
          Lcd_PutPixel(j * 16,i,1,*address,*(address+1));
		  address += 2;
       }
   }
   for(i=0;i<32;i++)    //�°�����32�Ų���ԭ����ϰ���һ��
   {
	   for(j=0;j<8;j++)//ÿ��128���㣬����һ��Ҫ16����λ16��������Ҳ����8λ��������������ȫ������
       {
		  Lcd_PutPixel(j * 16 ,i + 32,1,*address,*(address+1));
		  address += 2;
	   }
   }
//   LCD12864Display_ON();
}


/***************************************
��LCDָ������д��һ������,������ɫ�����֣�0����ף�����ʾ����1����ڣ�����ʾ��
****************************************/
void Lcd_PutPixel(unsigned char x,unsigned char y,unsigned char Color,uint8_t data_H,uint8_t data_L)
{
	unsigned char z;
	if(x>=128||y>=64)
		return;
	Color=Color%2;
	x=x/16;//ȷ��Ϊһ���ϵĵڼ���
	if(y<32) //���Ϊ��ҳ
		z=0x80;
	else     //�������Ϊ��ҳ
		z=0x88;
	y=y%32;
	LCD12864SendCmd(0x36);
	LCD12864SendCmd(y+0x80);        //�е�ַ
	LCD12864SendCmd(x+z);     //�е�ַ
   LCD12864SendDate(data_H);//��д���8λ����д���8λ
   LCD12864SendDate(data_L);
	LCD12864SendCmd(0x30);
}

/*****************************************
���Lcdȫ����������ģʽModeΪ0����Ϊȫ�����Ϊ��ɫ0�����κ���ʾ��
����Ϊȫ�����Ϊ��ɫ1(ȫ�������ʾ)
******************************************/
void Lcd_Clear(unsigned char Mode)
{
	unsigned char x,y,ii;
	unsigned char Temp;
	if(Mode%2==0)
		Temp=0x00;
	else
		Temp=0xff;
	LCD12864SendCmd(0x36);//����ָ�� ��ͼ��ʾ
	for(ii=0;ii<9;ii+=8)   
		for(y=0;y<0x20;y++)     
			for(x=0;x<8;x++)
			{ 	
				//EA=0;
				LCD12864SendCmd(y+0x80);        //�е�ַ
				LCD12864SendCmd(x+0x80+ii);     //�е�ַ     
				LCD12864SendDate(Temp); //д���� D15��D8 
				LCD12864SendDate(Temp); //д���� D7��D0 
				//EA=1;
			}
	LCD12864SendCmd(0x30);
}

/******************************************************************** 
* ���� :  LCD12864_pos() 
* ���� : �趨��ʾλ�� 
* ���� :  x,y 
* ��� : �� ***********************************************************************/  

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
	LCD12864SendCmd(pos);//��ʾ��ַ
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
	LCD12864SendCmd(x_add);//1xxx,xxxx �趨DDRAM 7λ��ַxxx,xxxx����ַ������AC
	delay_ms(1);
	while(*ptr != '\0')
	{
		LCD12864SendDate(*ptr);
		++ptr;
	}
	delay_ms(1);
}








