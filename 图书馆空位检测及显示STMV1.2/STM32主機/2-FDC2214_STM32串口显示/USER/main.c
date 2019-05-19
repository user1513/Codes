#include "main.h"


bool uart3flag=FALSE;//串口3接收標志位
bool uart3handflag=FALSE;//串口3接收標志位
bool touchflag=FALSE;    //觸摸標志位
bool ExtinguishingScreenflag=FALSE;   //熄屏標志位
bool Screensaverflag=FALSE;   //屏保標志位+回初始页面
bool ScreenTimeflag=FALSE;  //屏保时间标志位

pDATALIST pHard=NULL;//创建头节点


u8 Receivebufferarea[22];//默認定義3*7的數據存儲區
u16 ReceivebufferareaVal=1;//數據接受個數


u8 TestKeyVal=0;  //主界面按鍵想應
u8 UsartData[12]; //串口獲取數據

void ScreeLight(u32 _timer);//设置一个亮灭的时间
void Sys_Init(void);
void send1(void);
int main(void)
{
	pHard = CreateListHard();
	Sys_Init();
	while(1)
	{
		if(UsartData[11]==0xAA&&TestKeyVal!=0)//判读串口屏接收数据
		{
			UsartData[11]=0x00;
			Receivebufferarea[0]=0;//初始化第一個變量
			ReceivebufferareaVal=1;//重新初始化接收個數
			uart3flag=TRUE;
			uart3handflag=TRUE;
			LightSet(0x40);
			FuncReg(&uart3flag,500,FALSE);
			FuncReg(&ScreenTimeflag,10,FALSE);
			send1();
		} 
		if(touchflag==TRUE)					//串口屏触摸函数
		{
			UsartData[11]=0x00;
			LightSet(0x40);
			touchflag=FALSE;
			FuncReg(&ScreenTimeflag,10,FALSE);
			FuncReg(&Screensaverflag,10000,TRUE);
			FuncReg(&ExtinguishingScreenflag,30000,TRUE);
		}
		if(uart3handflag==TRUE&&uart3flag==FALSE)//接收子节点，并处理发送
		{
			FuncDataGet(Receivebufferarea);
			isSend(pHard);
			uart3handflag=FALSE;
			memset(Receivebufferarea + 1,0, sizeof(Receivebufferarea) - sizeof(u8));
			Send_Page_Switch(0x03,0x06);
			FuncReg(&Screensaverflag,10000,TRUE);
			FuncReg(&ExtinguishingScreenflag,30000,TRUE);
		}
		if(Screensaverflag==TRUE)				//屏保开始
		{
			Screensaverflag=FALSE;
			Send_Page_Switch(0x03,0x00);
			delay_ms(10);
			FuncReg(&ScreenTimeflag,10,TRUE);
		}
		if(ScreenTimeflag == TRUE&&ExtinguishingScreenflag==FALSE)//屏保处理
		{
			ScreeLight(3000);
		}
		if(ExtinguishingScreenflag == TRUE)
		{
			ScreenTimeflag =  FALSE;
			FuncReg(&ScreenTimeflag,10,FALSE);
			ExtinguishingScreenflag = FALSE;
			LightSet((u8)0);
		}
	}
}

void Sys_Init(void)
{
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration();//设置NVIC中断分组2:2；2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	Usart2_Init(115200);
	Usart3_Init(9600);
	LED_Init();
	led=0;
	TIM4_Ctrl_Init(72-1,1000-1);
	delay_ms(250);
	LightSet(0x40);
	delay_ms(250);
	SendListInit(3,0);
	FuncReg(&Screensaverflag,10000,TRUE);
	FuncReg(&ExtinguishingScreenflag,30000,TRUE);
}

void send1(void)
{
	u8 _data[6];
	_data[0]=0x55;
	_data[1]=0xAA;
	_data[2]=0x55;
	_data[3]=0x55;
	_data[4]=0x55;
	_data[5]=0xFF;
	Usart3_String_Send1(_data,6);
}

int lighttemp=0X40;

u8 lightdemo=0;
void ScreeLight(u32 _timer)//设置一个亮灭的时间
{
	
	u32 _timertemp = 0;
	_timertemp =_timer /(0x40);
	ScreenTimeflag=FALSE;
	FuncReg(&ScreenTimeflag,_timertemp,TRUE);
	if(lighttemp>=0x40)
	{
		lightdemo = 1;
	}
	if(lighttemp<=0x00)
	{
		lightdemo = 0;
	}
	if(lightdemo == 1)
	{
		lighttemp--;
	}
	if(lightdemo != 1)
	{
		lighttemp++;
	}
	LightSet((u8)lighttemp);
}


//定时器5中断服务程序

u16 Time500Ms;
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
