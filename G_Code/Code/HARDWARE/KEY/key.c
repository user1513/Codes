#include "key.h"

#define KEYTIMEOUT 400

#define LEVEL 0
uint8_t previous_key1 = 1 ;		//定义存储上一次按键值静态变量
uint8_t key1_flag = 0;			//定义按键状态静态变量
uint16_t key1_TimeOut = 0;

uint8_t previous_key2 = 1 ;		//定义存储上一次按键值静态变量
uint8_t key2_flag = 0;			//定义按键状态静态变量
//按键io初始化
void Key_Io_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE); //使能PD 端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;		//GPIO_Pin_14,15		     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 		//输入上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					       //根据设定参数初始化GPIOC.14,15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;		//GPIO_Pin_14,15		     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 		//输入上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOC.14,15

}


//按键事件处理函数

uint8_t Key_Event_Handler(void)
{
	uint8_t key_val = 1;
	key_val |= 1 << BUTTONLONGPRESS(1, KEY_1, &key1_TimeOut, &previous_key1, &key1_flag) ;
	key_val |= 1 << BUTTONPOINTSMOVE(3, KEY_2, &previous_key2, &key2_flag);
	return key_val - 1;
}


/**********************************************************************************************************
** Function name        :   Key_Get
** Descriptions         :   按键值获取函数
** parameters           :   mode 按键获取模式（0点动，1长按）
** parameters           :   key_num 设置按键反馈值
** parameters           :   current_key 传入按键值
** parameters           :   timeout 超时时间
** parameters           :   previous_key 上一次按键值
** parameters           :   key_flag 按键标志位
** Returned value       :   按下返回设定按键值，反之返回0
***********************************************************************************************************/
	
uint8_t Key_Get(uint8_t mode, uint8_t key_num, uint8_t current_key, uint16_t * keyTimeOut,uint8_t* previous_key,uint8_t *key_flag)
{
	
	if(current_key == (!LEVEL) && *previous_key == (!LEVEL) && *key_flag == 0)   	//判断当前按键值与之前按键是否都为低电平并且判断标志位是否为0
	{
		if(keyTimeOut != (uint16_t *)0)								
			*keyTimeOut = 0;											
		*key_flag = 1;
	}
	if(*key_flag == 1 && mode == 0)									//当按键按下判断模式				
	{
		*key_flag = 2;
		return key_num;
	}
	else if(*key_flag == 1 && mode)									//当按键按下判断模式
	{
		if(key_Long_press_test(keyTimeOut) == 0x01)					//调用按键长按检测函数
		{
			*key_flag = 2;
			return key_num + 1;
		}
	}
	*previous_key = current_key; 				//保存本次的值
	if((*previous_key == LEVEL) && *key_flag)
	{	
		if(mode == 1 && *key_flag == 1)
		{
			*key_flag = 0;
			return key_num;
		}
		*key_flag = 0;	
	}	
	return 0;
}

uint8_t key_Long_press_test(uint16_t * keyTimeOut)  //按键长按检测
{
	if((*keyTimeOut) ++ > KEYTIMEOUT)	//判断是否超时（更改宏定义KEYTIMEOUT可以改超时时间）
	{
		*keyTimeOut = 0;
		return 1;					//超时输出1
	}
	return 0xff;					//不超时输出0xff
}

