#include "dht11.h"

/************************************************
 基于Zigbee的路灯控制系统
 工程模板
 注意，这是DHT11驱动函数 
 //修改日期:2018/4/6
 //版本：V1.0
 作者：袁有成
************************************************/

void Dht11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeInit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitTypeInit.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitTypeInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitTypeInit);
}

void Dht11_Reset(void)
{
	dht11_Out();
	dht11_DQ_Out=0;
	delay_ms(20);
	dht11_DQ_Out=1;
	delay_us(30);
}

u8 Dht11_Check(void)
{
	u8 res=0,i=0;
	Dht11_Reset();
	dht11_In();
	while(dht11_DQ_In&&i<100)
	{
		i++;
		delay_us(1);
	}
	if(i>=90) res=1;
	i=0;
	while(!dht11_DQ_In&&i<100)
	{
		i++;
		delay_us(1);
	}
	if(i>=90) res=1;
	return res;
}

u8 Dht11_data(void)
{
	u8 res=0,i=0;
	while(dht11_DQ_In&&i<100)
	{
		i++;
		delay_us(1);
	}
	if(i>=90) res=2;
	i=0;
	while(!dht11_DQ_In&&i<100)
	{
		i++;
		delay_us(1);
	}
	if(i>=90) res=2;
	delay_us(50);
	if(res!=2)
	{
		if(dht11_DQ_In==1) res=1;
		else res=0;
	}
	return res;
}

u8 DHT11_Read_Byte(void)  
{
	u8 i=0,data=0;
	for(i=0;i<8;i++)
	{
		data <<=1;
		data|=Dht11_data();
		
	}
	return data;
	
	/*
	u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
	*/
}

u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	while(Dht11_Check()!=0);
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]&&buf[4]!=0)
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	return 0;	    
}


