#include "FirmwareDisplay.h"



/**
 * void displayInit(void)
 * 简介：显示初始化
 * 输入：无
 * 输出：无
 */
 
void FirmwarePeriphCfg(void)
{
	GPIO_InitTypeDef GPIO_InitTypeState;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeState.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitTypeState.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitTypeState.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitTypeState);
	
}
	
void displayInit(void)
{
	//MAX6921_Init(MAX6921_MODE_INVERTION);
}

/**
 * void displayNumber()
 * 简介：显示数字
 * 功能：发送数字到 MAX6921
 * 输入：需要显示的字符编号以及位置
 * 输出：无
 */
int const data_weima[9]={3,14,4,15,17,16,19,18,5};//1（0）-9（8）位

void displayNumber(int num,int seg)
{
	int data_sequ[20] = {0};//依次存储D19至DD0
	int i;
	BLANK_HIGH;//关闭显示
	LOAD_LOW;//锁存
	data_sequ[ data_weima[num - 1] ] = 1;
	switch(seg)//data_sequ[19] = D0
	{
		case(0):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
		break;
		case(1):{data_sequ[8] = 1;data_sequ[11] = 1;}
		break;
		case(2):{data_sequ[6] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[12] = 1;}
		break;
		case(3):{data_sequ[6] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
		break;
		case(4):{data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[11] = 1;}
		break;
		case(5):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
		break;
		case(6):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
		break;
		case(7):{data_sequ[6] = 1;data_sequ[8] = 1;data_sequ[11] = 1;}
		break;
		case(8):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
		break;
		case(9):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
		break;//A
		case(10):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[11] = 1;}
		break;//C
		case(11):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[12] = 1;}
		break;//D/0
		case(12):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
		break;//E
		case(13):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[12] = 1;}
		break;//F
		case(14):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[10] = 1;}
		break;//H
		case(15):{data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[11] = 1;}
		break;//L
		case(16):{data_sequ[7] = 1;data_sequ[10] = 1;data_sequ[12] = 1;}
		break;//P
		case(17):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;}
		break;//Q
		case(18):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[11] = 1;}
		break;//S
		case(19):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
		break;//U/V
		case(20):{data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
		break;//.
		case(21):{data_sequ[13] = 1;}
		break;//S
		case(22):{data_sequ[9] = 1;}
		break;
		default : break;
	}
    for(i = 0;i < 20;i++)
    {
		CLK_LOW;
		delay_us(1);
    	if(data_sequ[i] == 1)
    	{
			DIN_HIGH;
			delay_us(2);
    	}
    	else
    	{
			DIN_LOW;
			delay_us(2);
    	}
		CLK_HIGH;
		delay_us(1);
    }
	LOAD_HIGH;//载入并输出
	BLANK_LOW;//允许输出
	delay_us(2);
}



/**
 * void displayTime(void)
 * 简介：显示时间
 * 功能：发送时间显示数据到MAX6921，时间在全局变量中储存
 * 输入：无
 * 输出：无
 */
void displayTime(int * time, int num)
{
//	displayNumber(5,1);
	displayNumber(9,22);
	displayNumber(6,22);
	displayNumber(3,22);
	if(num != 1)
	{
		displayNumber(2,time[0]/10);
		displayNumber(1,time[0]%10);
	}
	if(num != 2)
	{
		displayNumber(5,time[1]/10);
		displayNumber(4,time[1]%10);
	}
	if(num != 3)
	{
		displayNumber(8,time[2]/10);
		displayNumber(7,time[2]%10);
	}
	displayNumber(7,24);			
}

void FirmwareShift(u8 digit,int *time, bool state)
{
	if(state == TRUE)
	{
		switch(digit)
		{
			case 1: displayTime(time + 4, 3); break;
			case 2: displayTime(time + 4, 2);break;
			case 3: displayTime(time + 4, 1);break;
			case 4: displayTime(time, 3);break;
			case 5: displayTime(time, 2);break;
			case 6: displayTime(time, 1);break;
			default :break;		
		}
	}
	else
	{
		switch(digit)
		{	
			case 1: 
			case 2: 
			case 3: displayTime(time + 4, 4);break;
			case 4: 
			case 5: 
			case 6: displayTime(time, 4);break;
			default :break;
		}
	}
}



