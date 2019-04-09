#ifndef _SerialPortScreen_H
#define _SerialPortScreen_H

#define RECEIVINGTIMEOUTSTATE 1
#define CHECKERRORSTATE 2


#define SIZELENGTH_ONE 1
#define SIZELENGTH_TWO 2
#define SIZELENGTH_THREE 3
#define SIZELENGTH_FOUR 4

typedef enum
{
	Version = 0, //版本号
	LED_NOW,		 //LED亮度（0x00-0x40）
	BZ_TIME, 		 //蜂鸣器控制
	PIC_ID_H,		 //页面切换
  PIC_ID_L,		 //页面切换
	TP_Flag,		 //触摸屏坐标更新
	TP_Status,	 //按键状态
	TCP_Enable = 0x0b,//触屏使能
	Music_Set = 0x50, //音乐设置
	Music_Num_H,			//播放音乐数
	Music_Num_L,			//播放音乐数
	Vol_Adj_En,				//申请音量调节	
	Vol,							//音量值
	Vol_Status,				//音频状态
	
	Reg_Com_Write = 0x80,  //写寄存器指令
	Reg_Com_Red ,					 //读寄存器指令
	Variable_Reg_Com_Write,//写变量寄存器指令
	Variable_Reg_Com_Red,	 //读变量寄存器指令
}_SerialPortScreen_reg;	



void SerialPortScreen_Usart_Send(uint8_t *reg, uint8_t length);

void SerialPortScreen_State(uint32_t _SerialPortScreen_Satae);

void SerialPortScreen_ReadWrite_Reg_com(uint8_t mode, uint8_t reg_addr, uint8_t SizeLength, uint32_t Data);

void SerialPortScreen_ReadWrite_Variable_Reg_com(uint8_t mode, uint16_t reg_addr, uint8_t SizeLength, int32_t Data);

void SerialPortScreen_Send_China_Language(uint16_t reg_addr,uint8_t SizeLength, const char *Data);
#endif





