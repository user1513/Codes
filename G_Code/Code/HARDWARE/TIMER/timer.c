#include "timer.h"
#include "gizwits_product.h" 

extern u32 Alarm_Clock_Val;
extern u8 wifi_sta ;
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
extern void Set_Alarm_Clock(void);
extern u8 Medicine_Flag; //吃药标志位
//定时器3中断服务程序
int num_500ms = 0;
u8 delay_10ms = 0;
u8 display_status = 0;
u8 Water_Temp_status = 0;
u16 Delay_1000ms = 0;

extern _bool MedicineEventFlag;

s32 rand_1000 = 1000;
void TIM3_IRQHandler(void)   //TIM3中断
{
	u8 key_status = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		gizTimerMs();
		FuncTimeTravers_ISR();
		rand_1000++;
		if(rand_1000 < 0)
			rand_1000 = 1000;
		if(++num_500ms >= 500)
		{
			num_500ms = 0;
			led = ~led;
		}
		if(++delay_10ms >= 10)
		{
			delay_10ms = 0;
			 key_status = Key_Event_Handler();
			if(key_status == 2 && Medicine_Flag == 0)
			{
				display_status ++;
				if(display_status > 1)
					display_status = 0;
			}
			else if(key_status == 2 && Medicine_Flag == 1)
			{
				Medicine_Flag = 0;
			}
			else if(key_status == 4)
			{
				display_status = 3;
			}
			else if(key_status == 8)
			{
				gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位 
				wifi_sta=0;//标志wifi已断开
			}
		}
		Set_Alarm_Clock();
		if(Alarm_Clock_Val)
		{
			if((Delay_1000ms ++) >= 999)
			{
				Delay_1000ms = 0;
				Alarm_Clock_Val --;
			}
		}
		else if(currentDataPoint.valueTime != 0)
		{
			currentDataPoint.valueTime = 0;
			Medicine_Flag |= 1;
			MedicineEventFlag = TRUE; 
		}
		
		
		
	}
}

