#include "timer.h"
#include "gizwits_product.h" 

extern u32 Alarm_Clock_Val;
extern u8 wifi_sta ;
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
extern void Set_Alarm_Clock(void);
extern u8 Medicine_Flag; //��ҩ��־λ
//��ʱ��3�жϷ������
int num_500ms = 0;
u8 delay_10ms = 0;
u8 display_status = 0;
u8 Water_Temp_status = 0;
u16 Delay_1000ms = 0;

extern _bool MedicineEventFlag;

s32 rand_1000 = 1000;
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	u8 key_status = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
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
				gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ 
				wifi_sta=0;//��־wifi�ѶϿ�
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

