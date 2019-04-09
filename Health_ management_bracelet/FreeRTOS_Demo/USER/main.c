#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "key.h"
#include "oled.h"
#include "bmp.h"
#include "rtc.h"
#include "limits.h"
#include "max30102.h"
#include "timers.h"
#include "app.h"
/************************************************
 ALIENTEK 探索者STM32F407开发板 FreeRTOS实验2-1
 FreeRTOS移植实验-库函数版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/




void vTaskinit(void * pvParameters); //INIT任务函数

#define TaskinitLenth 64			 //INIT任务字节长度

#define TaskinitPriority 1			 //INIT任务优先级

TaskHandle_t TaskinitHandle;		 //INIT任务句柄
///////////////////////////////////////////////////////


void vTaskLed(void * pvParameters); //LED任务函数

#define TaskLedLenth 64			//LED任务字节长度

#define TaskLedPriority 4			//LED任务优先级

TaskHandle_t TaskLedHandle;			//LED任务句柄
///////////////////////////////////////////////////////


void vTaskOledDisplay(void * pvParameters);	//oled12684任务函数

#define TaskOledDisplayLenth 64				//oled12684任务长度
	
#define TaskOledDisplayPriority 3			//oled12684任务优先级

TaskHandle_t TaskOledDisplayAssistHandle;	//oled12684任务句柄

/////////////////////////////////////////////////////////

//显示任务辅助任务,状态机

typedef enum {No_Display_State, Display_Time_State, Display_heart_rate_State, Display_Gps_State, Display_poisonous_gas_State} STATE_MACHINE;

STATE_MACHINE Current_State = Display_Time_State;
STATE_MACHINE Next_State = No_Display_State;

void vTaskDisplayAssist(void * pvParameters);	//vTaskDisplayAssist任务函数

#define TaskOledDisplayAssistLenth 64			//vTaskDisplayAssist任务长度
	
#define TaskOledDisplayAssistPriority 1			//vTaskDisplayAssist任务优先级

TaskHandle_t TaskOledDisplayHandle;				//vTaskDisplayAssist任务句柄
/////////////////////////////////////////////////////////


void vTaskGETRTC(void * pvParameters);	//RTC任务函数

#define TaskRTCLenth 64				//RTC任务长度
	
#define TaskRTCPriority 2			//RTC任务优先级

TaskHandle_t TaskRTCHandle;			//RTC任务句柄


RTC_TimeTypeDef RTC_TimeStruct;		//RTC Time结构体
RTC_DateTypeDef RTC_DateStruct;		//RTC Data结构体
/////////////////////////////////////////////////////////


void vTaskMAX30102(void * pvParameters);	//RTC任务函数

#define TaskMAX30102Lenth 256				//RTC任务长度
	
#define TaskMAX30102Priority 2			//RTC任务优先级

TaskHandle_t TaskMAX30102Handle;			//RTC任务句柄

char heart_rate = 0;
/////////////////////////////////////////////////////////

TimerHandle_t Timer1;

TimerHandle_t Timer_timeout;


//uint32_t timeout_flag = 0;

 void TimerCallbackFunc(TimerHandle_t xTimer);
 void TimerCallbackFunc_Timeout_detection(TimerHandle_t xTimer);

int main(void)
{
	HAL_Init();
	
	LED_Init();
	
	key_Init();
	
	delay_init(168);
	
	uart_init(115200);
	
	OLED_Init();
	OLED_Clear(); 
	//OLED_DrawBMP(0,0,128,2,BMP1); 
	
	RTC_Init();                     //初始化RTC 
	
	bsp_InitI2C();
	
	maxim_max30102_init();
	
	xTaskCreate((TaskFunction_t	) vTaskinit,
				(const char *	) "Task_init",
				(uint16_t		) TaskinitLenth,
				(void * 		)	NULL,
				(UBaseType_t	) TaskinitPriority,
				(TaskHandle_t *	) &TaskinitHandle );
				
	vTaskStartScheduler();
}



void vTaskinit(void * pvParameters)
 {
	 LED = 0;
	 taskENTER_CRITICAL();           //进入临界区
	 Timer1 = xTimerCreate( "heart_rate_display",
  							200,
  							pdTRUE,
  							(void *)1,
  							TimerCallbackFunc);
							
     Timer_timeout = xTimerCreate( "one_shot_timer",
  							500,
  							pdFALSE,
  							(void *)2,
  							TimerCallbackFunc_Timeout_detection);							
	 xTaskCreate((TaskFunction_t) vTaskLed,
				(const char *	) "Task_Led",
				(uint16_t		) TaskLedLenth,
				(void * 		)	NULL,
				(UBaseType_t	) TaskLedPriority,
				(TaskHandle_t *	) &TaskLedHandle );
	
	xTaskCreate((TaskFunction_t) vTaskOledDisplay,
				(const char *	) "Task_OledDisplay",
				(uint16_t		) TaskOledDisplayLenth,
				(void * 		)	NULL,
				(UBaseType_t	) TaskOledDisplayPriority,
				(TaskHandle_t *	) &TaskOledDisplayHandle );
				
	xTaskCreate((TaskFunction_t) vTaskDisplayAssist,
				(const char *	) "Task_Display_assist",
				(uint16_t		) TaskOledDisplayAssistLenth,
				(void * 		)	NULL,
				(UBaseType_t	) TaskOledDisplayAssistPriority,
				(TaskHandle_t *	) &TaskOledDisplayAssistHandle );
	
	xTaskCreate((TaskFunction_t) vTaskGETRTC,
				(const char *	) "Task_RTC",
				(uint16_t		) TaskRTCLenth,
				(void * 		)	NULL,
				(UBaseType_t	) TaskRTCPriority,
				(TaskHandle_t *	) &TaskRTCHandle );
	
	xTaskCreate((TaskFunction_t) vTaskMAX30102,
				(const char *	) "Task_MAX30102",
				(uint16_t		) TaskMAX30102Lenth,
				(void * 		)	NULL,
				(UBaseType_t	) TaskMAX30102Priority,
				(TaskHandle_t *	) &TaskMAX30102Handle );
							
	vTaskDelete(TaskinitHandle);
	
	taskEXIT_CRITICAL();            //退出临界区
 }


 
 //oled显示任务
 
void vTaskOledDisplay(void * pvparameters)
{
	
	uint32_t _NotificationValue = 0;
	BaseType_t err = 0;
	uint32_t temp = 0;
	for(;;)
	{
		err = xTaskNotifyWait( ULONG_MAX, ULONG_MAX, &_NotificationValue, portMAX_DELAY);
		
 		if(err == pdTRUE)
		{
			switch(_NotificationValue)
			{
				case 0x1:
					OLED_Clear();
				break;
				case 0x1 << 1:
					OLED_DrawBMP(0,0,128,1,BMP1);
//					OLED_Show_32_char(0, 2,  BMP2[0]);
//					OLED_Show_32_char(32, 2, BMP2[8]);
//					OLED_Show_32_char(16*7-16, 2,  BMP2[16]);
					OLED_Show_32_32(16*2-8,2,RTC_TimeStruct.Hours / 10 + '0');
					OLED_Show_32_32(16*3-8,2,RTC_TimeStruct.Hours % 10 + '0');
					OLED_Show_32_32(16*4-8,2,':');
					OLED_Show_32_32(16*5-8,2,RTC_TimeStruct.Minutes / 10 + '0');
					OLED_Show_32_32(16*6-8,2,RTC_TimeStruct.Minutes % 10 + '0');
					OLED_ShowChar(32,	  6, RTC_DateStruct.Year / 10 + '0',16);
					OLED_ShowChar(32+8*1, 6, RTC_DateStruct.Year % 10 + '0',16);
					OLED_ShowChar(32+8*2, 6, '-',16);
					OLED_ShowChar(32+8*3, 6, RTC_DateStruct.Month / 10 + '0',16);
					OLED_ShowChar(32+8*4, 6, RTC_DateStruct.Month % 10 + '0',16);
					OLED_ShowChar(32+8*5, 6, '-',16);
					OLED_ShowChar(32+8*6, 6, RTC_DateStruct.Date / 10 + '0',16);
					OLED_ShowChar(32+8*7, 6, RTC_DateStruct.Date % 10 + '0',16);
				break;
				case 0x1 << 2:
					OLED_Part_Clear(2,7);
				break;
				case 0x1 << 3:
					OLED_Part_Clear(2,7);
					OLED_ShowChar(32,	  6, '1',16);
					OLED_ShowChar(32+8*1, 6, '1',16);
					OLED_ShowChar(32+8*2, 6, '-',16);
					OLED_ShowChar(32+8*3, 6, '1',16);
					OLED_ShowChar(32+8*4, 6, '1',16);
					OLED_ShowChar(32+8*5, 6, '-',16);
					OLED_ShowChar(32+8*6, 6, '1',16);
					OLED_ShowChar(32+8*7, 6, '1',16);
				break;
				case 0x1 << 4:
					OLED_ShowChar(32,	  6, '2',16);
					OLED_ShowChar(32+8*1, 6, '2',16);
					OLED_ShowChar(32+8*2, 6, '-',16);
					OLED_ShowChar(32+8*3, 6, '2',16);
					OLED_ShowChar(32+8*4, 6, '2',16);
					OLED_ShowChar(32+8*5, 6, '-',16);
					OLED_ShowChar(32+8*6, 6, '2',16);
					OLED_ShowChar(32+8*7, 6, '2',16);
				break;
				case 0x1 << 5:
					OLED_Show_32_char(48, 2,  BMP2[0]);
					OLED_ShowString(32-8, 6, (uint8_t *)"...      ", 16);
				break;
				
				case 0x1 << 6:
					OLED_Show_32_char(48, 2, BMP2[8]);
					OLED_ShowString(32-8, 6, (uint8_t *)"......   ", 16);
				break;
				
				case 0x1 << 7:
					OLED_Show_32_char(48, 2,  BMP2[16]);
					OLED_ShowString(32-8, 6, (uint8_t *)".........", 16);
				break;
				
				case 0x1 << 8:
					OLED_Part_Clear(2,5);
					OLED_ShowCHinese(0 ,2 ,0);
					OLED_ShowCHinese(16 ,2 ,1);
					OLED_ShowCHinese(16 * 2 ,2 ,2);
					OLED_ShowChar(16 * 3, 2, ':',16);
					temp = heart_rate/100 ;
					if(temp != 0)
						OLED_Show_32_32(16 * 4 - 8,3,temp + '0');
					temp = (heart_rate % 100)/10 ;
					if(temp != 0)
						OLED_Show_32_32(16 * 4 +16- 8, 3, temp + '0');
					OLED_Show_32_32(16 * 4 +32- 8, 3,heart_rate % 10 + '0');
					OLED_ShowString(32-8, 6, (uint8_t *)"          bpm", 16);
				break;
				default :break;
			}
		}
		vTaskDelay(100);
	}
}		
		
		
//显示任务辅助任务,状态机
void vTaskDisplayAssist(void * pvParameters)
{
	uint32_t _NotificationValue = 0;
	BaseType_t err = 0;
	if( Timer_timeout != NULL)
	{
		err = xTimerChangePeriod( Timer_timeout ,10000 ,0 ); 
		//err = xTimerStart( Timer_timeout, 0 );
	}
	for(;;)
	{
		err = xTaskNotifyWait( ULONG_MAX, ULONG_MAX, &_NotificationValue, portMAX_DELAY);
		if(_NotificationValue == 0x1 && err == pdTRUE)
		{
			Current_State = Next_State;
		}
		if((_NotificationValue & 0x4) == 0x4)
		{
			Current_State = No_Display_State;
			_NotificationValue = 0x1;
		}
		if((_NotificationValue & 0x8) == 0x8)
		{
			Current_State = Display_Time_State;
			_NotificationValue = 0x1;
		}
		if((_NotificationValue == 0x2 && Current_State == Display_Time_State) || _NotificationValue == 0x1)
		{
			switch(Current_State)
			{
				case No_Display_State:
					Next_State = Display_Time_State;
					xTaskNotify( TaskOledDisplayHandle, 0x1, eSetBits); break;
				case Display_Time_State:	
					Next_State = Display_heart_rate_State;
					if( Timer_timeout != NULL && _NotificationValue != 0x2)
					{
						// 启动定时器
						err = xTimerChangePeriod( Timer_timeout ,10000 ,0 ); 
						//err = xTimerReset( Timer_timeout, 0 );
					}
					xTaskNotify( TaskOledDisplayHandle, 0x1<<1, eSetBits); break;
				case Display_heart_rate_State:	
					
					Next_State = Display_Gps_State;		
			 		xTimerStop(Timer_timeout, 0);		
					xTaskNotify( TaskOledDisplayHandle, 0x1<<2, eSetBits);
					xTaskNotify( TaskMAX30102Handle, 0x1, eSetBits); 
					if( Timer1 != NULL )
					{
						// 启动定时器
						err = xTimerStart( Timer1, 0 );
					}
					break;
				case Display_Gps_State:	
					err = xTimerChangePeriod( Timer_timeout ,10000 ,0 ); 
					//err = xTimerReset( Timer_timeout, 0 );
					Next_State = Display_poisonous_gas_State;
					xTimerStop(Timer1, 0);
					xTaskNotify( TaskOledDisplayHandle, 0x1<<3, eSetBits); break;
				case Display_poisonous_gas_State:
					err = xTimerChangePeriod( Timer_timeout ,10000 ,0 ); 
					//err = xTimerReset( Timer_timeout, 0 );
					Next_State = Display_Time_State;
					xTaskNotify( TaskOledDisplayHandle, 0x1<<4, eSetBits); break;
				default : Next_State = Display_Time_State;	
					xTaskNotify( TaskOledDisplayHandle, 0x1<<1, eSetBits); break;
			}
		}
		vTaskDelay(100);
	}
}	

 //RTC时间获取任务

void vTaskGETRTC(void * pvParameters)
{
	for( ;; )
	{
		HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
//		sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds); 
//		printf("%s\n",tbuf);	
		HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
//		sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); 
//		printf("%s\n",tbuf);	
//		sprintf((char*)tbuf,"Week:%d",RTC_DateStruct.WeekDay); 
//		printf("%s\n",tbuf);
		xTaskNotify( TaskOledDisplayAssistHandle, 0x2, eSetBits); 	
		vTaskDelay(5000);
	}
}


void vTaskMAX30102(void * pvParameters)
{
	uint32_t _NotificationValue = 0;
	BaseType_t err = 0;
	//max30102_onlyone_run();
	for(;;)
	{
		err = xTaskNotifyWait( ULONG_MAX, ULONG_MAX, &_NotificationValue, portMAX_DELAY);
		heart_rate = max30102_run();
		xTimerStop(Timer1, 0);
		if(Current_State == Display_heart_rate_State)
			xTaskNotify( TaskOledDisplayHandle, 0x1<<8, eSetBits);
		err = xTimerChangePeriod( Timer_timeout ,10000 ,0 ); 
		//err = xTimerReset( Timer_timeout, 0 );
		vTaskDelay(500);
	}
}





 
 //MCU状态指示灯 + KEY

 char _pcWriteBuffer[400] = {0};
 
void vTaskLed(void * pvParameters)
 {
	 uint8_t led_delay = 0;
	 for( ;; )
	 {
		 if(++led_delay == 25)
		 {
			led_delay = 0;
			LED = ~LED;
		 }
	
		
		if(GetKeyVAl(1) == 0x01)
		{
			vTaskList(_pcWriteBuffer);
			printf("%s", _pcWriteBuffer);
			//xTaskNotify( TaskMAX30102Handle, 0x1, eSetBits);
			xTaskNotify( TaskOledDisplayAssistHandle, 0x1, eSetBits);
		}			
		//printf("1\n");
		vTaskDelay(20);
		 
	 }
 }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 //////////////////////////////////////////////////////定时器回调函数
 
 
 void TimerCallbackFunc(TimerHandle_t xTimer)
 {
		static uint8_t countval = 0;
		if(countval == 2)
			xTaskNotify( TaskOledDisplayHandle, 0x1 << 5, eSetBits);
		if(countval == 4)
			xTaskNotify( TaskOledDisplayHandle, 0x1 << 6, eSetBits);
		if(countval == 6)
		{
			xTaskNotify( TaskOledDisplayHandle, 0x1 << 7, eSetBits);
			countval = 0;
		}
		countval ++;	
 }
 
 
 
  void TimerCallbackFunc_Timeout_detection(TimerHandle_t xTimer)
 {
		if(Current_State == Display_Time_State)
		{
			xTaskNotify( TaskOledDisplayAssistHandle, 0x1<<2, eSetBits);
		}
		else
		{
			OLED_Part_Clear(2,7);
			xTaskNotify( TaskOledDisplayAssistHandle, 0x1<<3, eSetBits);
		}
 }
 
 
 
 
 


