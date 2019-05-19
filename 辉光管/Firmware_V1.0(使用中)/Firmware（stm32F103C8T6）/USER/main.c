#include "main.h"

#define YEARDISPLAYTIME 2000 //��������ʾʱ��
#define DATEDISPLAYTIME 5000 //ʱ������ʾʱ��
#define SETFLICKERTIME  500  //������˸Ƶ��
#define KEYVALTIME       10  //��ֵ��ȡʱ��
#define TicktockTIME     1000  //�δ�ʱ��

int SetTime[7]={40,50,16,7,23,11,18};//��-��-ʱ/����/��-��-��
int GetTime[7];//��-��-ʱ/����/��-��-��

bool FIRMWAREDISPLAYFLAG = TRUE; //�Թ����ʾ��־λ
bool DS3231GETTIMEFLAG = TRUE;   //DS3231��ȡʱ���־λ
bool YEARDISFLAG = TRUE;         //������ʱ����ʾ������־λ
bool YEARDISTEMPFLAG = TRUE;     //����������ʱ����ʾ������־λ
bool SETFLICKERFLAG = TRUE;      //������˸��־λ
bool KEYGETFLAG = TRUE;     	 //��ֵ��־λ
bool TICKTOCKFLAG = TRUE;		 //�δ��־λ
int main(void)
{
	gps_process_data gps;     //GPS�ṹ��
	unsigned char GPS_Data_Temp[256];
	u8 GpsKeyFlag = 0, tempFlag = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	gps_io_cfg();
	key_io_config();
	delay_init();
	LED_Init();
	TIM4_Ctrl_Init(72-1,1000-1);
	IIC_Init();
	Usart2_Init(38400);
	//Ds3231_Init();
	//DS3231_SetTime(SetTime);
	FirmwarePeriphCfg();
	while(1)
	{
		if(KEYGETFLAG == TRUE)
		{
			Get_Key_Val(GetTime);
			KEYGETFLAG = FALSE;
			FuncReg(&KEYGETFLAG, KEYVALTIME, TRUE);
		}
		if(YEARDISFLAG == TRUE && YEARDISTEMPFLAG == TRUE) // ���ƻԹ����ʾ���ں�ʱ��ı���������ͨ���޸�main.c�ϲ��궨��
		{
			YEARDISTEMPFLAG = FALSE;
			FuncReg(&YEARDISFLAG, YEARDISPLAYTIME, FALSE);
		}
		else if(YEARDISFLAG == FALSE && YEARDISTEMPFLAG == FALSE)
		{
			YEARDISTEMPFLAG = TRUE;
			FuncReg(&YEARDISFLAG, DATEDISPLAYTIME, TRUE);
		}
		if(FIRMWAREDISPLAYFLAG == TRUE && keystate[0] == 0)                   // ÿ��1ms����һ�»Թ���ʾ����
		{
			FIRMWAREDISPLAYFLAG = FALSE;
			FuncReg(&FIRMWAREDISPLAYFLAG, 1, TRUE);
			if(YEARDISFLAG == TRUE)						  // ������ʾ������ʱ����ʾ����
				displayTime(GetTime + 4, 4);
			else
				displayTime(GetTime, 4);
		}
		if(DS3231GETTIMEFLAG == TRUE && keystate[0] == 0)                    //ÿ��500ms����һ��DS3231��ȡʱ��ĺ���
		{
			DS3231GETTIMEFLAG = FALSE;
			FuncReg(&DS3231GETTIMEFLAG, 500, TRUE);
			DS3231_GetTime(GetTime);
		}
		if(SETFLICKERFLAG == TRUE)
		{
			SETFLICKERFLAG = FALSE;
			tempFlag = ~tempFlag;
			FuncReg(&SETFLICKERFLAG , SETFLICKERTIME , TRUE);
		}
		if(keystate[0] == 0x01 )
		{	
			if(tempFlag == 0x00)
			{
				FirmwareShift(keystate[1], GetTime, TRUE);
			}
			else
			{
				FirmwareShift(keystate[1], GetTime, FALSE);
			}
		}
		if(TICKTOCKFLAG == TRUE && keystate[0] == 0)
		{
			TICKTOCKFLAG = FALSE;
			DIDT = ~DIDT;
			FuncReg(&TICKTOCKFLAG , TicktockTIME , TRUE);
		}
		if(GPSKEY == 0 && GpsKeyFlag != 0 )
		{
			GPSENABLE();
			if(Gps_Data_writ(&gps) == TRUE)
			{
				GpsKeyFlag = 0;
			}
			if(GPS_Stop_flag==1)                    //��ȡGPS����
			{
				GPS_Stop_flag=0;
				strcpy((char *) GPS_Data_Temp,(const char *)GPS_Uart2_Data);
				memset(GPS_Uart2_Data,0,sizeof(unsigned char) *256);
				GPS_Analysis(&gps,(u8*)GPS_Data_Temp);//�����ַ���			
			}
		}
		else if (GPSKEY != 0) {GpsKeyFlag = 0xff; GPSDISENABLE();memset((uint8_t *)&gps.utc, 0,sizeof(gps_utc_time));}
	}
}
//��ʱ��5�жϷ������

u16 Time500Ms = 0;
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
