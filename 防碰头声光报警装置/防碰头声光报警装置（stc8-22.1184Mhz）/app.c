#include "app.h"

#define ULTRASONICALARMDISTANCE 100//�������������루0-65535mm��
#define ULTRASONICALARMDISTANCE_H (u8)(ULTRASONICALARMDISTANCE>>8)//�������������루0-65535mm��
#define ULTRASONICALARMDISTANCE_L (u8)ULTRASONICALARMDISTANCE//�������������루0-65535mm��
u8 ReadorWrite=0XFF;//��д״̬λ
u8 ComputerCmd=0X00;//��λ������״̬λ
u16 ComputerData=0X0000;//��λ������λ
u8 E2promFlag=0x00;//E2PROM״̬λ

xdata u8 DefaultSetArry[16]={0X01,ULTRASONICALARMDISTANCE_H,ULTRASONICALARMDISTANCE_L,0X01,0X00,0X1E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};//Ĭ�ϲ�������

xdata u8 SaveEepromData[16];//��ȡEEPROM�ڲ�����
	
#define SAVEEEPROMDATALENGTH (u8)(sizeof(SaveEepromData)/sizeof(SaveEepromData[0]))
#define DEFAULTEEPROMDATALENGTH (u8)(sizeof(DefaultSetArry)/sizeof(DefaultSetArry[0]))	
void EepromInitTest(void);//EEPROM�ϵ���


_bool EquipmentInit = TRUE;
_bool EquipmentFault = TRUE;

void main()
{
	int numi = 0;
	u16 jl = 0;
	Timer0Init();
	Uart1_Init();
	Uart2Init();
	musicplaymode(1);
	Delayms(5000);
	My1680uControl(STOPMODE, DATALENGTH3, DATANONE);
	while(1)
	{
		if(UltrasonicArray[8] == 0XFF)
		{
			UltrasonicArray[8] = 0X00;	
			jl = (UltrasonicArray[4] << 8) + UltrasonicArray[5];
			Uart1_SendStr("��������������:/n");
//			Uart1_Send((char)((jl%10000)/1000+'0'));
//			Uart1_Send((char)((jl%1000)/100+'0'));	
//			Uart1_Send((char)((jl%100)/10+'0'));
//			Uart1_Send((jl%10)+'0');
//			Uart1_Send(1+'0');
//			Uart1_Send(1+'0');	
//			Uart1_Send(1+'0');
//			Uart1_Send(1+'0');
//			Uart1_SendStr("mm/n");
//			Uart1SendStrLen(UltrasonicArray, 9);
		}
		if(UartCmdArray[11] == 0XFF)
		{
			UartCmdArray[11]=0X00;
			ReadorWrite=UartCmdArray[3];
			ComputerCmd=UartCmdArray[4];
			ComputerData=UartCmdArray[5]<<8+UartCmdArray[6];
			E2promFlag=UartCmdArray[7];
			if(ReadorWrite==COMWRITECMD)
				ReplyAck(ComputerCmd);
			else if(ReadorWrite==COMREADCMD)
				ReplyReadCmd(ComputerCmd,ComputerData);
			//Uart1SendStrLen(UartCmdArray,12);
		}
		if(100 < jl && EquipmentInit == TRUE)
		{
			CleanCallbackFunc(&EquipmentFault, FALSE);
			EquipmentInit = FALSE;
			EquipmentFault = TRUE;
			musicplaymode(3);
			FuncReg(&EquipmentInit, 254000, TRUE);
		}
		else if(EquipmentFault == TRUE && 100 >= jl)
		{
			CleanCallbackFunc(&EquipmentInit, FALSE);
			EquipmentInit = TRUE;
			EquipmentFault = FALSE;
			musicplaymode(2);
			FuncReg(&EquipmentFault, 4000, TRUE);
		}
	}
}

void EepromInitTest(void)
{
	if(IfAddrData(0x0000,0x01)!=Enable)//�ж�EEPROM_ADDR(0X0000)==0X01;
    {
        ReplyError(0x0001);//���ʹ���(��⵽EEPROM0x0000���ݲ�����0x01)
        if((u8)CollectiveWritData(0x0000,DefaultSetArry,SAVEEEPROMDATALENGTH,12)!=(u8)Enable);//д��Ĭ������
			ReplyError(0x0003);//д�����ݳ��ȳ��������ȡ���ݳ���
		if(IfAddrData(0x0000,0x01)!=Enable)
		{
			ReplyError(0x0002);//���ʹ���(����EEPROM�Ƿ�����������ϵͳֹͣ����)
			while(1);
		}
    } 
	if((u8)(CollectiveReadData(0x0000,SaveEepromData,SAVEEEPROMDATALENGTH,12))!=(u8)Enable)
	{	
		ReplyError(0x0004);//��ȡ���ݳ��ȳ��������ȡ���ݳ���
		while(1);
	}
		Uart1SendStrLen(SaveEepromData,12);//��ӡ����ȡ��EEPROM���ݣ�HEX��
}
	


