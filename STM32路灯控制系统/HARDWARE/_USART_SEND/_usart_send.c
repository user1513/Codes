#include "_usart_send.h"

extern u8 Equip_State;   //设备状态变量
extern u32 Equip_State_Table[];
extern u8 Sensor_data[];


/************************************************
 基于Zigbee的路灯控制系统
 工程模板
 注意，这是数据发送函数 
 //修改日期:2018/4/6
 //版本：V1.0
 作者：袁有成
************************************************/

void Data_Return(u8 num)  //0x0101 -- 0x0105  -- 0x0109 -- 0x010D -- 0X0111
{
    switch (num)
    {
        case 0x01:
            if(Equip_State&0x02)
				Send_China_Data(0x0101,"设备故障");
			 else
				Send_China_Data(0x0101,"工作正常"); 
        break;
        case 0x02:
            if(Equip_State&0x04)
				Send_China_Data(0x0105,"设备故障");
			 else
				Send_China_Data(0x0105,"工作正常"); 
        break;   
        case 0x03:
            if(Equip_State&0x08)
                Send_China_Data(0x0109,"设备故障");
			 else
				Send_China_Data(0x0109,"工作正常"); 
        break;   
        case 0x04:
            if(Equip_State&0x10)
                Send_China_Data(0x010d,"设备故障");
			 else
				Send_China_Data(0x010d,"工作正常"); 
        break;      
        case 0x05:
            if(Equip_State&0x20)
                Send_China_Data(0x0111,"设备故障");
			 else
				Send_China_Data(0x0111,"工作正常"); 
        break;      
        default :break;
    }
}


void Real_Time_Data(void)
{
	
    Equip_State_Table[0]=Sensor_data[2];
    Equip_State_Table[1]=Sensor_data[3];
    Equip_State_Table[2]=Sensor_data[4];
    Equip_State_Table[3]=Sensor_data[5];
    Equip_State_Table[4]=Sensor_data[6]<<16|Sensor_data[7]<<8|Sensor_data[8];
    Equip_State_Table[5]=Sensor_data[9];
    Equip_State_Table[6]=Sensor_data[10]<<16|Sensor_data[11]<<8|Sensor_data[12];
    Equip_State_Table[7]=Sensor_data[13];
    Equip_State_Table[8]=Sensor_data[14];
    Equip_State_Table[9]=Sensor_data[15]<<8|Sensor_data[16];
    Equip_State_Table[10]=Sensor_data[20];
    Equip_State_Table[11]=Sensor_data[19]*3.3;
     Send_Number_Data(0x1000,Equip_State_Table[0]);
     Send_Number_Data(0x1001,Equip_State_Table[1]);
     Send_Number_Data(0x1002,Equip_State_Table[2]);
     Send_Number_Data(0x1003,Equip_State_Table[3]);
     Send_Number_Data_u32(0x1300,Equip_State_Table[4]);
     Send_Number_Data(0x1005,Equip_State_Table[5]);
     Send_Number_Data_u32(0x1400,Equip_State_Table[6]);
     Send_Number_Data(0x1007,Equip_State_Table[7]);
     Send_Number_Data(0x1008,Equip_State_Table[8]);
     Send_Number_Data(0x1009,Equip_State_Table[9]);
     Send_Number_Data(0x100a,Equip_State_Table[10]);
     Send_Number_Data(0x100b,Equip_State_Table[11]);
     //Data2_Send(0x100c,Equip_State_Table[10]);
     //Send_Number_Data(0x100e,Equip_State_Table[11]);      RGB值
	 if(Equip_State_Table[11]<10)
		Send_China_Data(0x100c,"设备故障");
	 else
		Send_China_Data(0x100c,"工作正常");
}

//从MCU中发送页面切换通过ModbusCRC16检验
void Send_Page_Switch(u8 addr,u16 num)
{
    u8 Data_able[9];
    u16 Check_val=0;
	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x06;  
	 Data_able[3]=0x80;  
	 Data_able[4]=addr;  
	 Data_able[5]=num>>8;  
	 Data_able[6]=num;  
	    
     Check_val=ModBus_CRC(Data_able+3,4);     
	 Data_able[8]=Check_val>>8;  
	 Data_able[7]=Check_val;   
     Usart2_String_Send1(Data_able,9);
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

//从MCU中发送数据到Zigebbe通过ModbusCRC16校验
void Send_Mcu_Zigbee(u16 addr,u8 num,u8 *data) //num=0x00 or num=0x10
{
    u8 Data_able[15],i=0;
    u16 Check_val=0;
    
    Data_able[0]=0x55;
    Data_able[1]=*data|num;
    for(i=0;i<10;i++)
    {
        Data_able[2+i]=*(data+1+i);    
    }
    Data_able[12]=0xFF;
    Check_val=ModBus_CRC(Data_able,13);     
    Data_able[13]=Check_val;
    Data_able[14]=Check_val>>8;
    Usart2_String_Send1(Data_able,15);    
}

void Send_Data_Page(u8 cState_Value)  //cPageNumber 包数，cState_Value 状态值
{
    u8 Send_Data[24];
    u16 CRC16_Val=0;
    Send_Data[0]=0x55;
	Send_Data[1]=0xAA;
    Send_Data[2]=Sensor_data[0];
    Send_Data[3]=Sensor_data[1];
	Send_Data[4]=cState_Value;
	Send_Data[5]=Sensor_data[17];
	Send_Data[6]=Sensor_data[9];
	Send_Data[7]=Sensor_data[10];
	Send_Data[8]=Sensor_data[11];
	Send_Data[9]=Sensor_data[12];
	Send_Data[10]=Sensor_data[18];
	Send_Data[11]=Sensor_data[5];
	Send_Data[12]=Sensor_data[6];
	Send_Data[13]=Sensor_data[7];
	Send_Data[14]=Sensor_data[8];
	Send_Data[15]=Sensor_data[13];
	Send_Data[16]=Sensor_data[14];
	Send_Data[17]=Sensor_data[15];
	Send_Data[18]=Sensor_data[16];
	Send_Data[19]=Sensor_data[19];
	Send_Data[20]=Sensor_data[20];
    CRC16_Val=ModBus_CRC(Send_Data,21);
    Send_Data[21]=CRC16_Val;
    Send_Data[22]=CRC16_Val>>8;
	Send_Data[23]=0xFF;
    Usart3_String_Send1(Send_Data,24);
}









