/**
 * ��ʾ��غ���
 * ����:hhelib
 * �����£�2016-03-13
 */
 
#include "Display.h"
#include "app.h"
#include "PinDef.h"
#include "delay.h"
#include "HV57708.h"

//defination of pin actions
#define DIN_HIGH    MAX6921_DIN = 1;
#define DIN_LOW     MAX6921_DIN = 0;
#define CLK_HIGH    MAX6921_CLK = 1;
#define CLK_LOW     MAX6921_CLK = 0;
#define BLANK_HIGH  MAX6921_BLANK = 1;
#define BLANK_LOW   MAX6921_BLANK = 0;
#define LOAD_HIGH   MAX6921_LOAD = 1;
#define LOAD_LOW    MAX6921_LOAD = 0;

/**
 * void displayInit(void)
 * ��飺��ʾ��ʼ��
 * ���룺��
 * �������
 */
void displayInit(void)
{
	//MAX6921_Init(MAX6921_MODE_INVERTION);
}

/**
 * void displayNumber()
 * ��飺��ʾ����
 * ���ܣ��������ֵ� MAX6921
 * ���룺��Ҫ��ʾ���ַ�����Լ�λ��
 * �������
 */
void displayNumber(int num,int seg)
{
	int data_sequ[20] = {0};//���δ洢D19��DD0
	int i;
	BLANK_HIGH//�ر���ʾ
	LOAD_LOW//����
	data_sequ[num - 1] = 1;
	switch(seg)//data_sequ[19] = D0
	{
	case(0):{data_sequ[16] = 1;data_sequ[15] = 1;data_sequ[14] = 1;data_sequ[13] = 1;data_sequ[12] = 1;data_sequ[11] = 1;}
	break;
	case(1):{data_sequ[15] = 1;data_sequ[14] = 1;}
	break;
	case(2):{data_sequ[16] = 1;data_sequ[15] = 1;data_sequ[10] = 1;data_sequ[12] = 1;data_sequ[13] = 1;}
	break;
	case(3):{data_sequ[16] = 1;data_sequ[15] = 1;data_sequ[10] = 1;data_sequ[14] = 1;data_sequ[13] = 1;}
	break;
	case(4):{data_sequ[11] = 1;data_sequ[10] = 1;data_sequ[15] = 1;data_sequ[14] = 1;}
	break;
	case(5):{data_sequ[16] = 1;data_sequ[11] = 1;data_sequ[10] = 1;data_sequ[14] = 1;data_sequ[13] = 1;}
	break;
	case(6):{data_sequ[16] = 1;data_sequ[11] = 1;data_sequ[12] = 1;data_sequ[10] = 1;data_sequ[14] = 1;data_sequ[13] = 1;}
	break;
	case(7):{data_sequ[16] = 1;data_sequ[15] = 1;data_sequ[14] = 1;}
	break;
	case(8):{data_sequ[11] = 1;data_sequ[16] = 1;data_sequ[15] = 1;data_sequ[14] = 1;data_sequ[13] = 1;data_sequ[12] = 1;data_sequ[10] = 1;}
	break;
	case(9):{data_sequ[11] = 1;data_sequ[16] = 1;data_sequ[15] = 1;data_sequ[14] = 1;data_sequ[13] = 1;data_sequ[10] = 1;}
	break;
	case(10):{data_sequ[10] = 1;}
	break;//��ʾ�θ�
	case(11):{data_sequ[9] = 1;}
	break;//��ʾС����
	case(12):{;}
	break;//NULL
	case(13):{data_sequ[11] = 1;data_sequ[16] = 1;data_sequ[12] = 1;data_sequ[10] = 1;}
	break;//F
	case(14):{data_sequ[14] = 1;data_sequ[13] = 1;data_sequ[12] = 1;}
	break;//u
	case(15):{data_sequ[14] = 1;data_sequ[12] = 1;data_sequ[10] = 1;}
	break;//n
	case(16):{data_sequ[13] = 1;data_sequ[12] = 1;data_sequ[10] = 1;}
	break;//c
	}
    for(i = 0;i < 20;i++)
    {
    CLK_LOW
		Delay5us();
    	if(data_sequ[i] == 1)
    	{
    DIN_HIGH
		Delay5us();
    	}
    	else
    	{
    DIN_LOW
		Delay5us();
    	}
		CLK_HIGH
		Delay5us();
    }
	LOAD_HIGH//���벢���
	BLANK_LOW//�������
}

/**
 * void displayTime(void)
 * ��飺��ʾʱ��
 * ���ܣ�����ʱ����ʾ���ݵ�MAX6921��ʱ����ȫ�ֱ����д���
 * ���룺��
 * �������
 */
void displayTime(void)
{
	displayNumber(8,hour/10);
	displayNumber(7,hour%10);
	
	displayNumber(5,minute/10);
	displayNumber(4,minute%10);
	
	displayNumber(2,second/10);
	displayNumber(1,second%10);	

  displayNumber(9,12);
	displayNumber(6,12);
	displayNumber(3,12);			
}