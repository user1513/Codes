#include "main.h"

typedef enum{InitDisplayState = 0, NumModeDisplayState, GraphModeDisplayState, ParameterSetDisplayState, ErrStatus = -1 } DisplayState; //����״̬��ö������

DisplayState _DisplayState = InitDisplayState;		//����һ��ö�ٱ���
	
DisplayState _InitDispay = GraphModeDisplayState;	//����һ��ö�ٱ���

bool LCD12864_START_FLAG = FALSE;  					//����LCD12864��ʼ��־λ,Ĭ��4������������

bool DISPLAY_STATE_FLAG = FALSE;  					//����display_state��ʼ��־λ,Ĭ��4.5���ʼ����

#define LCD12864_START_TIME 4000   					//����LCD12864��ʼʱ��,Ĭ��4������������

#define DISPLAY_STATE_TIME 4500   					//����display_state��ʼʱ��,Ĭ��4.5���ʼ����

#define SETVAL_MAX	4000							//�������ֵΪ4000

#define SETVAL_MIN	0								//�������ֵΪ0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PeriphInit(void);								//�����ʼ��

void display_state(void);							//��ʾ״̬������

int cmp(const void *a,const void *b);				//qsort

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int SetVal = 1000;									//���崫��������ֵĬ��1000�����4000����С0

double CurrentVal = 0.0;							//���崫������ǰֵ����

uint8_t PM2_5_table[128] = {0};						//����һ�����PM2.5���������ͼ����ʾ

uint8_t PM2_5_table_test[20] = {0};					//����һ����ʱ���PM2.5������

uint8_t GetDataFlag = 0;							//��ȡ���ݵ���20����־λ

