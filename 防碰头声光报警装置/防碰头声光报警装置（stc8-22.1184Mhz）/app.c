#include "app.h"

#define ULTRASONICALARMDISTANCE 100//超声波报警距离（0-65535mm）
#define ULTRASONICALARMDISTANCE_H (u8)(ULTRASONICALARMDISTANCE>>8)//超声波报警距离（0-65535mm）
#define ULTRASONICALARMDISTANCE_L (u8)ULTRASONICALARMDISTANCE//超声波报警距离（0-65535mm）
u8 ReadorWrite=0XFF;//读写状态位
u8 ComputerCmd=0X00;//上位机命令状态位
u16 ComputerData=0X0000;//上位机数据位
u8 E2promFlag=0x00;//E2PROM状态位

xdata u8 DefaultSetArry[16]={0X01,ULTRASONICALARMDISTANCE_H,ULTRASONICALARMDISTANCE_L,0X01,0X00,0X1E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};//默认参数设置

xdata u8 SaveEepromData[16];//读取EEPROM内部数据
	
#define SAVEEEPROMDATALENGTH (u8)(sizeof(SaveEepromData)/sizeof(SaveEepromData[0]))
#define DEFAULTEEPROMDATALENGTH (u8)(sizeof(DefaultSetArry)/sizeof(DefaultSetArry[0]))	
void EepromInitTest(void);//EEPROM上电检测


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
			Uart1_SendStr("超声波测量距离:/n");
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
	if(IfAddrData(0x0000,0x01)!=Enable)//判断EEPROM_ADDR(0X0000)==0X01;
    {
        ReplyError(0x0001);//发送错误(检测到EEPROM0x0000数据不等于0x01)
        if((u8)CollectiveWritData(0x0000,DefaultSetArry,SAVEEEPROMDATALENGTH,12)!=(u8)Enable);//写入默认数据
			ReplyError(0x0003);//写入数据长度超出定义存取数据长度
		if(IfAddrData(0x0000,0x01)!=Enable)
		{
			ReplyError(0x0002);//发送错误(请检查EEPROM是否正常工作，系统停止工作)
			while(1);
		}
    } 
	if((u8)(CollectiveReadData(0x0000,SaveEepromData,SAVEEEPROMDATALENGTH,12))!=(u8)Enable)
	{	
		ReplyError(0x0004);//读取数据长度超出定义存取数据长度
		while(1);
	}
		Uart1SendStrLen(SaveEepromData,12);//打印出读取的EEPROM数据（HEX）
}
	


