#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/ 
u8 Flag_Left,Flag_Right,Flag_Direction=0;   //����ң����صı���
u8 Flag_Stop=1,Flag_Show=0;                 //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder_A,Encoder_B,Encoder_C,Encoder_D;          //���������������
long int Position_A,Position_B,Position_C,Position_D,Rate_A,Rate_B,Rate_C,Rate_D; //PID������ر���
int Encoder_A_EXTI;                       //ͨ���ⲿ�ж϶�ȡ�ı���������                       
long int Motor_A,Motor_B,Motor_C,Motor_D;        //���PWM����
long int Target_A,Target_B,Target_C,Target_D;     //���Ŀ��ֵ
int Voltage;                             //��ص�ѹ������صı���
float Show_Data_Mb;                      //ȫ����ʾ������������ʾ��Ҫ�鿴������                         
u8 delay_50,delay_flag;                          //��ʱ��ر���
u8 Run_Flag=1;  												//����ң����ر���������״̬��־λ
u8 rxbuf[8],Urxbuf[8],CAN_ON_Flag=0,Usart_ON_Flag=0,Usart_Flag,PID_Send,Flash_Send;  //CAN�ʹ��ڿ�����ر���
u8 txbuf[8],txbuf2[8],Turn_Flag;             //CAN������ر���
float Pitch,Roll,Yaw,Move_X,Move_Y,Move_Z;   //����ǶȺ�XYZ��Ŀ���ٶ�
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash�������
float	Position_KP=20,Position_KI=0,Position_KD=20;  //λ�ÿ���PID����
float Velocity_KP=10,Velocity_KI=10;	          //�ٶȿ���PID����
int RC_Velocity=30,RC_Position=10000;         //����ң�ص��ٶȺ�λ��ֵ
u8 UsartData[9];      //��ȡ��ң����������
u16 G_dr16_ch0_value = 0;
u16 G_dr16_ch1_value = 0;
u16 G_dr16_ch2_value = 0;
u16 G_dr16_ch3_value = 0;
u8 G_dr16_ch4_value = 0;
int main(void)
{ 
	delay_init();                 //=====��ʱ��ʼ��
	JTAG_Set(SWD_ENABLE);     //=====�ر�JTAG�ӿ�
//	JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
	LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	//KEY_Init();                     //=====������ʼ��
	//if(MODE==0)Run_Flag=1;          //=====�����Ĺ����У�����ģʽѡ�񿪹�ȷ������λ��ģʽ�����ٶ�ģʽ
	//else Run_Flag=0;                //=====�����Ĺ����У�����ģʽѡ�񿪹�ȷ������λ��ģʽ�����ٶ�ģʽ
	//OLED_Init();                    //=====OLED��ʼ��
	//uart2_init(36,9600);            //=====����2��ʼ��
  //uart3_init(36,115200);          //=====����3��ʼ�� 
	Encoder_Init_TIM2();            //=====�������ӿ�
	Encoder_Init_TIM3();            //=====�������ӿ�
	Encoder_Init_TIM4();            //=====��ʼ��������C
	Encoder_Init_TIM5();            //=====��ʼ��������D
	//Adc_Init();                     //=====adc��ʼ��
	MiniBalance_PWM_Init(7199,0);   //=====��ʼ��PWM 10KHZ�������������
	//if(KEY==0) 	Flash_Read();        //=====��ȡFlash����Ĳ���
	//CAN1_Mode_Init(1,2,3,6,0);       //=====CAN��ʼ��	
	TIM6_Init(50,7199);              //=====��ʱ���ж�
	Usart3_Init(100000);
	LED=1;	
	while(1)
		{	
				
			if(UsartData[8]==0XFF)
			{
				UsartData[8]=0X00;
				if(UsartData[2]==0x01)//�ж��ǲ���cmd ��0x01��
					MotorGoAngle_Func(UsartData[4]);//ִ���˶�����
			}
			
			
			
			
			
			
			
/*			
	  	if(Flash_Send==1)          //д��PID������Flash,��app���Ƹ�ָ��
			{
				Flash_Write();	
				Flash_Send=0;	
			}	
	    if(Flag_Show==0)           //ʹ��MiniBalance APP��OLED��ʾ��
			{
				APP_Show();	              
				oled_show();             //===��ʾ����
	    }
    	else                       //ʹ��MiniBalance��λ�� ��λ��ʹ�õ�ʱ����Ҫ�ϸ��ʱ�򣬹ʴ�ʱ�ر�app��ز��ֺ�OLED��ʾ��
  		{
				DataScope();             //����MiniBalance��λ��
			}
      CAN1_SEND();                //CAN����			
			delay_flag=1;	
			delay_50=0;
			while(delay_flag);	       //ͨ��MPU6050��INT�ж�ʵ�ֵ�50ms��׼��ʱ				
*/
			} 
}
