#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "fdc2214.h"
#include "fdc2214t.h"
#include "McuDataSend.h"
#include "_usart_send.h"
#include "usart2.h"
#include "main.h"
#include "sortfunc.h"



/////////////////demo

float arr[10],arr1[10],arr2[10],arr3[10],arr4[10],arr5[10];//采样数据
////////////////demo

extern u8 UsartData[];

float ResDemo[6];//上电数据值


float res[6];//数据值
float temp[6];//上电数据值
//float ParameterDataDemo[6]={0.300,0.300,0.300,0.300,0.300,0.300};
float Parameter_Set[6]={0.75,0.75,0.75,0.75,0.75,1.5};//参数设置

u8 KeyFlag=Disable,JsKeyFlag=Disable;
u8 TestModeState=GUESSINGVERDICT;  //测试模式状态值
u8 PageNumState=STONE;			   //切换页面状态值

u8 TestKeyVal=0x00;                //串口屏（0X01猜拳按键值 0X02划拳按键值）
u8 LearnKeyVal=0x00;               //串口屏（学习模式下键值）
u8 CompetitionKeyVal=0x00;         //竞赛模式按键

float buff1;

void GetData_Handle(float *table );
u8 GetKeyVal(float *table );
void Sys_Init(void);
void LearnModel_Demo(u8 LearnModelVal);
u8 LearnModel_State(void);
void DataHandle(float *temp);
void CompetitionMode(float *table);
void Competition_vs(u8 j1,u8 y1);

