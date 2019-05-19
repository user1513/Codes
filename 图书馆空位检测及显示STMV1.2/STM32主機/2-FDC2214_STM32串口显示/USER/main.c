#include "main.h"


bool uart3flag=FALSE;//����3���՘�־λ
bool uart3handflag=FALSE;//����3���՘�־λ
bool touchflag=FALSE;    //�|����־λ
bool ExtinguishingScreenflag=FALSE;   //Ϩ����־λ
bool Screensaverflag=FALSE;   //������־λ+�س�ʼҳ��
bool ScreenTimeflag=FALSE;  //����ʱ���־λ

pDATALIST pHard=NULL;//����ͷ�ڵ�


u8 Receivebufferarea[22];//Ĭ�J���x3*7�Ĕ����惦�^
u16 ReceivebufferareaVal=1;//�������܂���


u8 TestKeyVal=0;  //�����水�I�둪
u8 UsartData[12]; //���ګ@ȡ����

void ScreeLight(u32 _timer);//����һ�������ʱ��
void Sys_Init(void);
void send1(void);
int main(void)
{
	pHard = CreateListHard();
	Sys_Init();
	while(1)
	{
		if(UsartData[11]==0xAA&&TestKeyVal!=0)//�ж���������������
		{
			UsartData[11]=0x00;
			Receivebufferarea[0]=0;//��ʼ����һ��׃��
			ReceivebufferareaVal=1;//���³�ʼ�����Ղ���
			uart3flag=TRUE;
			uart3handflag=TRUE;
			LightSet(0x40);
			FuncReg(&uart3flag,500,FALSE);
			FuncReg(&ScreenTimeflag,10,FALSE);
			send1();
		} 
		if(touchflag==TRUE)					//��������������
		{
			UsartData[11]=0x00;
			LightSet(0x40);
			touchflag=FALSE;
			FuncReg(&ScreenTimeflag,10,FALSE);
			FuncReg(&Screensaverflag,10000,TRUE);
			FuncReg(&ExtinguishingScreenflag,30000,TRUE);
		}
		if(uart3handflag==TRUE&&uart3flag==FALSE)//�����ӽڵ㣬��������
		{
			FuncDataGet(Receivebufferarea);
			isSend(pHard);
			uart3handflag=FALSE;
			memset(Receivebufferarea + 1,0, sizeof(Receivebufferarea) - sizeof(u8));
			Send_Page_Switch(0x03,0x06);
			FuncReg(&Screensaverflag,10000,TRUE);
			FuncReg(&ExtinguishingScreenflag,30000,TRUE);
		}
		if(Screensaverflag==TRUE)				//������ʼ
		{
			Screensaverflag=FALSE;
			Send_Page_Switch(0x03,0x00);
			delay_ms(10);
			FuncReg(&ScreenTimeflag,10,TRUE);
		}
		if(ScreenTimeflag == TRUE&&ExtinguishingScreenflag==FALSE)//��������
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
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration();//����NVIC�жϷ���2:2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
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
void ScreeLight(u32 _timer)//����һ�������ʱ��
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


//��ʱ��5�жϷ������

u16 Time500Ms;
void TIM4_IRQHandler(void)   //TIM5�ж�
{
	if(TIM4->SR&0X0001)//20ms��ʱ�ж�
 {
	 if((Time500Ms++)==500)
	 {
		 Time500Ms=0;
		 led=~led; 
	 }
	 FuncTimeTravers();
 }
 TIM4->SR&=~(1<<0);  //�жϱ�־λ��0���ȴ���һ���ж�
}
