#include "app.h"

extern uint8_t CRC_Ack_Flag;
extern tTimer ReceivingTimeout_Timer;
extern uint8_t SerialPortScreen_Reg[];

uint8_t SerialPortScreen_able[20];
void SerialPortScreen_Usart_Send(uint8_t *reg, uint8_t length)
{
	int i = 0;
	for(i = 0; i < length; i++ )
		Usart2_Send_Byte(*(reg + i));
	//CRC_Ack_Flag = 1;
	//ReceivingTimeout_Timer.arg = reg;
	//ReceivingTimeout_Timer.Length = length;
	//tTimerStart(&ReceivingTimeout_Timer);
}

void SerialPortScreen_State(uint32_t _SerialPortScreen_Satae)
{
	while(_SerialPortScreen_Satae & RECEIVINGTIMEOUTSTATE) //串口接收超时
	{
		;
	}
	while(_SerialPortScreen_Satae & CHECKERRORSTATE) //串口接收超时
	{
		;
	}
}


void SerialPortScreen_ReadWrite_Reg_com(uint8_t mode, uint8_t reg_addr, uint8_t SizeLength, uint32_t Data)
{
	int i = 0;
	
	uint8_t * _DataTransformation = (uint8_t *) &Data;
	
	uint16_t check_val = 0;
	
	SerialPortScreen_able[i++] = DATA_FRAME_HEADER >> 8;
	
	SerialPortScreen_able[i++] = DATA_FRAME_HEADER & 0x000000ff;
	
	SerialPortScreen_able[i++] = 4 + SizeLength;
	
	SerialPortScreen_able[i++] = mode;
	
	SerialPortScreen_able[i++] = reg_addr;
	
	while(SizeLength--)
		SerialPortScreen_able[i++] = *(_DataTransformation + SizeLength);
	
	check_val = ModBus_CRC(SerialPortScreen_able + 3, i - 3);
	
	SerialPortScreen_able[i++] = check_val>>8;
	
	SerialPortScreen_able[i++] = check_val;
	
	SerialPortScreen_Usart_Send(SerialPortScreen_able, i);

}


void SerialPortScreen_ReadWrite_Variable_Reg_com(uint8_t mode, uint16_t reg_addr, uint8_t SizeLength, int32_t Data)
{
	int i = 0;
		uint8_t * _DataTransformation = (uint8_t *)0;
	uint16_t check_val = 0;
	
	_DataTransformation = (uint8_t *) &Data;
	

	
	SerialPortScreen_able[i++] = DATA_FRAME_HEADER >> 8;
	
	SerialPortScreen_able[i++] = DATA_FRAME_HEADER & 0x000000ff;
	
	SerialPortScreen_able[i++] = 5 + SizeLength;
	
	SerialPortScreen_able[i++] = mode;
	
	SerialPortScreen_able[i++] = reg_addr >> 8;
	
	SerialPortScreen_able[i++] = reg_addr;
	
	while(SizeLength--)
		SerialPortScreen_able[i++] = *(_DataTransformation + SizeLength);
	
	check_val = ModBus_CRC(SerialPortScreen_able + 3, i - 3);
	
	SerialPortScreen_able[i++] = check_val>>8;
	
	SerialPortScreen_able[i++] = check_val;
	
	SerialPortScreen_Usart_Send(SerialPortScreen_able, i);

}


void SerialPortScreen_Send_China_Language(uint16_t reg_addr,uint8_t SizeLength, const char *Data)
{
	int i = 0;
	uint16_t check_val = 0;
	uint16_t SizeLength_temp = SizeLength * 2;
	

	
	SerialPortScreen_able[i++] = DATA_FRAME_HEADER >> 8;
	
	SerialPortScreen_able[i++] = DATA_FRAME_HEADER & 0x000000ff;
	
	SerialPortScreen_able[i++] = 7 + SizeLength * 2;
	
	SerialPortScreen_able[i++] = Variable_Reg_Com_Write;
	
	SerialPortScreen_able[i++] = reg_addr >> 8;
	
	SerialPortScreen_able[i++] = reg_addr;
	
	while(SizeLength_temp--)
		SerialPortScreen_able[i++] = *(Data + SizeLength * 2 - SizeLength_temp - 1);
	
	SerialPortScreen_able[i++] = 0xFF;
	
	SerialPortScreen_able[i++] = 0xFF;
	
	check_val = ModBus_CRC(SerialPortScreen_able + 3, i - 3);
	
	SerialPortScreen_able[i++] = check_val>>8;
	
	SerialPortScreen_able[i++] = check_val;
	
	SerialPortScreen_Usart_Send(SerialPortScreen_able, i);

}


















//			CRC_Ack_Flag = 1;
//			usart_data_temp[i++] = 0XA5;
//			usart_data_temp[i++] = 0X5A;
//			usart_data_temp[i++] = 0X05;
//			usart_data_temp[i++] = 0X81;
//			usart_data_temp[i++] = 0X00;
//			usart_data_temp[i++] = 0X01;
//			check_flag_temp = ModBus_CRC(usart_data_temp + 3, i - 3);
//			usart_data_temp[i++] = check_flag_temp>>8;
//			usart_data_temp[i++] = check_flag_temp;
//			SerialPortScreen_Usart_Send(usart_data_temp, i);
