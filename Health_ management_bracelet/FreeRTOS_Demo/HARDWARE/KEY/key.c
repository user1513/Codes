#include "key.h"



void key_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOF时钟
	
    GPIO_Initure.Pin = GPIO_PIN_15; //PF9,10
    GPIO_Initure.Mode = GPIO_MODE_INPUT;  //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;          //上拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}


uint8_t GetKeyVAl(uint8_t mode)
{
	static uint8_t Previous_KEY_VAL = 0, stste = 0;
	static uint8_t KEY_state = 0;
	uint8_t current_key_val = KEY;
	if(current_key_val == 0 && Previous_KEY_VAL == current_key_val && stste)
	{
		if(mode == 1)
		{
			stste = 0;
		}
		KEY_state |= 0x01;
	}
	else
	{
		KEY_state &= ~0x01;
	}	
	if(current_key_val == 0)
	{
		Previous_KEY_VAL &= ~0x01; 
	}
	else
	{
		stste = 1;
		Previous_KEY_VAL |= 0x01; 
	}		
	return KEY_state;
}

