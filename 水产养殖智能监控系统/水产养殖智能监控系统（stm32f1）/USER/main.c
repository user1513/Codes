#include "main.h"

int DataSave[6]={0,0,0,10,85,10};// 1、确定 2、切换 3、确定 4-6传感器设定数据
u8 SprintfSave[16];//由于12864显示一行需要16个字节
bool FeedingStartFlag = FALSE;
bool FeedingStopFlag = FALSE;
bool FeedingLoopFlag = FALSE;
bool YJ12864DisplayFlag = FALSE;//lcd12864初始化显示
bool EquipmentWorkFlag = FALSE;
bool LCD12864CleanFlag =FALSE;

bool SetLoopFlag = TRUE;
bool PreheatEndFlag = FALSE; //预热结束，开始进行数值判断 
bool SwitchStartFlag = TRUE;
bool PumpSwitchingFlag = FALSE;
bool SwitchEndFlag = FALSE;
#define LCD12864INITSWITCHTIME 3000 //lcd12864初始页面切换时间
#define LCD12864HEATTOWORKTIME 10000//lcd12864显示加热到工作界面切换时间
#define LCD12864LOOPWORKTIME  250   //lcd12864循环工作时间
#define SETLOOPWORKTIME  250   //lcd12864循环工作时间
#define PumpSwitchingTime  10000   //lcd12864循环工作时间
#define SwitchEndTime  10000   //lcd12864循环工作时间

void PeriphInit(void);

int main(void)
{
	PeriphInit();
	//FuncReg(&FeedingStartFlag,10000,TRUE);
	while(1)
	{
		KeyHandle();
		if(PreheatEndFlag == TRUE && DataSave[0] != 1)
		{
			if(DataSave[3]<((ADC_Val[0]*100.0)/4096))
			{
				FeedingStartFlag = TRUE;
				FeedingLoopFlag = TRUE;
			}
			else
			{
				FeedingStartFlag = FALSE;
				CleanCallbackFunc(&FeedingLoopFlag,FALSE);
			}
			
			if(FeedingLoopFlag == TRUE && FeedingStartFlag == TRUE)
			{
				feed = ~feed;
				FeedingLoopFlag = FALSE;
				FuncReg(&FeedingLoopFlag,5000,TRUE);		
			}
			if((DataSave[4]<(100-((ADC_Val[1]*100.0)/4096)))&&SwitchStartFlag == TRUE)
			{
				SwitchStartFlag = FALSE;
				jdq1Absorption();
				jdq2ToBreakOff();
				FuncReg(&PumpSwitchingFlag,PumpSwitchingTime,TRUE);
			}
			if(PumpSwitchingFlag == TRUE)
			{
				PumpSwitchingFlag = FALSE;
				jdq2Absorption();
				jdq1ToBreakOff();
				FuncReg(&SwitchEndFlag,SwitchEndTime,TRUE);
			}
			if(SwitchEndFlag == TRUE)
			{
				SwitchEndFlag = FALSE;
				jdq2ToBreakOff();
				jdq1ToBreakOff();
				SwitchStartFlag = TRUE;
			}
		}
		

		if(DataSave[0] == 1 && SetLoopFlag == TRUE)
		{
			SetLoopFlag = FALSE;
			FuncReg(&SetLoopFlag,SETLOOPWORKTIME,TRUE);
			CleanCallbackFunc(&EquipmentWorkFlag,TRUE);
			Lcd12864Clean(2);
			LCD12864CleanFlag = TRUE;
			Lcd12864Display(0x80,"水产养殖监控SET "); 
			sprintf((char *)SprintfSave,"气体值:%3d%%",DataSave[3]);
			Lcd12864String(1,0,SprintfSave);
			sprintf((char *)SprintfSave,"浊度值:%3d%%",DataSave[4]);
			Lcd12864String(2,0,SprintfSave);
			sprintf((char *)SprintfSave,"P H 值:%3d%%",DataSave[5]);
			Lcd12864String(3,0,SprintfSave);
			switch(DataSave[1])
			{
				case 0: 
					Lcd12864String(1,6,"<--");	
					Lcd12864String(2,6,"   ");	
					Lcd12864String(3,6,"   ");	
					break;
				case 1:
					Lcd12864String(1,6,"   ");	
					Lcd12864String(2,6,"<--");	
					Lcd12864String(3,6,"   ");
					break;
				case 2:
					Lcd12864String(1,6,"   ");	
					Lcd12864String(2,6,"   ");	
					Lcd12864String(3,6,"<--");
					break;
				default :break;
			}
		}
		if(YJ12864DisplayFlag == TRUE && DataSave[0] == 0)
		{
			YJ12864DisplayFlag = FALSE;
			Lcd12864Display(0x80,"水产养殖监控系统"); 
			Lcd12864Display(0x90,"气体值: 设备预热");
			Lcd12864Display(0x88,"浊度值: 设备预热");
			Lcd12864Display(0x98,"P H 值: 设备预热");
			FuncReg(&EquipmentWorkFlag,LCD12864HEATTOWORKTIME,TRUE);
		}
		if(EquipmentWorkFlag == TRUE && DataSave[0] == 0)
		{
			Lcd12864Clean(1);
			CleanCallbackFunc(&SetLoopFlag,TRUE);
			PreheatEndFlag = TRUE;
			EquipmentWorkFlag = FALSE;
			Lcd12864Display(0x80,"水产养殖监控系统"); 
			sprintf((char *)SprintfSave,"气体值:%6.2lf%%",(ADC_Val[0]*100.0)/4096);
			Lcd12864String(1,0,SprintfSave);
			sprintf((char *)SprintfSave,"浊度值:%6.2lf%%",100-(ADC_Val[1]*100.0)/4096);
			Lcd12864String(2,0,SprintfSave);
			sprintf((char *)SprintfSave,"P H 值:%6.2lf%%",(ADC_Val[2]*100.0)/4096);
			Lcd12864String(3,0,SprintfSave);
			FuncReg(&EquipmentWorkFlag,LCD12864LOOPWORKTIME,TRUE);
		}
	}
}

void PeriphInit(void)
{
	IoInit();
	delay_init();
	TIM4_Ctrl_Init(719,99);
	Adc_Init();
	Usart2_Init(115200);
	Lcd12864init();
	delay_ms(1000);
	Lcd12864Display(0x80,"   ~欢迎使用~   "); 
	Lcd12864Display(0x90,"----水产养殖----");
	Lcd12864Display(0x88,"  智能监控系统  ");
	Lcd12864Display(0x98,"            --YG");
	FuncReg(&YJ12864DisplayFlag,LCD12864INITSWITCHTIME,TRUE);
}

//定时器5中断服务程序

u16 Time500Ms = 0;
void TIM4_IRQHandler(void)   //TIM5中断
{
	if(TIM4->SR&0X0001)//20ms定时中断
 {
	 if((Time500Ms++)==500)
	 {
		 Time500Ms=0;
		 led=~led; 
	 }
	 FuncTimeTravers();
 }
 TIM4->SR&=~(1<<0);  //中断标志位清0，等待下一次中断
}
