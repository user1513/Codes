#include "lcd12864.h"


void lcd12864_IoInit(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStructure;					//����GPIO��ʼ���ṹ��GPIO_InitStructure 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);										
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;		//������ӦС�ƹܽ�Ϊ�������,���תƵ��Ϊ50MHz | GPIO_Pin_4 | GPIO_Pin_5
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOB, &GPIO_InitStructure);										
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//������ӦС�ƹܽ�Ϊ�������,���תƵ��Ϊ50MHz | GPIO_Pin_4 | GPIO_Pin_5
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
	delay_ms(1);
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

void Lcd12864Display(u8 x_add,u8 *ptr)
{
	LCD12864SendCmd(x_add);//1xxx,xxxx �趨DDRAM 7λ��ַxxx,xxxx����ַ������AC
	while(*ptr != '\0')
	{
		LCD12864SendDate(*ptr);
		++ptr;
	}
}
//��ʼ�� LCM
void Lcd12864init(void)
{
	lcd12864_IoInit();
	LCD12864_PSB_L;
	LCD12864_RST_L;
	delay_ms(100);
	LCD12864_RST_H;
	LCD12864_CS_L;
	LCD12864delay(100);
	LCD12864SendCmd(0x30);//�������ã�һ����8λ���ݣ�����ָ�
	LCD12864SendCmd(0x0C);//0000,1100 ������ʾ���α�off���α�λ��off
	LCD12864SendCmd(0x01);//0000,0001 ��DDRAM
	LCD12864SendCmd(0x02);//0000,0010 DDRAM��ַ��λ
	LCD12864SendCmd(0x80);//1000,0000 �趨DDRAM 7λ��ַ000��0000����ַ������AC
//      SendCMD(0x04);//���趨����ʾ�ַ�/����������λ��DDRAM��ַ�� һ
//      SendCMD(0x0F);//��ʾ�趨������ʾ����ʾ��꣬��ǰ��ʾλ��������
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


//#include "lcd12864.h"


//void lcd12864_IoInit(void) 
//{ 
//	GPIO_InitTypeDef GPIO_InitStructure;					//����GPIO��ʼ���ṹ��GPIO_InitStructure 
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);										
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14||GPIO_Pin_15;		//������ӦС�ƹܽ�Ϊ�������,���תƵ��Ϊ50MHz | GPIO_Pin_4 | GPIO_Pin_5
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);										
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//������ӦС�ƹܽ�Ϊ�������,���תƵ��Ϊ50MHz | GPIO_Pin_4 | GPIO_Pin_5
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);
//}

//void LCD12864delay(u16 _n) 
//{
//	unsigned int i;
//	for(i=0; i<_n; i++) {;}
//}
////���з���һ�ֽ�����
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
//	LCD12864SendByte(0xF8);//11111,00,0 RW=0,RS=0   ͬ����־
//	LCD12864SendByte(_dat&0xF0);//����λ
//	LCD12864SendByte((_dat&0x0F)<<4);//����λ
//	LCD12864_CS_L;
//}

////д��ʾ���ݻ��ֽ��ַ�
//void LCD12864SendDate(u8 _dat)
//{
////      while(ReadBF){;}
//	LCD12864_LCD12864_LCD12864_CS_H;
//	LCD12864SendByte(0xFA);//11111,01,0 RW=0,RS=1
//	LCD12864SendByte(_dat&0xF0);//����λ
//	LCD12864SendByte((_dat&0x0F)<<4);//����λ
//	LCD12864_CS_L;
//} 

////��ʼ�� LCM
//void Lcd12864init(void)
//{
//	lcd12864_IoInit();

//	LCD12864_CS_L;
//	 
//	LCD12864delay(100);
//	LCD12864SendCmd(0x30);//�������ã�һ����8λ���ݣ�����ָ�
//	LCD12864SendCmd(0x0C);//0000,1100 ������ʾ���α�off���α�λ��off
//	LCD12864SendCmd(0x01);//0000,0001 ��DDRAM
//	LCD12864SendCmd(0x02);//0000,0010 DDRAM��ַ��λ
//	LCD12864SendCmd(0x80);//1000,0000 �趨DDRAM 7λ��ַ000��0000����ַ������AC
////      SendCMD(0x04);//���趨����ʾ�ַ�/����������λ��DDRAM��ַ�� һ
////      SendCMD(0x0F);//��ʾ�趨������ʾ����ʾ��꣬��ǰ��ʾλ��������
//}


