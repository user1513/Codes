 #include "adc.h"
 #include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
u16 ADC_Val[3]={0};  
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitTypeState;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	DMA_InitTypeState.DMA_BufferSize=3;
	DMA_InitTypeState.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitTypeState.DMA_M2M=DMA_M2M_Disable;
	DMA_InitTypeState.DMA_MemoryBaseAddr=(u32)&ADC_Val;
	DMA_InitTypeState.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_InitTypeState.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitTypeState.DMA_Mode=DMA_Mode_Circular;
	DMA_InitTypeState.DMA_PeripheralBaseAddr=(u32)&ADC1->DR;
	DMA_InitTypeState.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_InitTypeState.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitTypeState.DMA_Priority=DMA_Priority_High;
	DMA_Init(DMA1_Channel1,&DMA_InitTypeState);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 3;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	ADC_DMACmd(ADC1,ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1,1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4,2, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5,3, ADC_SampleTime_239Cycles5 );
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch,u8 RANK)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, RANK, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u8 ch,u8 RANK,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch,RANK);
		delay_ms(5);
	}
	return temp_val/times;
} 	 



























