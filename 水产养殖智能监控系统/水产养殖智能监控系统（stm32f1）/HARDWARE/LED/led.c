#include "led.h"

bool KeyContinuityFlag =FALSE;
bool KeyContinuityTestFlag =TRUE;
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PD 端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				     //LED-->PD.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					       //根据设定参数初始化GPIOD.2
	GPIO_SetBits(GPIOC,GPIO_Pin_13);						           //PD.2 输出高，不亮灯
}

void FeedIoInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PD 端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				     //LED-->PD.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					       //根据设定参数初始化GPIOD.2
	GPIO_SetBits(GPIOA,GPIO_Pin_0);						           //PD.2 输出高，不亮灯
}

void KeyIoInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE); //使能PD 端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				     //LED-->PD.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);					       //根据设定参数初始化GPIOD.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7;				     //LED-->PD.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
}


/*
  GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
*/
void jdqIoInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PD 端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				     //LED-->PD.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOD.2					           
}

void IoInit(void)
{
	jdqIoInit();
	LED_Init();
	FeedIoInit();
	KeyIoInit();
}

u8 GetKeySelflocking(void)
{
	u8 keyval = 0;
	static u8 key_state = 1;
	if(key_state && (keyS1 ==0 || keyS2 == 0))
	{
		delay_ms(5);
		if(keyS1 == 0)
		{
			key_state = 0;
			keyval = 1;
			return keyval;
		}
		if(keyS2 == 0)
		{
			key_state = 0;
			keyval = 2;
			return keyval;
		}
//		if(keyS5 == 0)
//		{
//			key_state = 0;
//			keyval = 5;
//			return keyval;
//		}
	}
	else if(keyS1 ==0 || keyS2 == 0)
	{
		key_state = 0;
	}
	else
	{
		key_state = 1;
	}
	return keyval;
}


void GetKeycontinuity(void)//连续获取按键值
{
	static u8 key_state = 1;
	if(key_state && (keyS4 ==0 || keyS5 == 0))
	{
		delay_ms(5);
		if(keyS4 == 0)
		{			
			KeyInside(1);
			key_state = 0;
		}
		if(keyS5 == 0)
		{
			KeyInside(0);
			key_state = 0;
		}
	}
	else if(keyS4 ==0 || keyS5 == 0)
	{
		key_state = 0;
		if(KeyContinuityTestFlag == TRUE)
		{
			KeyContinuityTestFlag = FALSE;
			FuncReg(&KeyContinuityFlag,200,TRUE);	
		}
	}
	else
	{
		key_state = 1;
		KeyContinuityTestFlag = TRUE;
		CleanCallbackFunc(&KeyContinuityFlag,FALSE);
	}
	if(KeyContinuityFlag == TRUE)
	{
		KeyContinuityFlag = FALSE;
		KeyContinuityTestFlag = TRUE;
		key_state = 1;
	}
}

void KeyHandle(void)//
{
	u8 keyval = 0;
	keyval = GetKeySelflocking();
	switch(keyval)
	{
		case 1: DataSave[0]++;
				if(DataSave[0] == 2)
				{
					DataSave[0] = 0;
				}
				break;
		case 2: DataSave[1]++;
				if(DataSave[1] == 3)
				{
					DataSave[1] = 0;
				}
				break;
		case 5: DataSave[2] = 1;break;
		default :break;
	}
	GetKeycontinuity();
}

void KeyInside(u8 mode)//按键内部判断函数
{
	if(DataSave[0] == 1)
	{
		if(mode != 0)
		{
			switch(DataSave[1])
			{
				case 0:DataSave[3]++;
					   if(DataSave[3] > 100)
					   {
						   DataSave[3] = 100;
					   }
						break;
				case 1:DataSave[4]++;
					   if(DataSave[4] > 100)
					   {
						   DataSave[4] = 100;
					   }
						break;
				case 2:DataSave[5]++;
					   if(DataSave[5] > 100)
					   {
						   DataSave[5] = 100;
					   }
						break;
				default: break;
			}
		}
		else
		{
			switch(DataSave[1])
			{
				case 0:DataSave[3]--;
					  if(DataSave[3] < 0)
					  {
						 DataSave[3] = 0; 
					  }
					  break;
				case 1:DataSave[4]--;
					  if(DataSave[4] < 0)
					  {
						 DataSave[4] = 0; 
					  }
					  break;
				case 2:DataSave[5]--;					  
					  if(DataSave[5] < 0)
					  {
						 DataSave[5] = 0; 
					  }
					  break;
				default: break;
			}
		}
		
	}
}




 




