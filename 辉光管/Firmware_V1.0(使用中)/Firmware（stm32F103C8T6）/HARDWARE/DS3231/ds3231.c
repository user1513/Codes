#include "ds3231.h"

void Ds3231_Init(void)
{
	u8 State_Timing;
	State_Timing =DS3231_ReadOneByteFromE2PROM(); //�ж��Ƿ�����ֹͣ����
    if(State_Timing & 0x80)
			{                                   //���ۺ�ʱ����ֹͣ����������λ����Ϊ�߼�1��
        DS3231_Initialization();                                //��ʼ��ʱ��ʱ��оƬDS3231
        DS3231_SetTime(SetTime);
    }
			DS3231_GetTime(GetTime);                               //�������������Դ��·���߹���ϵͳ�����������ĵ�����Ч ÿ���ϵ��ȡһ�ε�ǰʱ����Ϣ
    if(GetTime[6] == 0x00){                                 //�����ϢΪ0x00��оƬʱ����Ҫ��ʼ��
        DS3231_Initialization();                               //��ʼ��ʱ��ʱ��оƬDS3231
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

void DS3231_Initialization(void){            //��ʼ��ʱ��оƬDS3231,��ѡ��Ҫд��ļĴ�������д����Ҫ���õ�ָ��
	Ds3231_Write_Cmd(DS3231WriteAddress,DS3231_Hour);
    IIC_Send_Byte(0x00);                //д��ָ�ʱ�ӷ�ΧΪ0-23����24Сʱ��ʽ
    IIC_Wait_Ack();
	IIC_Stop();
    Ds3231_Write_Cmd(DS3231WriteAddress,DS3231_Interrupt); 
    IIC_Send_Byte(0x04);                    //�жϼĴ�����ʼ��,�رշ����źţ��ر������ж�
    IIC_Send_Byte(0x00);                    //״̬�Ĵ�����ʼ��,ʧЧ32KHz�ź��������ƥ������
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
    IIC_Send_Byte(0x00);                          //�ر������жϱ�־λ
	IIC_Wait_Ack();
    IIC_Stop();                                   //DS3231оƬIICͨ��ֹͣ�ź�
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





