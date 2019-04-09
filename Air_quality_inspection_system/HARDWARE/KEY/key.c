#include "key.h"
#define KEYTIMEOUT    100// 200*10MS = 1S

uint16_t key1TimeOut = 0;			//定义超时静态变量
uint8_t previous_key1 = 1 ;		//定义存储上一次按键值静态变量
uint8_t key1_flag = 0;			//定义按键状态静态变量
	
uint8_t previous_key2 = 1 ;		//定义存储上一次按键值静态变量
uint8_t key2_flag = 0;			//定义按键状态静态变量

uint8_t previous_key3 = 1 ;		//定义存储上一次按键值静态变量
uint8_t key3_flag = 0;			//定义按键状态静态变量


//按键io初始化
void Key_Io_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE); //使能PD 端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;		//GPIO_Pin_14,15		     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 		//输入上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);					       //根据设定参数初始化GPIOC.14,15

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					//GPIO_Pin_2		     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 		//输入上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					       //根据设定参数初始化GPIOC.14,15	
}


//按键事件处理函数

uint8_t Key_Event_Handler(void)
{
	uint8_t key_val = 1;
	key_val |= 1 << BUTTONLONGPRESS(1, MIDDLE_KEY, &key1TimeOut, &previous_key1, &key1_flag);   //获取中间按键状态
	key_val |= 1 << BUTTONPOINTSMOVE(3, ABOVE_KEY, &previous_key2, &key2_flag);					//获取上面按键状态
	key_val |= 1 << BUTTONPOINTSMOVE(4, BELOW_KEY, &previous_key3, &key3_flag);					//获取下间按键状态
	return key_val;
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
	
	if(current_key == 0 && *previous_key == 0 && *key_flag == 0)   	//判断当前按键值与之前按键是否都为低电平并且判断标志位是否为0
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
	if(*previous_key && *key_flag)
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

