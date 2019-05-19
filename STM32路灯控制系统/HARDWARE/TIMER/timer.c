#include "timer.h"

extern u8 Usart2_table[];

extern u8 Sensor_data[];

extern u8 Equip_State;
unsigned char GPS_Data_Temp[256];

gps_process_data gps;     //GPS结构体
/************************************************
 基于Zigbee的路灯控制系统
 工程模板
 注意，这是timer定时器函数 
 //修改日期:2018/4/6
 //版本：V1.0
 作者：袁有成
************************************************/


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
u8 State_control[8];
extern u8 State_count[];//接收到RGB值与路灯开关值
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序

	u16 State_1ms=0,Heartbeat_val=0;
    u8 State_1s=0;
u16 Light_value=0;
void TIM3_IRQHandler(void)   //TIM3中断
{
    
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
			State_1ms++;
			if(State_1ms==500)
			{
				State_1ms=0;
                Real_Time_Data();
				LED0=!LED0;
                State_1s++;
               bh1750fvi_Write(0x46,0x10);                    //BH1750FVI连续高分辨模式
                Light_value=bh1750fvi_Read(0x47);
                Sensor_data[15]=Light_value>>8;       //获取BH1750FVI高8位数据
                Sensor_data[16]=Light_value;          //获取BH1750FVI低8位数据
                //printf("光照值：%d",Light_value);
                DHT11_Read_Data(&Sensor_data[13],&Sensor_data[14]);//获取DHT11中的温湿度值
                //printf("温度：%d℃,湿度：%d%\n",Sensor_data[11],Sensor_data[12]);
				Sensor_data[19]=((double)((3.3*ADC_Val[0]*100)/4096));
				//GPS函数		
				if(gps.gpssta!=0)	
				{
					//Usart3_String_Send("经度： 纬度： GPS状态： 小时：分钟：秒：");
					//Usart3_Send_Byte('\n');
					Sensor_data[9]=gps.longitude/1000000;  //经度
					Sensor_data[10]=(gps.longitude%1000000)>>16;
					Sensor_data[11]=(gps.longitude%1000000)>>8;
					Sensor_data[12]=(gps.longitude%1000000);
					Sensor_data[5]=gps.latitude/1000000;   //纬度
					Sensor_data[6]=(gps.latitude%1000000)>>16; 
					Sensor_data[7]=(gps.latitude%1000000)>>8; 
					Sensor_data[8]=(gps.latitude%1000000);

					Sensor_data[2]=gps.utc.hour;          //时间
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
            触摸屏故障检测
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
			if(GPS_Stop_flag==1)                    //获取GPS数据
				{
					int i;
					GPS_Stop_flag=0;			
					for(i=0;i<256;i++)
						GPS_Data_Temp[i]=GPS_Uart1_Data[i];
					GPS_Analysis(&gps,(u8*)GPS_Data_Temp);//分析字符串			
				}
		}
	

}


void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 
}

void TIM2_IRQHandler(void)   //TIM3中断
{
    
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
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