uint8_t keyval = 0;
int main(void)
{
	int i = 0;
	
	PeriphInit();
	Lcd12864Display(0x80,(uint8_t *)"    ��ӭʹ��    "); 
	Lcd12864Display(0x90,(uint8_t *)"  ���ڵ�Ƭ����  ");
	Lcd12864Display(0x88,(uint8_t *)"  �����������  ");
	Lcd12864Display(0x98,(uint8_t *)"      --ϵͳ���");
	LCD12864Display_ON();													
	FuncReg(&LCD12864_START_FLAG, LCD12864_START_TIME, TRUE);	//����LCD12864_START_FLAG��ʱ�ص�����
	FuncReg(&DISPLAY_STATE_FLAG, DISPLAY_STATE_TIME, TRUE);	//����LCD12864_START_FLAG��ʱ�ص�����
	while(1)
	{
		if(LCD12864_START_FLAG == TRUE && _DisplayState == InitDisplayState)
		{
			LCD12864_START_FLAG = FALSE;
			Lcd_Clear(0);
			Lcd12864Display(0x80,(uint8_t *)"    ģʽѡ��    "); 
			Lcd12864Display(0x90,(uint8_t *)"    ����ģʽ    ");
			Lcd12864Display(0x88,(uint8_t *)"    ͼ��ģʽ    ");
			Lcd12864Display(0x98,(uint8_t *)"    ��������    ");
			Lcd12864String(0x91,0,(uint8_t *)" >");
			Lcd12864String(0x96,0,(uint8_t *)"<");
		}
		if(_DisplayState == NumModeDisplayState && LCD12864_START_FLAG == FALSE)
		{
			LCD12864_START_FLAG = TRUE;
			Lcd12864Display(0x80,(uint8_t *)"    ����ģʽ    "); 
			Lcd12864Display(0x90,(uint8_t *)"�趨ֵ:         ");
			Lcd12864Display(0x88,(uint8_t *)"��ǰֵ:         ");
			Lcd12864Display(0x98,(uint8_t *)"״̬��          ");
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

			memset(OLED_GRAM, 0, 16*64);//����
			delay_ms(1000);
			Lcd_Clear(0);
		}
		if(_DisplayState == ParameterSetDisplayState && LCD12864_START_FLAG == FALSE)
		{
			LCD12864_START_FLAG = TRUE;
			Lcd12864Display(0x80,(uint8_t *)"    ��������    "); 
			Lcd12864Display(0x90,(uint8_t *)"�趨ֵ:         ");
			Lcd12864Display(0x88,(uint8_t *)"����:           ");
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

	LED_Init();						//LED ���ų�ʼ��
	
	PM2_5_Led_Io_init();			//PM2.5������led�Ƴ�ʼ��
	
	Key_Io_Init();					//����io��ʼ��
	
	Sound_Light_Alarm_Io_Init();	//���ⱨ��io��ʼ��
	
	Adc_Init();						//ADC��ʼ��
	
	delay_init();					//��ʱ��������ʼ������systick��ʼ����
	
	TIM4_Init(719,99);				//��ʱ��4��ʼ�� ��ʱʱ�� = [(719 + 1)*(99 + 1)]/72000000��1ms
	
	TIM3_Init(71,99);				//��ʱ��3��ʼ�� ��ʱʱ�� = [(71 + 1)*(99 + 1)]/72000000��0.1ms
	
	Adc_Init();		  				//ADC��ʼ��
	
	uart_init(115200);				//����2��ʼ��,�����ʣ�115200
	
	Lcd12864init();					//LCD12864��ʼ��
	
	Buzzer_Disable();				//��������ʹ��
	
	Led_Disable();					//�ƹⲻʹ��
}

uint8_t display_string[20] = {0}; //�����ַ���

void display_state(void)		//��ʾ״̬������ 
{
	static uint8_t ParameterSetDisplayFlag = 0;		//����һ������ģʽ�±�־λ
	static uint8_t CallPoliceSetDisplayFlag = 0;	//����һ������ģʽ�±�����־λ
	switch(_DisplayState)
	{
		case InitDisplayState:						//��ʼĿ¼����
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
		case NumModeDisplayState:									//����ģʽ����
			
		if((keyval & 1<<2) == 1<<2)									//��ⰴ����������
		{
			_DisplayState = InitDisplayState;						//�ص���ʼ����
			_InitDispay = GraphModeDisplayState;
		}
		sprintf((char *)display_string,"%4d",SetVal);
		Lcd12864Display(0x94,(uint8_t *)display_string);
		sprintf((char *)display_string,"%4.1f  ",CurrentVal);
		Lcd12864Display(0x8C,(uint8_t *)display_string);
		if(CurrentVal > SetVal && CallPoliceSetDisplayFlag)			//��ǰPM2.5����>�趨ֵ�����ұ���ģʽ��
		{
			Lcd12864Display(0x9C,(uint8_t *)"����");
			Buzzer_Enable();										//������ʹ��
			Led_Enable();											//�ƹ�ʹ��
		}
		else
		{
			Lcd12864Display(0x9C,(uint8_t *)"����");
			Buzzer_Disable();										//��������ʹ��
			Led_Disable();											//�ƹⲻʹ��
		}
		break;
		case GraphModeDisplayState: 								//ͼ��ģʽ����
		if((keyval & 1<<2) == 1<<2)									//��ⰴ����������
		{
			_DisplayState = InitDisplayState;						//�ص���ʼ����
			_InitDispay = GraphModeDisplayState;
			LCD12864_START_FLAG = TRUE;
		}
		if(CurrentVal > SetVal && CallPoliceSetDisplayFlag)			//��ǰPM2.5����>�趨ֵ�����ұ���ģʽ��
		{
			Buzzer_Enable();										//������ʹ��
			Led_Enable();											//�ƹ�ʹ��
		}
		else
		{
			Buzzer_Disable();										//��������ʹ��
			Led_Disable();											//�ƹⲻʹ��
		}
		break;
		case ParameterSetDisplayState: 								//�������ý���
		if((keyval & 1<<2) == 1<<2)     							//��ⰴ����������
		{
			_DisplayState = InitDisplayState;						//�ص���ʼ����
			_InitDispay = GraphModeDisplayState;
		}
		if((keyval & 1<<1) == 1<<1)									//����м䰴���̰�����
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
		sprintf((char *)display_string,"�趨ֵ: %4d",SetVal);
		Lcd12864Display(0x90,(uint8_t *)display_string);
		if(CallPoliceSetDisplayFlag)
			Lcd12864Display(0x8C,(uint8_t *)"�ر�");
		else
			Lcd12864Display(0x8C,(uint8_t *)"��");
		break;
		default :break;
	}
	
}

//��ʱ��4�жϷ������

u16 Time500Ms = 0;
u16 Time50Ms = 0;
u16 Time10Ms = 0;
u8	GetDataNum = 0;
uint8_t  data_test[10] = {0};

double sort_table[10] = {0};
void TIM4_IRQHandler(void)   //TIM4�ж�
{
	int i = 0, j = 0;
	if(TIM4->SR&0X0001)//1ms��ʱ�ж�
 {
	 if((Time500Ms++)==500)						//500ms��ʱ
	 {
		 Time500Ms=0;
		 led=~led;
		 printf("pm2.5 = %f\n", CurrentVal);	//���ڴ�ӡ����,usart1
		 
	 }
	 FuncTimeTravers_ISR();						//�ص������ӿ�
	 if(DISPLAY_STATE_FLAG == TRUE)				//��ʾ�Ƿ�ʹ��
	{
		if((Time10Ms++) == 10)					//10ms��ʱ
		{
		 Time10Ms = 0;
		 keyval = Key_Event_Handler();			//��ȡ�����¼�������
		 display_state();						//��ʾ״̬��ˢ��
		}
	 
		if(++Time50Ms == 50)
		{
			Time50Ms = 0;
			sort_table[GetDataNum] = (ADC_Val[0] > 72.91? (ADC_Val[0]/2048.0 - 0.0356)*120000*0.035 :  ADC_Val[0]); //��ȡ����
			data_test[GetDataNum] = (uint8_t)(sort_table[GetDataNum] / 40.0);
			if(data_test[GetDataNum] > 90)
				data_test[GetDataNum] = 90;
			 if(++GetDataNum  == 10)
			 {
				 GetDataNum = 0;
				 for(i = 0; i < 10; i++)							//���ݸ���
				 {
					qsort(sort_table,10,sizeof(sort_table[0]),cmp); //��������
					for(j = 3; j < 7; j++)
					 {
						CurrentVal +=  sort_table[j]; 				//�ۼ�
					 }
					 CurrentVal  /= 4; 								//��ƽ��ֵ
					PM2_5_table_test[i * 2 + 1] = data_test[i]; 
					PM2_5_table_test[i * 2] = data_test[i];
				 }
				GetDataFlag = 1;
			 }
		}
	 }
 }
 TIM4->SR&=~(1<<0);  //�жϱ�־λ��0���ȴ���һ���ж�
}



/******************************************************************************************************

1KHz-pm2.5ר��

******************************************************************************************************/
u16 Time500us = 0;
void TIM3_IRQHandler(void)   //TIM3�ж� //100us�ж�
{
	if(TIM3->SR&0X0001)//1ms��ʱ�ж�
 {
	 if((Time500us++)==5)
	 {
		 Time500us=0;
		 PM2_5_Led=~PM2_5_Led; 
	 }
 }
 TIM3->SR&=~(1<<0);  //�жϱ�־λ��0���ȴ���һ���ж�
}







int cmp(const void *a,const void *b)
{
    return *(double *)a-*(double *)b;//���Ǵ�С�����������ǴӴ�С�ĳɣ� return *(int *)b-*(int *)a;
}

