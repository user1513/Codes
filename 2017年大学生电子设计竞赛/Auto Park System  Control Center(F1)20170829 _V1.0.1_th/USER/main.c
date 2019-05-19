#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart3.h"
#include "led.h"
#include "ILI93xx.h"
#include "sram.h"   
#include "malloc.h" 
#include "touch.h"
#include "key.h"
#include "Speech_control.h"
#include "exti.h"

#include "GUI.h"
#include "includes.h"
#include "DIALOG.h"

#include "bitmapdisplay.h"
#include "ChooseTheGarageDLG.h"

//START����
//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С
#define START_STK_SIZE		1024
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//Touch����
//�������ȼ�
#define TOUCH_TASK_PRIO		4
//�����ջ��С
#define TOUCH_STK_SIZE		128
//������ƿ�
OS_TCB TouchTaskTCB;
//�����ջ
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//������
void touch_task(void *p_arg);

//LED0����
//�������ȼ�
#define LED0_TASK_PRIO		6
//�����ջ��С
#define LED0_STK_SIZE		128
//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//������
void led0_task(void *p_arg);

//�Ʒ�������
//�������ȼ�
#define INFARRED_TASK_PRIO		5
//�����ջ��С
#define INFARRED_STK_SIZE		128
//������ƿ�
OS_TCB InfarredTaskTCB;
//�����ջ
CPU_STK INFARRED_TASK_STK[INFARRED_STK_SIZE];
//������
void infarred_task(void *p_arg);


//emwindemo����
//�������ȼ�
#define EMWINDEMO_TASK_PRIO		7
//�����ջ��С
#define EMWINDEMO_STK_SIZE		2048
//������ƿ�
OS_TCB EmwindemoTaskTCB;
//�����ջ
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//������
void emwindemo_task(void *p_arg);

int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();	    	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	uart3_init(9600);
 	LED_Init();			    //LED�˿ڳ�ʼ��
	//KEY_Init();				//
	EXTIX_Init();
	TFTLCD_Init();			//��ʼ��LCD
	LCD_Clear(BLACK);
	TP_Init();				//��ʼ��������
	FSMC_SRAM_Init(); 		//SRAM��ʼ��	
	my_mem_init(SRAMIN); 		//�ڲ�RAM��ʼ��
	my_mem_init(SRAMEX); 		//�ⲿRAM��ʼ��
	

	
	OSInit(&err);			//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//����CRCʱ��
	GUI_Init();  			//STemWin��ʼ��
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//STemWin Demo����	
	OSTaskCreate((OS_TCB*     )&EmwindemoTaskTCB,		
				 (CPU_CHAR*   )"Emwindemo task", 		
                 (OS_TASK_PTR )emwindemo_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDEMO_TASK_PRIO,     
                 (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//����������
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			 
	//LED0����
	OSTaskCreate((OS_TCB*     )&Led0TaskTCB,		
				 (CPU_CHAR*   )"Led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK*    )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//��������
	OSTaskCreate((OS_TCB*     )&InfarredTaskTCB,		
				 (CPU_CHAR*   )"InfarRed task", 		
                 (OS_TASK_PTR )infarred_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )INFARRED_TASK_PRIO,     
                 (CPU_STK*    )&INFARRED_TASK_STK[0],	
                 (CPU_STK_SIZE)INFARRED_STK_SIZE/10,	
                 (CPU_STK_SIZE)INFARRED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}
//touch����
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);
	}
}
u16 ParkingDurationS;
u16 ParkingFeeRMB; 
extern char start_static;//��ʼ״̬
extern char Garage;
u8 js_start_static = 0;//����Ʒѣ�Ĭ��Ϊ0������Ϊ1 
extern u8 KEY_U8;
//LED0����
void led0_task(void *p_arg)
{
	OS_ERR err;
	u8 cnt = 0;
	while(1)
	{
		if(start_static == 1)
		{
			if(js_start_static == 1)
			{
				
				//��Ҫ�ж�һ������Ŀ�ʼŶ
				cnt++;
				if(cnt == 2)
				{
					cnt = 0;
					ParkingDurationS ++;
					ParkingFeeRMB = ParkingDurationS / 30 * 5;
					if(ParkingDurationS % 30 != 0)
					{
						ParkingFeeRMB = ParkingFeeRMB + 5;
					}
				}				
			}
		}else
		{
			cnt = 0;
		}
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);
	}
}
extern u8 CollisionValue;
void infarred_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		if(KEY_U8 == 1)
		{
			KEY_U8 = 0;
			if(js_start_static == 0)
			{
				Speech_Switch_Go(Garage+1);
				js_start_static = 1;
			}else if(js_start_static == 1)
			{
				js_start_static = 0;
				Speech_Switch_home(ParkingDurationS,ParkingFeeRMB,CollisionValue);
			}
			OSTimeDlyHMSM(0,0,5,50,OS_OPT_TIME_PERIODIC,&err);

		}
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);
	}
}
extern char Cancel_key_static;
 
//emwindemo_task����
void emwindemo_task(void *p_arg)
{
	OS_ERR err;
	//����Ƥ��
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	

	//�������
	GUI_CURSOR_Show();
	//������ҳ
	draw_bitmap();
	OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_PERIODIC,&err);//��ʱ5s
	

	while(1)
	{
		tp_dev.scan(1);//ɨ����������
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//����������һ��(��ʱ�����ɿ���.)
		{	
			tp_dev.sta&=~(1<<6);//��ǰ����Ѿ����������.
			Speech_null();
			break;
		}
		GUI_Delay(500);
	}
	CreateChooseTheGarage();
	while(1)
	{
		if(Cancel_key_static == 1)
		{
			Cancel_key_static = 0;
			//����
			USART1Send(0x55);
			USART1Send(0x01);
			USART1Send(0x00);
			USART1Send(0x07);
			USART1Send(0x08);
			USART1Send(0xAA);
			OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&err);//��ʱ20ms
			Speech_Reset();
			OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&err);//��ʱ20ms
			
			__set_FAULTMASK(1);
			NVIC_SystemReset();
		}
		updateDialogBox();
		GUI_Delay(500);
	}
}

