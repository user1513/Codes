#include "main.h"

typedef enum{InitDisplayState = 0, NumModeDisplayState, GraphModeDisplayState, ParameterSetDisplayState, ErrStatus = -1 } DisplayState; //创建状态机枚举类型

DisplayState _DisplayState = InitDisplayState;		//定义一个枚举变量
	
DisplayState _InitDispay = GraphModeDisplayState;	//定义一个枚举变量

bool LCD12864_START_FLAG = FALSE;  					//设置LCD12864开始标志位,默认4秒后进入主界面

bool DISPLAY_STATE_FLAG = FALSE;  					//设置display_state开始标志位,默认4.5秒后开始运行

#define LCD12864_START_TIME 4000   					//设置LCD12864开始时间,默认4秒后进入主界面

#define DISPLAY_STATE_TIME 4500   					//设置display_state开始时间,默认4.5秒后开始运行

#define SETVAL_MAX	4000							//设置最大值为4000

#define SETVAL_MIN	0								//设置最大值为0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PeriphInit(void);								//外设初始化

void display_state(void);							//显示状态机函数

int cmp(const void *a,const void *b);				//qsort

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int SetVal = 1000;									//定义传感器设置值默认1000，最大4000，最小0

double CurrentVal = 0.0;							//定义传感器当前值变量

uint8_t PM2_5_table[128] = {0};						//创建一个存放PM2.5的数组进行图形显示

uint8_t PM2_5_table_test[20] = {0};					//创建一个临时存放PM2.5的数组

uint8_t GetDataFlag = 0;							//获取数据到达20个标志位

uint8_t keyval = 0;
int main(void)
{
	int i = 0;
	
	PeriphInit();
	Lcd12864Display(0x80,(uint8_t *)"    欢迎使用    "); 
	Lcd12864Display(0x90,(uint8_t *)"  基于单片机的  ");
	Lcd12864Display(0x88,(uint8_t *)"  空气质量检测  ");
	Lcd12864Display(0x98,(uint8_t *)"      --系统设计");
	LCD12864Display_ON();													
	FuncReg(&LCD12864_START_FLAG, LCD12864_START_TIME, TRUE);	//设置LCD12864_START_FLAG定时回调任务
	FuncReg(&DISPLAY_STATE_FLAG, DISPLAY_STATE_TIME, TRUE);	//设置LCD12864_START_FLAG定时回调任务
	while(1)
	{
		if(LCD12864_START_FLAG == TRUE && _DisplayState == InitDisplayState)
		{
			LCD12864_START_FLAG = FALSE;
			Lcd_Clear(0);
			Lcd12864Display(0x80,(uint8_t *)"    模式选择    "); 
			Lcd12864Display(0x90,(uint8_t *)"    数字模式    ");
			Lcd12864Display(0x88,(uint8_t *)"    图形模式    ");
			Lcd12864Display(0x98,(uint8_t *)"    参数设置    ");
			Lcd12864String(0x91,0,(uint8_t *)" >");
			Lcd12864String(0x96,0,(uint8_t *)"<");
		}
		if(_DisplayState == NumModeDisplayState && LCD12864_START_FLAG == FALSE)
		{
			LCD12864_START_FLAG = TRUE;
			Lcd12864Display(0x80,(uint8_t *)"    数字模式    "); 
			Lcd12864Display(0x90,(uint8_t *)"设定值:         ");
			Lcd12864Display(0x88,(uint8_t *)"当前值:         ");
			Lcd12864Display(0x98,(uint8_t *)"状态：          ");
		}
		if(_DisplayState == GraphModeDisplayState && LCD12864_START_FLAG == FALSE)
		{
			
			Lcd12864Clean();
			
			for(i = 0; i < 128; i++)
			{
				OLED_DrawPoint(i, (u8)(63 - (0.64*PM2_5_table[i]))  ,1);
			}	
			for(i = 0; i < 32; i++)
			{
				OLED_DrawPoint(4*i+0, (u8)(64-0.016*SetVal)  ,1);
			}

			show_Pic(OLED_GRAM[0]);

			memset(OLED_GRAM, 0, 16*64);//清零
			delay_ms(1000);
			Lcd_Clear(0);
		}
		if(_DisplayState == ParameterSetDisplayState && LCD12864_START_FLAG == FALSE)
		{
			LCD12864_START_FLAG = TRUE;
			Lcd12864Display(0x80,(uint8_t *)"    参数设置    "); 
			Lcd12864Display(0x90,(uint8_t *)"设定值:         ");
			Lcd12864Display(0x88,(uint8_t *)"报警:           ");
			Lcd12864Display(0x98,(uint8_t *)"----------------");
		}
		if(GetDataFlag == 1)
		{
			GetDataFlag = 0;
			for(i = 20; i < 128; i++)
				PM2_5_table[i - 20] = PM2_5_table[i];
			
			for(i = 0; i < 20; i++)
				PM2_5_table[127 - 19 + i] = PM2_5_table_test[i];
		}
	}
}


	

