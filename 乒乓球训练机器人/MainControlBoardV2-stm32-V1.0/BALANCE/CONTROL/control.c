#include "control.h"	
#include "filter.h"	
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/

u8 Flag_Target,Flag_Change;                             //��ر�־λ
u8 temp1;                                               //��ʱ����
float Voltage_Count,Voltage_All; 											  //��ѹ������ر���
float Gyro_K=0.004;     				  											//�����Ǳ���ϵ��
int j;
#define a_PARAMETER          (0.079f)                    
/**************************************************************************
�������ܣ�С���˶���ѧģ��
��ڲ�����X Y Z �����ٶȻ���λ��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(float Vy,float Vz)
{
        Target_A   = Vy-Vz*(a_PARAMETER);
        Target_B   = Vy-Vz*(a_PARAMETER);
	      Target_C   = Vy+Vz*(a_PARAMETER);
				Target_D   = Vy+Vz*(a_PARAMETER);  
}
/**************************************************************************
�������ܣ���ȡλ�ÿ��ƹ����ٶ�ֵ
��ڲ�����X Y Z ����λ�ñ仯��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis2(float Vy,float Vz)
{
	    Rate_A   = Vy-Vz*(a_PARAMETER)*10;
        Rate_B   = Vy-Vz*(a_PARAMETER)*10;
	      Rate_C   = Vy+Vz*(a_PARAMETER)*10;
				Rate_D   = Vy+Vz*(a_PARAMETER)*10;
}
void Clear_Position(void)
{
	Position_A=0;
	Position_B=0;
	Position_C=0;
	Position_D=0;
}
	//Target_B--1��
	//Target_A--2��
	//Target_C--3��
	//Target_D--4��
	
void MotorGoAngle_Func(u8 AngleVal)
{
	switch(AngleVal)
	{
		case Angle_0:   Clear_Position();Target_C=10000;  Target_D=-10000;Target_B=-10000;Target_A=10000;break;
		case Angle_45:  Clear_Position();Target_C=10000;   Target_D=0;Target_B=0;Target_A=10000;break;
		case Angle_90:  Clear_Position();Target_C=10000;  Target_D=10000;Target_B=10000;Target_A=10000;break;
		case Angle_135: Clear_Position();Target_C=0;  Target_D=10000;Target_B=10000;Target_A=0;break;
		case Angle_180: Clear_Position();Target_C=-10000; Target_D=10000;Target_B=10000;Target_A=-10000;break;
		case Angle_225: Clear_Position();Target_C=-10000;      Target_D=0;Target_B=0;Target_A=-10000;break;
		case Angle_270: Clear_Position();Target_C=-10000; Target_D=-10000;Target_B=-10000;Target_A=-10000;break;
		case Angle_315: Clear_Position();Target_C=0; Target_D=-10000;Target_B=-10000;Target_A=0;break;
		case AngleL_360:Clear_Position();Target_C=-10000;  Target_D=10000;Target_B=-10000;Target_A=10000;break;
		case AngleR_360:Clear_Position();Target_C=10000; Target_D=-10000;Target_B=10000;Target_A=-10000;break;
		default:Target_A=0;Target_C=0;Target_D=0;Target_B=0;Clear_Position();break;
	}
}

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
int TIM6_IRQHandler(void) 
{    
	if(TIM6->SR&0X0001)//����ж�
	{     
			TIM6->SR&=~(1<<0);//����жϱ�־λ 	    
		   Flag_Target=!Flag_Target;
//		  if(delay_flag==1)
//			 {
//				 if(++delay_50==10)	 delay_50=0,delay_flag=0;                     //���������ṩ50ms�ľ�׼��ʱ
//			 }
//		  if(Flag_Target==1)                                                  //5ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ
//			{
//					if(Usart_Flag==0&&Usart_ON_Flag==1)  memcpy(rxbuf,Urxbuf,8*sizeof(u8));	//��������˴��ڿ��Ʊ�־λ�����봮�ڿ���ģʽ
//			  	Key();//ɨ�谴���仯	
//			return 0;	                                               
//			}                                                                     	 //===10ms����һ�Σ�Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
			Encoder_A=Read_Encoder(2);                                          		//===��ȡ��������ֵ
			Position_A+=Encoder_A;                                                	 //===���ֵõ��ٶ�   
			Encoder_C=-Read_Encoder(3);                                          		//===��ȡ��������ֵ
			Position_C+=Encoder_C;                                                 	//===���ֵõ��ٶ�   
			Encoder_D=Read_Encoder(5);                                         		 //===��ȡ��������ֵ
			Position_D+=Encoder_D;                                                 	//===���ֵõ��ٶ�   
			Encoder_B=-Read_Encoder(4);                                         		//===��ȡ��������ֵ
			Position_B+=Encoder_B;                                                 	//===���ֵõ��ٶ�   
			Led_Flash(100);                                                       	 //===LED��˸;����ģʽ 1s�ı�һ��ָʾ�Ƶ�״̬	
//			Voltage_All+=Get_battery_volt();                                      	 //��β����ۻ�
//			if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//��ƽ��ֵ ��ȡ��ص�ѹ	       
//		  if(CAN_ON_Flag==1||Usart_ON_Flag==1) CAN_N_Usart_Control();          //�ӵ����ڻ���CANң�ؽ���ָ��֮��ʹ��CAN�ʹ��ڿ�������

		 if(Turn_Off(1112)==0)               //===�����ص�ѹ�������쳣
		 { 			 
		  if(Run_Flag==0)//�ٶ�ģʽ
			{		
				if(CAN_ON_Flag==0&&Usart_ON_Flag==0)  Get_RC(Run_Flag);                //===���ں�CAN���ƶ�δʹ�ܣ����������ң��ָ
				Motor_A=Incremental_PI_A(Encoder_A,Target_A);                         //===�ٶȱջ����Ƽ�����A����PWM
				Motor_B=Incremental_PI_B(Encoder_B,Target_B);                         //===�ٶȱջ����Ƽ�����B����PWM
				Motor_C=Incremental_PI_C(Encoder_C,Target_C);                         //===�ٶȱջ����Ƽ�����C����PWM
				Motor_D=Incremental_PI_D(Encoder_D,Target_D);                         //===�ٶȱջ����Ƽ�����C����PWM
			}
			 else//λ��ģʽ
			{
			//	 if(CAN_ON_Flag==0&&Usart_ON_Flag==0) //===���ں�CAN���ƶ�δʹ�ܣ����������ң��ָ��
			//	 {	
			//		if(Turn_Flag==0) 	Flag_Direction=click_RC();     
			//		Get_RC(Run_Flag);
			//	 }
					Motor_A=-Position_PID_A(Position_A,Target_A)>>8;//λ�ñջ����ƣ�������A�ٶ��ڻ���������
					Motor_B=-Position_PID_B(Position_B,Target_B)>>8;//λ�ñջ����ƣ�������B�ٶ��ڻ���������
					Motor_C=-Position_PID_C(Position_C,Target_C)>>8;//λ�ñջ����ƣ�������C�ٶ��ڻ���������
				 	Motor_D=-Position_PID_D(Position_D,Target_D)>>8;//λ�ñջ����ƣ�������C�ٶ��ڻ���������
			/*	
			   if(rxbuf[0]!=2)  Count_Velocity();   //���ǵ���λ�ÿ��ƹ��̵��ٶȴ�С
					else 	
					Xianfu_Velocity(RC_Velocity,RC_Velocity,RC_Velocity,RC_Velocity); 
					Motor_A=Incremental_PI_A(Encoder_A,-Motor_A);         //===�ٶȱջ����Ƽ�����A����PWM
					Motor_B=Incremental_PI_B(Encoder_B,-Motor_B);         //===�ٶȱջ����Ƽ�����B����PWM
					Motor_C=Incremental_PI_C(Encoder_C,-Motor_C);         //===�ٶȱջ����Ƽ�����C����PWM
					Motor_D=Incremental_PI_D(Encoder_D,-Motor_D);         //===�ٶȱջ����Ƽ�����C����PWM
			*/
			}	 
		 Xianfu_Pwm(100);                        //===PWM�޷�
		 Set_Pwm(Motor_A,Motor_C,Motor_D,Motor_B);     //===��ֵ��PWM�Ĵ���  
		 }

 }
	 return 0;	 
} 
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d)
{
	  	if(motor_a>0)			PWMA1=7200,PWMA2=7200-motor_a;
			else 	            PWMA2=7200,PWMA1=7200+motor_a;
		
		  if(motor_b>0)			PWMB1=7200,PWMB2=7200-motor_b;
			else 	            PWMB2=7200,PWMB1=7200+motor_b;
	
	    if(motor_c>0)			PWMC1=7200,PWMC2=7200-motor_c;
			else 	            PWMC2=7200,PWMC1=7200+motor_c;
		
	    if(motor_d>0)			PWMD1=7200,PWMD2=7200-motor_d;
			else 	            PWMD2=7200,PWMD1=7200+motor_d;
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(int amplitude)
{	
    if(Motor_A<-amplitude) Motor_A=-amplitude;	
		if(Motor_A>amplitude)  Motor_A=amplitude;	
	  if(Motor_B<-amplitude) Motor_B=-amplitude;	
		if(Motor_B>amplitude)  Motor_B=amplitude;		
	  if(Motor_C<-amplitude) Motor_C=-amplitude;	
		if(Motor_C>amplitude)  Motor_C=amplitude;		
	  if(Motor_D<-amplitude) Motor_D=-amplitude;	
	  if(Motor_D>amplitude)  Motor_D=amplitude;		
}
/**************************************************************************
�������ܣ�λ��PID���ƹ������ٶȵ�����
��ڲ������ޡ���ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Velocity(int amplitude_A,int amplitude_B,int amplitude_C,int amplitude_D)
{	
    if(Motor_A<-amplitude_A) Motor_A=-amplitude_A;	//λ�ÿ���ģʽ�У�A����������ٶ�
		if(Motor_A>amplitude_A)  Motor_A=amplitude_A;	  //λ�ÿ���ģʽ�У�A����������ٶ�
	  if(Motor_B<-amplitude_B) Motor_B=-amplitude_B;	//λ�ÿ���ģʽ�У�B����������ٶ�
		if(Motor_B>amplitude_B)  Motor_B=amplitude_B;		//λ�ÿ���ģʽ�У�B����������ٶ�
	  if(Motor_C<-amplitude_C) Motor_C=-amplitude_C;	//λ�ÿ���ģʽ�У�C����������ٶ�
		if(Motor_C>amplitude_C)  Motor_C=amplitude_C;		//λ�ÿ���ģʽ�У�C����������ٶ�
	  if(Motor_D<-amplitude_D) Motor_D=-amplitude_D;	//λ�ÿ���ģʽ�У�D����������ٶ�
		if(Motor_D>amplitude_D)  Motor_D=amplitude_D;		//λ�ÿ���ģʽ�У�D����������ٶ�
}
/**************************************************************************
�������ܣ������޸�С������״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
//void Key(void)
//{	
//	u8 tmp;
//	tmp=click_N_Double(100); 
//	if(tmp==2)Flag_Show=!Flag_Show;//˫��������ʾģʽ                  
//}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
			if(voltage<1110)//��ص�ѹ���͹رյ��
			{	                                                
      temp=1;      
     	PWMA1=0; //�������λ����                                           
			PWMB1=0; //�������λ����
			PWMC1=0; //�������λ����
			PWMD1=0; //�������λ����
			PWMA2=0; //�������λ����
			PWMB2=0; //�������λ����
			PWMC2=0; //�������λ����	
      PWMD2=0; //�������λ����				
      }
			else
      temp=0;
      return temp;			
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����long int
����  ֵ��unsigned int
**************************************************************************/
u32 myabs(long int a)
{ 		   
	  u32 temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_C (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_D (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PID_A (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_B (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_C (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_D (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
/**************************************************************************
�������ܣ�ͨ������ָ���С������ң��
��ڲ���������ָ��
����  ֵ����
**************************************************************************/
void Get_RC(u8 mode)
{
float step=0.5;  //�����ٶȿ��Ʋ���ֵ��
	  if(mode==0)//�ٶ�
		{	
				 switch(Flag_Direction)   //�������
				 {
				 case 0:      Move_Y=0; 			 			 	 Move_Z=0;        	 break;	 
				 case 1:      Move_Y+=step;  	 				 Move_Z=0;       		 break;
				 case 2:      Move_Y+=step;  	 	 			 Move_Z-=step;   	 	 break;
				 case 3:      Move_Y=0;      				 	 Move_Z-=step;   		 break;
				 case 4:      Move_Y-=step;  					 Move_Z+=step; 			 break;
				 case 5:      Move_Y-=step;  					 Move_Z=0;       		 break;
				 case 6:      Move_Y-=step;  					 Move_Z-=step;   		 break;
				 case 7:      Move_Y=0;     	 			 	 Move_Z+=step;			 break;
				 case 8:      Move_Y+=step; 	 				 Move_Z+=step;			 break; 
				 default:     break;	 
			 }
				if(Move_X<-RC_Velocity) Move_X=-RC_Velocity;	   //�ٶȿ����޷�
				if(Move_X>RC_Velocity)  Move_X=RC_Velocity;	     
				if(Move_Y<-RC_Velocity) Move_Y=-RC_Velocity;	
				if(Move_Y>RC_Velocity)  Move_Y=RC_Velocity;	 
				if(Move_Z<-RC_Velocity) Move_Z=-RC_Velocity;	
				if(Move_Z>RC_Velocity)  Move_Z=RC_Velocity;	 
	 }
		 else if(mode==1)//λ��ģʽ
		{	
				 switch(Flag_Direction)   //�������
				 {
				 case 1:     	Move_Y+=RC_Position;  	      															   break;
				 case 2:      Move_Y+=RC_Position;           Move_Z-=RC_Position;       		 break;
				 case 3:      															 Move_Z-=RC_Position;            break;
				 case 4:      Move_Y-=RC_Position; 					 Move_Z+=RC_Position;       		 break;
				 case 5:      Move_Y-=RC_Position;  	 			 															   break;
				 case 6:      Move_Y-=RC_Position; 					 Move_Z-=RC_Position;       		 break;
				 case 7:      															 Move_Z+=RC_Position;     		   break;
				 case 8:      Move_Y+=RC_Position;           Move_Z+=RC_Position;            break; 	 
				 default: break;	 
			 }
	 }
		 Kinematic_Analysis(Move_Y,Move_Z);//�õ�����Ŀ��ֵ�������˶�ѧ����
}
/**************************************************************************
�������ܣ�ÿ�����λ�ÿ��ƹ����ٶȼ���
��ڲ�������
����  ֵ����
**************************************************************************/
void Count_Velocity(void)
{
	static double Last_Target_Y,Last_Target_Z,Divider;
	double Bias_Y,Bias_Z;
	Bias_Y=(Move_Y-Last_Target_Y);	//��Y��λ����
	Bias_Z=(Move_Z-Last_Target_Z);	//��Z��λ����
	if(Bias_Y!=0||Bias_Z!=0)Divider=sqrt(Bias_Y*Bias_Y+Bias_Z*Bias_Z);
	if(Bias_Y!=0||Bias_Z!=0) Kinematic_Analysis2(Bias_Y,Bias_Z);

	Xianfu_Velocity(RC_Velocity*myabs(Rate_A)/Divider,RC_Velocity*myabs(Rate_B)/Divider,RC_Velocity*myabs(Rate_C)/Divider,RC_Velocity*myabs(Rate_D)/Divider); 
	Last_Target_Y=Move_Y;   //����Y����һ�ε�λ����Ϣ�����ڵ���
	Last_Target_Z=Move_Z;   //����Z����һ�ε�λ����Ϣ�����ڵ���
}
/**************************************************************************
�������ܣ�����CAN���ߴ��ڿ���ָ����д���
��ڲ�������
����  ֵ����
**************************************************************************/
void CAN_N_Usart_Control(void)
{
 int flag_1, flag_2,flag_3,flag_4;
	 if(Run_Flag==0)//�ٶ�ģʽ
	 {
		 if(rxbuf[0]==1)
		 {
			 if((rxbuf[7]&0x02)==0)flag_1=1;  else flag_1=-1;  //�������λ
			 if((rxbuf[7]&0x01)==0)flag_2=1;  else flag_2=-1;  //�������λ
			 Move_Y=flag_1*(rxbuf[1]*256+rxbuf[2]);	
			 Move_Z=flag_2*(rxbuf[3]*256+rxbuf[4]);	
			 Kinematic_Analysis(Move_Y,Move_Z);//�����˶�ѧ����
			 Gyro_K=0;    
		 }
		 if(rxbuf[0]==2)
		 {
			 if((rxbuf[7]&0x08)==0)flag_1=1;  else flag_1=-1;  //�������λ
			 if((rxbuf[7]&0x04)==0)flag_2=1;  else flag_2=-1;  //�������λ
			 if((rxbuf[7]&0x02)==0)flag_3=1;  else flag_3=-1;  //�������λ
			 if((rxbuf[7]&0x01)==0)flag_4=1;  else flag_4=-1;  //�������λ
			 Target_A=flag_1*rxbuf[1];
			 Target_B=flag_2*rxbuf[2];
			 Target_C=flag_3*rxbuf[3];
			 Target_D=flag_4*rxbuf[4];
		 }
	 }
	 	else if(Run_Flag==1)//λ��ģʽ
	 {
		 if(rxbuf[0]==1)
		 {
			 if((rxbuf[7]&0x02)==0)flag_1=1;  else flag_1=-1;  //�������λ
			 if((rxbuf[7]&0x01)==0)flag_2=1;  else flag_2=-1;  //�������λ
			 Move_Y=flag_1*(rxbuf[1]*256+rxbuf[2]);	
			 Move_Z=flag_2*(rxbuf[3]*256+rxbuf[4]);	
			 Kinematic_Analysis(Move_Y,Move_Z);//�����˶�ѧ����
			 Gyro_K=0;    
		 }
	 }
}

