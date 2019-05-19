#include "ds3231.h"

void Ds3231_Init(void)
{
	u8 State_Timing;
	State_Timing =DS3231_ReadOneByteFromE2PROM(); //判断是否振荡器停止工作
    if(State_Timing & 0x80)
			{                                   //无论何时振荡器停止工作，第七位均置为逻辑1。
        DS3231_Initialization();                                //初始化时间时钟芯片DS3231
        DS3231_SetTime(SetTime);
    }
			DS3231_GetTime(GetTime);                               //掉电检测操作，电源短路或者供电系统电量彻底消耗掉才有效 每次上电读取一次当前时间信息
    if(GetTime[6] == 0x00){                                 //年份信息为0x00，芯片时间需要初始化
        DS3231_Initialization();                               //初始化时间时钟芯片DS3231
        DS3231_SetTime(SetTime);                               
		}
}

void Ds3231_Write_Cmd(u8 slave_addr,u8 word_addr)
{
	IIC_Start();
	IIC_Send_Byte(slave_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(word_addr);
	IIC_Wait_Ack();
}

void Ds3231_Read_Data(u8 slave_addr)
{
	IIC_Start();
	IIC_Send_Byte(slave_addr);
	IIC_Wait_Ack();
}

u8 DS3231_ReadOneByteFromE2PROM(void)
{
	u8 ReadData=0;
	Ds3231_Write_Cmd(DS3231WriteAddress,DS3231_Status);
	Ds3231_Read_Data(DS3231ReadAddress);
	ReadData=IIC_Read_Byte(0);
	IIC_Stop();
	return ReadData;
}

void DS3231_Initialization(void){            //初始化时钟芯片DS3231,先选择要写入的寄存器，在写入需要设置的指令
	Ds3231_Write_Cmd(DS3231WriteAddress,DS3231_Hour);
    IIC_Send_Byte(0x00);                //写入指令，时钟范围为0-23，即24小时制式
    IIC_Wait_Ack();
	IIC_Stop();
    Ds3231_Write_Cmd(DS3231WriteAddress,DS3231_Interrupt); 
    IIC_Send_Byte(0x04);                    //中断寄存器初始化,关闭方波信号，关闭闹钟中断
    IIC_Send_Byte(0x00);                    //状态寄存器初始化,失效32KHz信号输出，不匹配闹钟
    IIC_Wait_Ack();
	IIC_Stop();
}

void DS3231_SetTime(int *time_data)
{
	u8 Number = 0x00;
	int TransitionData = 0;
	u8 TransitionData1 = 0;
	Ds3231_Write_Cmd(DS3231WriteAddress,DS3231_TimeFirst);
	for(Number=0;Number<7;Number++)
	{
		TransitionData=*time_data++;
		TransitionData1=((TransitionData/10)<<4)+(TransitionData%10);
		IIC_Send_Byte(TransitionData1);
		IIC_Wait_Ack();
	}
	IIC_Stop();
}

void DS3231_GetTime(int *Get_Time)
{
	u8 Number  = 7;
	u8 Time    = 0x00;
    Ds3231_Write_Cmd(DS3231WriteAddress,DS3231_Status);
    IIC_Send_Byte(0x00);                          //关闭闹钟中断标志位
	IIC_Wait_Ack();
    IIC_Stop();                                   //DS3231芯片IIC通信停止信号
	Ds3231_Write_Cmd(DS3231WriteAddress,DS3231_TimeFirst);
    Ds3231_Read_Data(DS3231ReadAddress);
  for(Number=0; Number<6; Number++) {
        Time =IIC_Read_Byte(1);
    *Get_Time++ = (int)(Time>>4)*10+(int)(Time&0x0f);    
  }
    Time = IIC_Read_Byte(0);
    *Get_Time++ = (int)(Time>>4)*10+(int)(Time&0x0f);    
    IIC_Stop();                               
}