void PeriphInit(void)
{

	LED_Init();						//LED 引脚初始化
	
	PM2_5_Led_Io_init();			//PM2.5传感器led灯初始化
	
	Key_Io_Init();					//按键io初始化
	
	Sound_Light_Alarm_Io_Init();	//声光报警io初始化
	
	Adc_Init();						//ADC初始化
	
	delay_init();					//定时器函数初始化（即systick初始化）
	
	TIM4_Init(719,99);				//定时器4初始化 定时时间 = [(719 + 1)*(99 + 1)]/72000000即1ms
	
	TIM3_Init(71,99);				//定时器3初始化 定时时间 = [(71 + 1)*(99 + 1)]/72000000即0.1ms
	
	Adc_Init();		  				//ADC初始化
	
	uart_init(115200);				//串口2初始化,波特率：115200
	
	Lcd12864init();					//LCD12864初始化
	
	Buzzer_Disable();				//蜂鸣器不使能
	
	Led_Disable();					//灯光不使能
}

uint8_t display_string[20] = {0}; //生成字符串

void display_state(void)		//显示状态机函数 
{
	static uint8_t ParameterSetDisplayFlag = 0;		//定义一个设置模式下标志位
	static uint8_t CallPoliceSetDisplayFlag = 0;	//定义一个设置模式下报警标志位
	switch(_DisplayState)
	{
		case InitDisplayState:						//初始目录界面
			{
				if((keyval & 1<<2) == 1<<2)
				{
					_DisplayState = (DisplayState)(_InitDispay - 1);
					if(_DisplayState == 0)
					{
						_DisplayState = (DisplayState)3;
					}
				}
					
				else if((keyval & 1<<1) == 1<<1)
				{
					switch(_InitDispay)
					{
						case NumModeDisplayState :
							Lcd12864String(0x99,0,(uint8_t *)"  ");
							Lcd12864String(0x9E,0,(uint8_t *)" ");
							Lcd12864String(0x91,0,(uint8_t *)" >");
							Lcd12864String(0x96,0,(uint8_t *)"<");
							_InitDispay =  GraphModeDisplayState; break;
						case GraphModeDisplayState :
							Lcd12864String(0x91,0,(uint8_t *)"  ");
							Lcd12864String(0x96,0,(uint8_t *)" ");
							Lcd12864String(0x89,0,(uint8_t *)" >");
							Lcd12864String(0x8E,0,(uint8_t *)"<");
							_InitDispay = ParameterSetDisplayState; break;
						case ParameterSetDisplayState : 
							Lcd12864String(0x89,0,(uint8_t *)"  ");
							Lcd12864String(0x8E,0,(uint8_t *)" ");
							Lcd12864String(0x99,0,(uint8_t *)" >");
							Lcd12864String(0x9E,0,(uint8_t *)"<");
							_InitDispay = NumModeDisplayState; break;
						default:break;
					}
				}
			}
			 break;
		case NumModeDisplayState:									//数字模式界面
			
		if((keyval & 1<<2) == 1<<2)									//检测按键长按操作
		{
			_DisplayState = InitDisplayState;						//回到初始界面
			_InitDispay = GraphModeDisplayState;
		}
		sprintf((char *)display_string,"%4d",SetVal);
		Lcd12864Display(0x94,(uint8_t *)display_string);
		sprintf((char *)display_string,"%4.1f  ",CurrentVal);
		Lcd12864Display(0x8C,(uint8_t *)display_string);
		if(CurrentVal > SetVal && CallPoliceSetDisplayFlag)			//当前PM2.5参数>设定值，并且报警模式打开
		{
			Lcd12864Display(0x9C,(uint8_t *)"报警");
			Buzzer_Enable();										//蜂鸣器使能
			Led_Enable();											//灯光使能
		}
		else
		{
			Lcd12864Display(0x9C,(uint8_t *)"正常");
			Buzzer_Disable();										//蜂鸣器不使能
			Led_Disable();											//灯光不使能
		}
		break;
		case GraphModeDisplayState: 								//图形模式界面
		if((keyval & 1<<2) == 1<<2)									//检测按键长按操作
		{
			_DisplayState = InitDisplayState;						//回到初始界面
			_InitDispay = GraphModeDisplayState;
			LCD12864_START_FLAG = TRUE;
		}
		if(CurrentVal > SetVal && CallPoliceSetDisplayFlag)			//当前PM2.5参数>设定值，并且报警模式打开
		{
			Buzzer_Enable();										//蜂鸣器使能
			Led_Enable();											//灯光使能
		}
		else
		{
			Buzzer_Disable();										//蜂鸣器不使能
			Led_Disable();											//灯光不使能
		}
		break;
		case ParameterSetDisplayState: 								//参数设置界面
		if((keyval & 1<<2) == 1<<2)     							//检测按键长按操作
		{
			_DisplayState = InitDisplayState;						//回到初始界面
			_InitDispay = GraphModeDisplayState;
		}
		if((keyval & 1<<1) == 1<<1)									//检测中间按键短按操作
		{
			ParameterSetDisplayFlag = ~ParameterSetDisplayFlag;
		}
		if(ParameterSetDisplayFlag)
		{
			if((keyval & 1<<3) == 1<<3)
				CallPoliceSetDisplayFlag = ~CallPoliceSetDisplayFlag;
			else if((keyval & 1<<4) == 1<<4)
				CallPoliceSetDisplayFlag = ~CallPoliceSetDisplayFlag;
			Lcd12864Display(0x97,(uint8_t *)"  ");
			Lcd12864Display(0x8F,(uint8_t *)"<-");

		}
		else
		{
			if((keyval & 1<<3) == 1<<3)								
				SetVal += 100;
			else if((keyval & 1<<4) == 1<<4)
				SetVal -= 100;
			if(SetVal>SETVAL_MAX)
				SetVal = SETVAL_MAX;
			if(SetVal<SETVAL_MIN)
				SetVal = SETVAL_MIN;

			Lcd12864Display(0x8F,(uint8_t *)"  ");
			Lcd12864Display(0x97,(uint8_t *)"<-");
		}
		sprintf((char *)display_string,"设定值: %4d",SetVal);
		Lcd12864Display(0x90,(uint8_t *)display_string);
		if(CallPoliceSetDisplayFlag)
			Lcd12864Display(0x8C,(uint8_t *)"关闭");
		else
			Lcd12864Display(0x8C,(uint8_t *)"打开");
		break;
		default :break;
	}
	
}

