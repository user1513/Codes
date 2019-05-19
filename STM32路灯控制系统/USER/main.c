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
//���ݱ���
u16 source_address=0x0001;   //Դ��ַ
u8 Sensor_data[21];          //���д���������
u32 Equip_State_Table[12];   //���ݷ��ʹ�����
u8 Equip_State=0x00;         //�豸״̬����
u8 State_count[4]={0X64,0X64,0X64,0x00};           //���յ�RGBֵ��·�ƿ���ֵ
/************************************************
 ����Zigbee��·�ƿ���ϵͳ
 ����ģ��
 ע�⣬���Ǻ���main�ļ� 
 //�޸�����:2018/4/6
 //�汾��V1.0
 ���ߣ�Ԭ�г�
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
