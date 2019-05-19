#include "timer.h"

extern u8 Usart2_table[];

extern u8 Sensor_data[];

extern u8 Equip_State;
unsigned char GPS_Data_Temp[256];

gps_process_data gps;     //GPS�ṹ��
/************************************************
 ����Zigbee��·�ƿ���ϵͳ
 ����ģ��
 ע�⣬����timer��ʱ������ 
 //�޸�����:2018/4/6
 //�汾��V1.0
 ���ߣ�Ԭ�г�
************************************************/


//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
u8 State_control[8];
extern u8 State_count[];//���յ�RGBֵ��·�ƿ���ֵ
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������

	u16 State_1ms=0,Heartbeat_val=0;
    u8 State_1s=0;
u16 Light_value=0;
void TIM3_IRQHandler(void)   //TIM3�ж�
{
    
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			State_1ms++;
			if(State_1ms==500)
			{
				State_1ms=0;
                Real_Time_Data();
				LED0=!LED0;
                State_1s++;
               bh1750fvi_Write(0x46,0x10);                    //BH1750FVI�����߷ֱ�ģʽ
                Light_value=bh1750fvi_Read(0x47);
                Sensor_data[15]=Light_value>>8;       //��ȡBH1750FVI��8λ����
                Sensor_data[16]=Light_value;          //��ȡBH1750FVI��8λ����
                //printf("����ֵ��%d",Light_value);
                DHT11_Read_Data(&Sensor_data[13],&Sensor_data[14]);//��ȡDHT11�е���ʪ��ֵ
                //printf("�¶ȣ�%d��,ʪ�ȣ�%d%\n",Sensor_data[11],Sensor_data[12]);
				Sensor_data[19]=((double)((3.3*ADC_Val[0]*100)/4096));
				//GPS����		
				if(gps.gpssta!=0)	
				{
					//Usart3_String_Send("���ȣ� γ�ȣ� GPS״̬�� Сʱ�����ӣ��룺");
					//Usart3_Send_Byte('\n');
					Sensor_data[9]=gps.longitude/1000000;  //����
					Sensor_data[10]=(gps.longitude%1000000)>>16;
					Sensor_data[11]=(gps.longitude%1000000)>>8;
					Sensor_data[12]=(gps.longitude%1000000);
					Sensor_data[5]=gps.latitude/1000000;   //γ��
					Sensor_data[6]=(gps.latitude%1000000)>>16; 
					Sensor_data[7]=(gps.latitude%1000000)>>8; 
					Sensor_data[8]=(gps.latitude%1000000);

					Sensor_data[2]=gps.utc.hour;          //ʱ��
					Sensor_data[3]=gps.utc.min;
					Sensor_data[4]=gps.utc.sec;
					if(gps.nshemi=='N')
						Sensor_data[17]=0x01;
					else if(gps.nshemi=='S')
						Sensor_data[17]=0x02;
					else
						Sensor_data[17]=0x00;
					if(gps.ewhemi=='E')
						Sensor_data[18]=0x01;
					else if(gps.ewhemi=='W')
						Sensor_data[18]=0x02;
					else
						Sensor_data[18]=0x00;
					
					//Usart3_Send_Byte('\n');
				}
                	Send_Data_Page(Equip_State);
		
			}	
            if(State_1s==10)
            {
                State_1s=0;
                HeartBeat_Send(Heartbeat_val);
                Heartbeat_val++;
            }
            if(Usart2_table[9]==0xAA)
            {
                Usart2_table[9]=0X00;
                switch(Usart2_table[4])
                {
                 case 0x00:
                    {
                    switch(Usart2_table[5])
                    {
                      case 0x00:
                      {
                        switch(Usart2_table[8])
                         {
                              case 0x01:State_control[0]=0x01;break;
                              case 0x02:State_control[1]=0x01;break;
                              case 0x03:State_control[2]=0x01;break;
                              case 0x04:State_control[3]=0x01;break;
                              case 0x05:State_control[4]=0x01;break;
                              default: break;
                          } 
                       break;
                      }
                      case 0x01:State_count[0]=Usart2_table[8];State_control[5]=0x01;break;
                      case 0x02:State_count[1]=Usart2_table[8];State_control[6]=0x01;break;
                      case 0x03:State_count[2]=Usart2_table[8];State_control[7]=0x01;break;
                      default :break;
                    }
                    break;
                 } 
                 case 0x10:
                    {
                        if(Usart2_table[8]==0x00) State_count[3]=Usart2_table[8];
                        else if(Usart2_table[8]==0x01) State_count[3]=Usart2_table[8]; 
                        break;

                    }

                 default :break;
                                      
                    
                }
                
            }
            /********************************************************
            ���������ϼ��
            ********************************************************/
            if(gps.longitude==0||gps.latitude==0)
            {
                Equip_State |=0x02;
            }
            else
            {
                Equip_State &=~0x02;
            }
            if(Sensor_data[13]==0&&Sensor_data[14]==0)
            {
                Equip_State |=0x04;
            }
            else
            {
                Equip_State &=~0x04;
            }            
            if(Light_value==0||Light_value>20000)
            {
                Equip_State |=0x08;
            }
            else
            {
                Equip_State &=~0x08;
            }            
//            if(Sensor_data[14]==0)
//            {
                Equip_State |=0x10;
//            }
//            else
//            {
//                Equip_State &=~0x10;
            //}            
            if(Sensor_data[19]==0)
            {
                Equip_State |=0x20;
            }
            else
            {
                Equip_State &=~0x20;
            }            
            
            if(State_control[0]==0x01)
            {
                State_control[0]=0x00;
                Data_Return(0x01);
            }
            
            if(State_control[1]==0x01)
            {
                State_control[1]=0x00;
                Data_Return(0x02);
            }
            
            if(State_control[2]==0x01)
            {
                State_control[2]=0x00;
                Data_Return(0x03);
            }
            
            if(State_control[3]==0x01)
            {
                State_control[3]=0x00;
                Data_Return(0x04);
            }
            
            if(State_control[4]==0x01)
            {
                State_control[4]=0x00;
                Data_Return(0x05);
            }
			if(GPS_Stop_flag==1)                    //��ȡGPS����
				{
					int i;
					GPS_Stop_flag=0;			
					for(i=0;i<256;i++)
						GPS_Data_Temp[i]=GPS_Uart1_Data[i];
					GPS_Analysis(&gps,(u8*)GPS_Data_Temp);//�����ַ���			
				}
		}
	

}


void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}

void TIM2_IRQHandler(void)   //TIM3�ж�
{
    
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			street_lamp_Rgb(State_count[3],State_count[0],State_count[1],State_count[2]);
		}
}

void street_lamp_Rgb(u8 rgb_flag,u8 R_Val,u8 G_Val,u8 B_Val)
{
	static u8 time_val=0;
	if(rgb_flag==0x01)
	{
		time_val++;
		if(time_val==20)
		 time_val=0;
		if((R_Val/5)<time_val)
			LEDR=0;
		else
			LEDR=1;
		if((G_Val/5)<time_val)
			LEDG=0;
		else
			LEDG=1;
		if((B_Val/5)<time_val)
			LEDB=0;
		else
			LEDB=1;	
		}
	else
	{
		LEDB=1;
		LEDG=1;
		LEDR=1;
	}
}










