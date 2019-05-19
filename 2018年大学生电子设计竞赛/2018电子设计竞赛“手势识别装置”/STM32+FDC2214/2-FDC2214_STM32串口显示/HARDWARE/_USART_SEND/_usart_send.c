#include "_usart_send.h"



/************************************************
 基于Zigbee的路灯控制系统
 工程模板
 注意，这是数据发送函数 
 //修改日期:2018/4/6
 //版本：V1.0
 作者：袁有成
************************************************/

//void Data_Return(u8 num)  //0x0101 -- 0x0105  -- 0x0109 -- 0x010D -- 0X0111
//{
//    switch (num)
//    {
//        case 0x01:
//            if(Equip_State&0x02)
//				Send_China_Data(0x0101,"设备故障");
//			 else
//				Send_China_Data(0x0101,"工作正常"); 
//        break;
//        case 0x02:
//            if(Equip_State&0x04)
//				Send_China_Data(0x0105,"设备故障");
//			 else
//				Send_China_Data(0x0105,"工作正常"); 
//        break;   
//        case 0x03:
//            if(Equip_State&0x08)
//                Send_China_Data(0x0109,"设备故障");
//			 else
//				Send_China_Data(0x0109,"工作正常"); 
//        break;   
//        case 0x04:
//            if(Equip_State&0x10)
//                Send_China_Data(0x010d,"设备故障");
//			 else
//				Send_China_Data(0x010d,"工作正常"); 
//        break;      
//        case 0x05:
//            if(Equip_State&0x20)
//                Send_China_Data(0x0111,"设备故障");
//			 else
//				Send_China_Data(0x0111,"工作正常"); 
//        break;      
//        default :break;
//    }
//}

void Test_result(u8 TestMode,u8 PageNum)
{
	switch(TestMode)
	{
		case GUESSINGVERDICT :
			switch(PageNum)
			{
				case STONE:Send_Page_Switch(0X03,0X0003);
					break;
				case SCISSORS:Send_Page_Switch(0X03,0X0002);
					break;
				case CLOTH:Send_Page_Switch(0X03,0X0004);
					break;
				default :break;
			}
			break;
		case VERDICTPUNCH    :
			switch(PageNum)
			{
				case FIRST:Send_Page_Switch(0X03,0X0005);
					break;
				case SECOND:Send_Page_Switch(0X03,0X0006);
					break;
				case THIRD:Send_Page_Switch(0X03,0X000C);
					break;
				case FOURTH:Send_Page_Switch(0X03,0X000D);
					break;
				case FIFTH:Send_Page_Switch(0X03,0X0011);
					break;
				default :break;
			}
			break;
		default :break;
	}
}

//Send_Page_Switch(0x03,0x0013);//屏幕刷新
//Send_Page_Switch(0x03,0x0014);//屏幕刷新
void Send_China_Demo(void)
{
	//Send_China_Data(0x0100,"开始学习");
	Send_China_Data(0x0100,"开始学习");
	Send_China_Data(0x0104,"开始学习");
	Send_China_Data(0x0108,"开始学习");
	Send_China_Data(0x010C,"开始学习");
	Send_China_Data(0x0110,"开始学习");
	Send_China_Data(0x1000,"开始学习");
	Send_China_Data(0x1004,"开始学习");
	Send_China_Data(0x011C,"开始学习");
	
	Send_Number_Data(0X0210,(u16)(Parameter_Set[0]*100));
	delay_ms(10);
	Send_Number_Data(0X0212,(u16)(Parameter_Set[1]*100));
	delay_ms(10);
	Send_Number_Data(0X0214,(u16)(Parameter_Set[2]*100));
	delay_ms(10);
	Send_Number_Data(0X0216,(u16)(Parameter_Set[3]*100));
	delay_ms(10);
	Send_Number_Data(0X0218,(u16)(Parameter_Set[4]*100));
	delay_ms(10);
	Send_Number_Data(0X021A,(u16)(Parameter_Set[5]*100));
	delay_ms(10);
}

//从MCU中发送页面切换通过ModbusCRC16检验
void Send_Page_Switch(u8 addr ,u16 Pageaddr)
{
    u8 Data_able[9];
    u16 Check_val=0;
	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x06;  
	 Data_able[3]=0x80;  
	 Data_able[4]=addr;  
	 Data_able[5]=Pageaddr>>8;  
	 Data_able[6]=Pageaddr;  
	    
     Check_val=ModBus_CRC(Data_able+3,4);     
	 Data_able[8]=Check_val>>8;  
	 Data_able[7]=Check_val;   
     Usart2_String_Send1(Data_able,9);
}

int resdata[6];

void SendFdc2214Data(void)
{
	resdata[0]=res[0]*100;
	resdata[1]=res[1]*100;
	resdata[2]=res[2]*100;
	resdata[3]=res[3]*100;
	resdata[4]=res[4]*100;
	resdata[5]=res[5]*100;
	
	Send_Number_Data(0X0200,resdata[0]);
	Send_Number_Data(0X0202,resdata[1]);
	Send_Number_Data(0X0204,resdata[2]);
	Send_Number_Data(0X0206,resdata[3]);
	Send_Number_Data(0X0208,resdata[4]);
	Send_Number_Data(0X020A,resdata[5]);
}

//从MCU中发送数据（ascll）到串口屏通过ModbusCRC16检验
void Send_Number_Data(u16 addr,u16 num)
{
    u8 Data_able[10];
    u16 Check_val=0;
	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x07;  
	 Data_able[3]=0x82;  
	 Data_able[4]=addr>>8;  
	 Data_able[5]=addr;  
	 Data_able[6]=num>>8;  
	 Data_able[7]=num;   
     Check_val=ModBus_CRC(Data_able+3,5);     
	 Data_able[9]=Check_val>>8;  
	 Data_able[8]=Check_val;   
     Usart2_String_Send1(Data_able,10);
}

void Send_Number_Data_u32 (u16 addr,u32 num)
{
    u8 Data_able[12];
    u16 Check_val=0;
	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x09;  
	 Data_able[3]=0x82;  
	 Data_able[4]=addr>>8;  
	 Data_able[5]=addr; 
	 Data_able[6]=num>>24;	
	 Data_able[7]=num>>16;
	 Data_able[8]=num>>8;  
	 Data_able[9]=num;   
     Check_val=ModBus_CRC(Data_able+3,7);     
	 Data_able[11]=Check_val>>8;  
	 Data_able[10]=Check_val;   
     Usart2_String_Send1(Data_able,12);
}


//从MCU中发送数据（汉字）到串口屏通过ModbusCRC16检验
void Send_China_Data(u16 addr,u8 *data)
{
    u8 num=0,i=0;
    u8 Data_able[20];
    u8 Data_able1[12];//最多发送6个汉字
    u16 Check_val=0;
    while(*data!='\0')
    {
       
       Data_able1[num]=*data;//0x02;//*(data++);
       num++; 
       data++;
    }

	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x05+num;  
	 Data_able[3]=0x82;  
	 Data_able[4]=addr>>8;  
	 Data_able[5]=addr;  
     while(num>=i)
     {        
        Data_able[6+i]=Data_able1[i];   
        i++;
     }
     Check_val=ModBus_CRC(Data_able+3,0x03+num);     
	 Data_able[num+7]=Check_val>>8;  
	 Data_able[num+6]=Check_val;   
     Usart2_String_Send1(Data_able,8+num);
    
}










