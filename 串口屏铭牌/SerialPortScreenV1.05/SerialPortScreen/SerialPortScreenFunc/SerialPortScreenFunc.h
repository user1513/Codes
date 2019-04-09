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
	Version = 0, //�汾��
	LED_NOW,		 //LED���ȣ�0x00-0x40��
	BZ_TIME, 		 //����������
	PIC_ID_H,		 //ҳ���л�
  PIC_ID_L,		 //ҳ���л�
	TP_Flag,		 //�������������
	TP_Status,	 //����״̬
	TCP_Enable = 0x0b,//����ʹ��
	Music_Set = 0x50, //��������
	Music_Num_H,			//����������
	Music_Num_L,			//����������
	Vol_Adj_En,				//������������	
	Vol,							//����ֵ
	Vol_Status,				//��Ƶ״̬
	
	Reg_Com_Write = 0x80,  //д�Ĵ���ָ��
	Reg_Com_Red ,					 //���Ĵ���ָ��
	Variable_Reg_Com_Write,//д�����Ĵ���ָ��
	Variable_Reg_Com_Red,	 //�������Ĵ���ָ��
}_SerialPortScreen_reg;	



void SerialPortScreen_Usart_Send(uint8_t *reg, uint8_t length);

void SerialPortScreen_State(uint32_t _SerialPortScreen_Satae);

void SerialPortScreen_ReadWrite_Reg_com(uint8_t mode, uint8_t reg_addr, uint8_t SizeLength, uint32_t Data);

void SerialPortScreen_ReadWrite_Variable_Reg_com(uint8_t mode, uint16_t reg_addr, uint8_t SizeLength, int32_t Data);

void SerialPortScreen_Send_China_Language(uint16_t reg_addr,uint8_t SizeLength, const char *Data);
#endif