int main(void)
{
	u8 buff;
	
	Sys_Init();
	while(1)
	{
		printf("CH0;%3.3f CH1;%3.3f CH2;%3.3f CH3;%3.3f CH4;%3.3f CH5;%3.3f\r\n",res[0],res[1],res[2],res[3],res[4],res[5]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		if(UsartData[11]==0xAA)
		{
			UsartData[11]=0X00;
			KeyFlag=Enable;
		}
		if(UsartData[11]==0xBB)
		{
			UsartData[11]=0X00;
			JsKeyFlag=Enable;
		}
		GetData_Handle(res);
		CompetitionMode(res);
		buff=LearnModel_State();
		LearnModel_Demo(buff);
		
	}
}

void Sys_Init(void)
{
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration();//设置NVIC中断分组2:2；2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	Usart2_Init(115200);
	LED_Init();
		FDC_IIC_Init();
	while(FDC2214_Init());
	FDCt_IIC_Init();
	while(FDC2214t_Init());
	led=0;
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	
//	temp[0] = Cap_Calculate(0);//读取初始值
//	temp[1] = Cap_Calculate(1);
//	temp[2] = Cap_Calculate(2);
//	temp[3] = Cap1_Calculate(0);
//	temp[4] = Cap1_Calculate(1);//读取初始值
//	temp[5] = Cap1_Calculate(2);
	DataHandle(temp);
	Send_China_Demo();
	TIM4_Ctrl_Init(2000-1,720-1);
	
	
}



void DataHandle(float *temp)
{
	int i=0;
	for(i=0;i<10;i++)
	{
		arr[i]=Cap_Calculate(0);
		arr1[i]=Cap_Calculate(1);
		arr2[i]=Cap_Calculate(2);
		arr3[i]=Cap1_Calculate(0);
		arr4[i]=Cap1_Calculate(1);
		arr5[i]=Cap1_Calculate(2);
	}
	
	bubbleSort_super(arr,4);
	temp[0]=EliminateFunc(arr,4,4);
	bubbleSort_super(arr1,4);
	temp[1]=EliminateFunc(arr1,4,4);
	bubbleSort_super(arr2,4);
	temp[2]=EliminateFunc(arr2,4,4);
	bubbleSort_super(arr3,4);
	temp[3]=EliminateFunc(arr3,4,4);
	bubbleSort_super(arr4,4);
	temp[4]=EliminateFunc(arr4,4,4);
	bubbleSort_super(arr5,4);
	temp[5]=EliminateFunc(arr5,4,4);

}

u8 LearnModel_State(void)
{
	u8 res1=0;
	if(LearnKeyVal&0x01) {
	res1=SCISSORSLEARN;LearnKeyVal &=~0x01;
	Send_China_Data(0x0100,"学习中  ");Send_Page_Switch(0x03,0x0013);}
	if(LearnKeyVal&0x02) {
	res1=STONELEARN;LearnKeyVal &=~0x02;
	Send_China_Data(0x0104,"学习中  ");Send_Page_Switch(0x03,0x0013);}
	if(LearnKeyVal&0x04) {
	res1=CLOTHLEARN;LearnKeyVal &=~0x04;
	Send_China_Data(0x0108,"学习中  ");Send_Page_Switch(0x03,0x0013);}
	if(LearnKeyVal&0x08) {
	res1=FIRSTLEARN;LearnKeyVal &=~0x08;
	Send_China_Data(0x010C,"学习中  ");Send_Page_Switch(0x03,0x0014);}
	if(LearnKeyVal&0x10) {
	res1=SECONDLEARN;LearnKeyVal &=~0x10;
	Send_China_Data(0x0110,"学习中  ");Send_Page_Switch(0x03,0x0014);}
	if(LearnKeyVal&0x20) {
	res1=THIRDLEARN;LearnKeyVal &=~0x20;
	Send_China_Data(0x1000,"学习中  ");Send_Page_Switch(0x03,0x0014);}
	if(LearnKeyVal&0x40) {
	res1=FOURTHLEARN;LearnKeyVal &=~0x40;
	Send_China_Data(0x1004,"学习中  ");Send_Page_Switch(0x03,0x0014);}
	if(LearnKeyVal&0x80) {
	res1=FIFTHLEARN;LearnKeyVal &=~0x80;
	Send_China_Data(0x011C,"学习中  ");Send_Page_Switch(0x03,0x0014);}
	return res1;
}

void LearnModel_Demo(u8 LearnModelVal)
{
	u8 KeyVal=0;
	switch (LearnModelVal)
	{
		case SCISSORSLEARN:
			while(KeyVal!=3)
			{
				KeyVal=GetKeyVal(res);
			}
			Send_China_Data(0x0100,"学习成功");Send_Page_Switch(0x03,0x0013);
			break;
		case STONELEARN:
			while(KeyVal!=1)
			{
				KeyVal=GetKeyVal(res);
			}
			Send_China_Data(0x0104,"学习成功");Send_Page_Switch(0x03,0x0013);
		break;
		case CLOTHLEARN:
			while(KeyVal!=6)
			{
				KeyVal=GetKeyVal(res);
			}
			Send_China_Data(0x0108,"学习成功");Send_Page_Switch(0x03,0x0013);
			break;
		case FIRSTLEARN:
			while(KeyVal!=2)
			{
				KeyVal=GetKeyVal(res);
			}
			Send_China_Data(0x010C,"学习成功");Send_Page_Switch(0x03,0x0014);
			break;
		case SECONDLEARN:
			while(KeyVal!=3)
			{
				KeyVal=GetKeyVal(res);
			}
			Send_China_Data(0x0110,"学习成功");delay_ms(100);Send_Page_Switch(0x03,0x0014);
			break;
		case THIRDLEARN:
			while(KeyVal!=4)
			{
				KeyVal=GetKeyVal(res);
			}
			Send_China_Data(0x1000,"学习成功");delay_ms(100);Send_Page_Switch(0x03,0x0014);
			break;
		case FOURTHLEARN:
			while(KeyVal!=5)
			{
				KeyVal=GetKeyVal(res);
			}
			Send_China_Data(0x1004,"学习成功");delay_ms(100);Send_Page_Switch(0x03,0x0014);
			break;
		case FIFTHLEARN:
			while(KeyVal!=6)
			{
				KeyVal=GetKeyVal(res);
			}
			Send_China_Data(0x011C,"学习成功");delay_ms(100);Send_Page_Switch(0x03,0x0014);
			break;
		default :break;
	}
}



u8 GetKeyVal(float *table )
{
	u8 i=0;
	
	ResDemo[0]=*table;
	if((*table)>Parameter_Set[0]) ++i;
	ResDemo[1]=*(table+1);
	if((*(table+1))>Parameter_Set[1])++i;
	ResDemo[2]=*(table+2);
	if((*(table+2))>Parameter_Set[2])++i;
	ResDemo[3]=*(table+3);
	if((*(table+3))>Parameter_Set[3])++i;
	ResDemo[4]=*(table+4);
	if((*(table+4))>Parameter_Set[4])++i;
	ResDemo[5]=*(table+5);
	if((*(table+5))>Parameter_Set[5])++i;
	return i;
}

u8 KeyVal_Check_val=0;//次数
int KeyVal_Check_time=0;//倒计时时间
u8 KeyValtime_Flag=Disable;//使能倒计时功能
/*
u8 Current_value=0;   //当前数据
u8 Last_value=0;      //上一次数据
u8 KeyVal_Check(u8 Time,u8 Num,float *table )
{

	
	switch(KeyVal_Check_val)
	{
		case 1:Last_value=GetKeyVal(table);KeyVal_Check_val++;KeyVal_Check_time=5;break;
		case 2:KeyValtime_Flag=Enable;break;
		case 3:Current_value=GetKeyVal(table);
		if(Last_value==Current_value) {KeyVal_Check_val++;KeyVal_Check_time=5};
		else KeyVal_Check_val=0;break;
		case 4:KeyValtime_Flag=Enable;break;
	}
	
	
	if(GetKeyVal
	
	
	
}
*/
u8 jiaf,yif;
u8 CompetitionState=0;
void CompetitionMode(float *table)
{
	u8 KeyVal=0;
	if(JsKeyFlag==Enable&&CompetitionState==0)
	{
		Send_China_Data(0x0300,"甲方请出拳");
		Send_China_Data(0x0310,"乙方请等待");
		Send_China_Data(0x0320,"         ");
		KeyVal=GetKeyVal(table);
		while(KeyVal==0)
		{
			KeyVal=GetKeyVal(table);
		}
			KeyVal_Check_time=7;
			KeyValtime_Flag=Enable;
			while(KeyVal_Check_time!=0);
		
			KeyVal=GetKeyVal(table);
			jiaf=KeyVal;
			switch(KeyVal)
			{
				case 0x01:Send_China_Data(0x0300,"   拳头   ");CompetitionState =1;break;
				case 0x03:Send_China_Data(0x0300,"   剪刀   ");CompetitionState =1;break;
				case 0x06:Send_China_Data(0x0300,"    布    ");CompetitionState =1;break;
				default :KeyFlag=Enable; break;
			}
			while(KeyVal!=0)
			{
				KeyVal=GetKeyVal(table);
			}
				
	}
	
	if(CompetitionState==1)
	{
		Send_China_Data(0x0310,"乙方请出拳");
		KeyVal=GetKeyVal(table);
		while(KeyVal==0)
		{
			KeyVal=GetKeyVal(table);
		}
			KeyVal_Check_time=7;
			KeyValtime_Flag=Enable;
			while(KeyVal_Check_time!=0);
		
			KeyVal=GetKeyVal(table);
			yif=KeyVal;
			switch(KeyVal)
			{
				case 0x01:Send_China_Data(0x0310,"    拳头  ");CompetitionState =0;break;
				case 0x03:Send_China_Data(0x0310,"    剪刀  ");CompetitionState =0;break;
				case 0x06:Send_China_Data(0x0310,"     布   ");CompetitionState =0;break;
				default :KeyFlag=Enable; break;
			}
				while(KeyVal!=0)
			{
				KeyVal=GetKeyVal(table);
			}
			
			Competition_vs(jiaf,yif);
		JsKeyFlag=Disable;
	}
	
}

void Competition_vs(u8 j1,u8 y1)
{
	if(j1==y1) Send_China_Data(0x0320,"  平局  ");
	else if((j1==1)&&(y1==3)) Send_China_Data(0x0320,"甲方获胜");
	else if((j1==1)&&(y1==6)) Send_China_Data(0x0320,"乙方获胜");
	else if((j1==3)&&(y1==1)) Send_China_Data(0x0320,"乙方获胜");
	else if((j1==3)&&(y1==6)) Send_China_Data(0x0320,"甲方获胜");
	else if((j1==6)&&(y1==1)) Send_China_Data(0x0320,"甲方获胜");
	else if((j1==6)&&(y1==3)) Send_China_Data(0x0320,"乙方获胜");
}

void GetData_Handle(float *table )
{
	u8 KeyVal=0;
	if(KeyFlag==Enable)
	{
		KeyVal=GetKeyVal(table);
		while(KeyVal==0)
		{
			KeyVal=GetKeyVal(table);
		}
			KeyVal_Check_time=7;
			KeyValtime_Flag=Enable;
			while(KeyVal_Check_time!=0);
		
			KeyVal=GetKeyVal(table);
		KeyFlag=Disable;
		if(TestKeyVal==GUESSINGVERDICT)
		{
			switch(KeyVal)
			{
				case 0x01:Test_result(GUESSINGVERDICT,STONE );Test_result(GUESSINGVERDICT,STONE );TestKeyVal=0x00;break;
				case 0x03:Test_result(GUESSINGVERDICT,SCISSORS  );Test_result(GUESSINGVERDICT,SCISSORS  );TestKeyVal=0x00;break;
				case 0x06:Test_result(GUESSINGVERDICT,CLOTH  );Test_result(GUESSINGVERDICT,CLOTH  );TestKeyVal=0x00;break;
				default :KeyFlag=Enable; break;
			}
		}
		else if(TestKeyVal==VERDICTPUNCH)
		{
			switch(KeyVal)
			{
				case 0x02:Test_result(VERDICTPUNCH,FIRST);Test_result(VERDICTPUNCH,FIRST);TestKeyVal=0x00;break;
				case 0x03:Test_result(VERDICTPUNCH,SECOND);Test_result(VERDICTPUNCH,SECOND);TestKeyVal=0x00;break;
				case 0x04:Test_result(VERDICTPUNCH,THIRD);Test_result(VERDICTPUNCH,THIRD);TestKeyVal=0x00;break;
				case 0x05:Test_result(VERDICTPUNCH,FOURTH);Test_result(VERDICTPUNCH,FOURTH);TestKeyVal=0x00;break;
				case 0x06:Test_result(VERDICTPUNCH,FIFTH);Test_result(VERDICTPUNCH,FIFTH);TestKeyVal=0x00;break;
				default :KeyFlag=Enable; break;
			}
		}
		
	}
	
}



//定时器5中断服务程序

u8 Time500Ms;
u8 Time300Ms;
void TIM4_IRQHandler(void)   //TIM5中断
{
	if(TIM4->SR&0X0001)//20ms定时中断
 {
	 DataHandle(res);
	res[0] = res[0]-temp[0];//采集数据
	res[1] = res[1]-temp[1];
	res[2] = res[2]-temp[2];
	res[3] = res[3]-temp[3];
	res[4] = res[4]-temp[4];//采集数据
	res[5] = res[5]-temp[5];
//SendFdc2214Data();
//	Data_Send_Status(res,0,0);
//	printf("%5d : %5d : %5d\r\n",res0,res1,res2);
 }
 
 if(KeyValtime_Flag==Enable)
 {
	KeyVal_Check_time--;
		if(KeyVal_Check_time<0)
		{
			KeyVal_Check_time=0;
			KeyValtime_Flag=Disable;
		}	
 }
 if((Time300Ms++)==15)
 {
	 Time300Ms=0;
	 SendFdc2214Data();
 }
 if((Time500Ms++)==25)
 {
	 Time500Ms=0;
	 led=~led;
	 
 }
 TIM4->SR&=~(1<<0);  //中断标志位清0，等待下一次中断
}