//定时器4中断服务程序

u16 Time500Ms = 0;
u16 Time50Ms = 0;
u16 Time10Ms = 0;
u8	GetDataNum = 0;
uint8_t  data_test[10] = {0};

double sort_table[10] = {0};
void TIM4_IRQHandler(void)   //TIM4中断
{
	int i = 0, j = 0;
	if(TIM4->SR&0X0001)//1ms定时中断
 {
	 if((Time500Ms++)==500)						//500ms定时
	 {
		 Time500Ms=0;
		 led=~led;
		 printf("pm2.5 = %f\n", CurrentVal);	//串口打印数据,usart1
		 
	 }
	 FuncTimeTravers_ISR();						//回调函数接口
	 if(DISPLAY_STATE_FLAG == TRUE)				//显示是否使能
	{
		if((Time10Ms++) == 10)					//10ms定时
		{
		 Time10Ms = 0;
		 keyval = Key_Event_Handler();			//获取按键事件处理函数
		 display_state();						//显示状态机刷新
		}
	 
		if(++Time50Ms == 50)
		{
			Time50Ms = 0;
			sort_table[GetDataNum] = (ADC_Val[0] > 72.91? (ADC_Val[0]/2048.0 - 0.0356)*120000*0.035 :  ADC_Val[0]); //获取数据
			data_test[GetDataNum] = (uint8_t)(sort_table[GetDataNum] / 40.0);
			if(data_test[GetDataNum] > 90)
				data_test[GetDataNum] = 90;
			 if(++GetDataNum  == 10)
			 {
				 GetDataNum = 0;
				 for(i = 0; i < 10; i++)							//数据更新
				 {
					qsort(sort_table,10,sizeof(sort_table[0]),cmp); //数据排序
					for(j = 3; j < 7; j++)
					 {
						CurrentVal +=  sort_table[j]; 				//累加
					 }
					 CurrentVal  /= 4; 								//求平均值
					PM2_5_table_test[i * 2 + 1] = data_test[i]; 
					PM2_5_table_test[i * 2] = data_test[i];
				 }
				GetDataFlag = 1;
			 }
		}
	 }
 }
 TIM4->SR&=~(1<<0);  //中断标志位清0，等待下一次中断
}



/******************************************************************************************************

1KHz-pm2.5专用

******************************************************************************************************/
u16 Time500us = 0;
void TIM3_IRQHandler(void)   //TIM3中断 //100us中断
{
	if(TIM3->SR&0X0001)//1ms定时中断
 {
	 if((Time500us++)==5)
	 {
		 Time500us=0;
		 PM2_5_Led=~PM2_5_Led; 
	 }
 }
 TIM3->SR&=~(1<<0);  //中断标志位清0，等待下一次中断
}







int cmp(const void *a,const void *b)
{
    return *(double *)a-*(double *)b;//这是从小到大排序，若是从大到小改成： return *(int *)b-*(int *)a;
}

