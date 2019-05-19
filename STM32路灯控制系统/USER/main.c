#include "sys.h"
#include "led.h"
#include "delay.h"
#include "dht11.h"
#include "usart1.h"	
#include "timer.h"
#include "usart2.h"
#include "usart3.h"
#include "Data_Check.h"
#include "_usart_send.h"
#include "Bh1750fvi.h"
#include "myiic.h"
#include "gps.h"
#include "adc.h"
//数据变量
u16 source_address=0x0001;   //源地址
u8 Sensor_data[21];          //所有传感器数据
u32 Equip_State_Table[12];   //数据发送串口屏
u8 Equip_State=0x00;         //设备状态变量
u8 State_count[4]={0X64,0X64,0X64,0x00};           //接收到RGB值与路灯开关值
/************************************************
 基于Zigbee的路灯控制系统
 工程模板
 注意，这是函数main文件 
 //修改日期:2018/4/6
 //版本：V1.0
 作者：袁有成
************************************************/

int main(void)
{
    Sensor_data[0]=source_address>>8;
    Sensor_data[1]=source_address;
	Adc_Init();
	LED_Init();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    IIC_Init();
	Dht11_Init();
	while(Dht11_Check()!=0);
	bh1750fvi_Init();
	usart1_init(9600);
	Usart2_Init(115200);
    Usart3_Init(115200);
	TIM3_Int_Init(719,99);
	TIM2_Int_Init(359,9);
	while(1){
		;
	}
}
