#include "djiReceiver_dr16.h"


#define ORIGINCH0 1024
#define ORIGINCH1 1024

extern u16 G_dr16_ch0_value ;
extern u16 G_dr16_ch1_value ;
extern u16 G_dr16_ch2_value ;
extern u16 G_dr16_ch3_value ;
extern u8 Artilleryflag;
unsigned char xdata G_dr16_array[25] = {
	0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00
};
u8 G_dr16_count = 0;

u8 flag = 0;

u16 NumberOfFetching(u8 _value, u8 _bit)
{
	u8 number = 0;
	u8 mask = 0;

	switch(_bit)
	{
		case 1:
			mask = 0x01 << 0;
			number = _value & mask;
			number = number >> 0;
			return number;
		case 2:
			mask = 0x01 << 1;
			number = _value & mask;
			number = number >> 1;
			return number;
		case 3:
			mask = 0x01 << 2;
			number = _value & mask;
			number = number >> 2;
			return number;
		case 4:
			mask = 0x01 << 3;
			number = _value & mask;
			number = number >> 3;
			return number;
		case 5:
			mask = 0x01 << 4;
			number = _value & mask;
			number = number >> 4;
			return number;
		case 6:
			mask = 0x01 << 5;
			number = _value & mask;
			number = number >> 5;
			return number;
		case 7:
			mask = 0x01 << 6;
			number = _value & mask;
			number = number >> 6;
			return number;
		case 8:
			mask = 0x01 << 7;
			number = _value & mask;
			number = number >> 7;
			return number;
		default:
			return 2;
	}
}


void DR16DataAnalysis(u8 _data)
{

	
	if(flag != Enable)
	{
		//SendUart1Data(G_dr16_array[G_dr16_count]);
			G_dr16_array[G_dr16_count] = _data;
		switch(G_dr16_count)
		{
			case 0:
				if(G_dr16_array[G_dr16_count] == 0x0F)
				{
					G_dr16_count ++;
				}else
				{
					G_dr16_count = 0;
				}
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				G_dr16_count ++;
				//Uart2SendStr("OK");
				break;
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
				G_dr16_count ++;
				//Uart2SendStr("OK");
				break;
			case 23:
				if(G_dr16_array[G_dr16_count] == 0x00)
				{
					G_dr16_count ++;
				}else
				{
					G_dr16_count = 0;
				}
				break;
			case 24:
				if(G_dr16_array[G_dr16_count] == 0x00)
				{
					
					G_dr16_ch0_value  = NumberOfFetching(G_dr16_array[2], 3) * 1024;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[2], 2) * 512;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[2], 1) * 256;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[1], 8) * 128;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[1], 7) * 64;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[1], 6) * 32;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[1], 5) * 16;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[1], 4) * 8;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[1], 3) * 4;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[1], 2) * 2;
					G_dr16_ch0_value += NumberOfFetching(G_dr16_array[1], 1) * 1;
					
					
					G_dr16_ch1_value  = NumberOfFetching(G_dr16_array[3], 6) * 1024;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[3], 5) * 512;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[3], 4) * 256;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[3], 3) * 128;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[3], 2) * 64;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[3], 1) * 32;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[2], 8) * 16;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[2], 7) * 8;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[2], 6) * 4;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[2], 5) * 2;
					G_dr16_ch1_value += NumberOfFetching(G_dr16_array[2], 4) * 1;

					G_dr16_ch2_value  = NumberOfFetching(G_dr16_array[6], 4) * 1024;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[6], 3) * 512;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[6], 2) * 256;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[6], 1) * 128;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[5], 8) * 64;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[5], 7) * 32;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[5], 6) * 16;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[5], 5) * 8;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[5], 4) * 4;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[5], 3) * 2;
					G_dr16_ch2_value += NumberOfFetching(G_dr16_array[5], 2) * 1;

					G_dr16_ch3_value  = NumberOfFetching(G_dr16_array[5], 1) * 1024;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[4], 8) * 512;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[4], 7) * 256;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[4], 6) * 128;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[4], 5) * 64;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[4], 4) * 32;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[4], 3) * 16;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[4], 2) * 8;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[4], 1) * 4;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[3], 8) * 2;
					G_dr16_ch3_value += NumberOfFetching(G_dr16_array[3], 7) * 1;

					if ((G_dr16_array[7] == 0xC0) && (G_dr16_array[8] == 0x02) && (G_dr16_array[9] == 0x17) && (G_dr16_array[10] == 0x18))
					{
						//原位
						
					}
					else if ((G_dr16_array[7] == 0xC0) && (G_dr16_array[8] == 0xFF) && (G_dr16_array[9] == 0x14))
					{
						//HL
						
					}
					else if ((G_dr16_array[7] == 0x40) && (G_dr16_array[8] == 0x00) && (G_dr16_array[9] == 0x16))
					{
						
						//CL
					}
					
					if ((G_dr16_array[7] == 0xC0) && (G_dr16_array[8] == 0x02) && (G_dr16_array[9] == 0x17) && (G_dr16_array[10] == 0x18))
					{
						//原位
					}
					else if ((G_dr16_array[8] == 0x02) && (G_dr16_array[9] == 0x03) && (G_dr16_array[10] == 0x10))
					{
						//ATTI中
					}
					else if ((G_dr16_array[8] == 0x02) && (G_dr16_array[9] == 0xFF) && (G_dr16_array[10] == 0x07))
					{
						//ATTI下
					}

					//滑轮
					if ((G_dr16_array[7] == 0xC0) && (G_dr16_array[6] == 0x08))
					{
						//"滑轮原位";
						
					}
					else if ((G_dr16_array[7] == 0xC2) && (G_dr16_array[6] == 0x38))
					{
						//滑轮上
						
					}
					else if ((G_dr16_array[7] == 0xE9) && (G_dr16_array[6] == 0x48))
					{
						//滑轮下
						
					}
					//Uart1_SendStr("OK");
					flag = Enable;
					ES=0;
				}
				G_dr16_count = 0;
				break;
			default:
				G_dr16_count = 0;
				break;
		}
	}
}



//modbus_CRC16校验
u16 ModBus_CRC(u8 *arg,u8 length)
{
    u16 Reg_CRC=0xffff;
    u8 Temp_Reg=0x00;
    u8 i,j;
    for(i=0;i<length;i++)
    {
        Reg_CRC ^= *arg++;
        for(j=0;j<8;j++)
        {
            if(Reg_CRC&0x0001)
            {
                Reg_CRC=Reg_CRC>>1^0xA001;
            }
            else
            {
                Reg_CRC >>=1;
            }
        }
    }
    Temp_Reg=Reg_CRC;
    Reg_CRC=(Reg_CRC&0xff00)|Temp_Reg;
    return Reg_CRC;
}


void Send_dji_Data(u8 _cmd,u8 _data0,u8 _data1)
{
    u8 Data_able[8];
    u16 Check_val=0;
	 Data_able[0]=0x55;
	 Data_able[1]=0xAA;     
	 Data_able[2]=_cmd;  
	 Data_able[3]=_data0;  
	 Data_able[4]=_data1;  
	 Data_able[5]=0XFF;    
	 Check_val=ModBus_CRC(Data_able,6);     
	 Data_able[6]=Check_val;  
	 Data_able[7]=Check_val>>8;   
	 Uart2SendStrLen(Data_able,8);
	 
}




u8 last_Val=0xFF,qjflag=0,htflag=0;

u8 Remote_Control(u16 ch0,u16 ch1,u8 data1,u8 data2)
{
	u8 Remote_Control_Val=0XFF;
	data2=0;
	if((data1&0x01)==0)
	{
		while(1)
		{
		  if(ch0==1024&&ch1==1024) break;
		  if(ch0>(ORIGINCH0+500)&&(ch1<(ORIGINCH1+275)&&ch1>(ORIGINCH1-275)))  {Remote_Control_Val=Angle_0;break;}
		  if(ch0<(ORIGINCH0-500)&&(ch1<(ORIGINCH1+275)&&ch1>(ORIGINCH1-275)))   {Remote_Control_Val=Angle_180;break;}
		  if(ch1>(ORIGINCH0+500)&&(ch0<(ORIGINCH0+275)&&ch0>(ORIGINCH0-275)))  {Remote_Control_Val=Angle_90;break;}
		  if(ch1<(ORIGINCH0-500)&&(ch0<(ORIGINCH0+275)&&ch0>(ORIGINCH0-275)))   {Remote_Control_Val=Angle_270;break;}
		  if(ch1>1500&&ch0>1500) 			  {Remote_Control_Val=Angle_45;break;}
		  if(ch1>1500&&ch0<500) 			  {Remote_Control_Val=Angle_135;break;}
		  if(ch0>1500&&ch1<500) 			  {Remote_Control_Val=Angle_315;break;}
		  if(ch0<500&&ch1<500) 			      {Remote_Control_Val=Angle_225;break;}
		  break;
		}
		
		if(last_Val==Angle_90||qjflag)
		{
			if(ch1>1250&&ch0>1250) 			  {Remote_Control_Val=AngleR_360;qjflag=1;}
			else if(ch1>1250&&ch0<750) 			  {Remote_Control_Val=AngleL_360;qjflag=1;}
			else qjflag=0;
		}
		if(last_Val==Angle_270||htflag)
		{
			if(ch0>1250&&ch1<750) 			  {Remote_Control_Val=AngleL_360;htflag=1;}
			else if(ch0<750&&ch1<750) 			      {Remote_Control_Val=AngleR_360;htflag=1;}
			else htflag=0;
		}		
		last_Val=Remote_Control_Val;
	}
	else if((data1&0x01)==0x01)
	{
		while(1)
		{
		  if(ch0==1024&&ch1==1024) break;
		  if(ch0>(ORIGINCH0+500)&&(ch1<(ORIGINCH1+275)&&ch1>(ORIGINCH1-275)))  {Remote_Control_Val=AngleR_360;break;}
		  if(ch0<(ORIGINCH0-500)&&(ch1<(ORIGINCH1+275)&&ch1>(ORIGINCH1-275)))   {Remote_Control_Val=AngleL_360;break;}
		  if(ch1>(ORIGINCH0+500)&&(ch0<(ORIGINCH0+275)&&ch0>(ORIGINCH0-275)))  {Remote_Control_Val=Angle_90;break;}
		  if(ch1<(ORIGINCH0-500)&&(ch0<(ORIGINCH0+275)&&ch0>(ORIGINCH0-275)))   {Remote_Control_Val=Angle_270;break;}
		  break;
		}
	}
	return Remote_Control_Val;
}





int tabletennisdata(u16 ch2,u16 ch3,int _angleval)
{
	while(1)
	{
		if(ch2==1024&&ch3==1024) break;
		if(ch2>1500&&(ch3<1100&&ch3>1000))  {_angleval-=2;break;}
		if(ch2<500&&(ch3<1100&&ch3>1000))   {_angleval+=2;break;}
		if(ch3<500&&(ch2<1100&&ch2>1000))   {_angleval=tabletennisangleInit;break;}
		break;
	}
	return _angleval;
}

void Artillerydata(u16 ch2,u16 ch3)
{
	if(ch3>(ORIGINCH0+500)&&(ch2<(ORIGINCH0+275)&&ch2>(ORIGINCH0-275)))  
	{
		Artilleryflag|=0x01;
	}
	else
	{
		Artilleryflag|=0x80;
	}
}



//u8 Remote_Control(u16 ch0,u16 ch1,u8 data1,u8 data2)
//{
//	u8 Remote_Control_Val=0XFF;
//	data2=0;
//	if((data1&0x01)==0)
//	{
//		while(1)
//		{
//		  if(ch0==1024&&ch1==1024) break;
//		  if(ch0>1500&&(ch1<1100&&ch1>1000))  {Remote_Control_Val=Angle_0;break;}
//		  if(ch0<500&&(ch1<1100&&ch1>1000))   {Remote_Control_Val=Angle_180;break;}
//		  if(ch1>1500&&(ch0<1100&&ch0>1000))  {Remote_Control_Val=Angle_90;break;}
//		  z
//		  if(ch1>1500&&ch0>1500) 			  {Remote_Control_Val=Angle_45;break;}
//		  if(ch1>1500&&ch0<500) 			  {Remote_Control_Val=Angle_135;break;}
//		  if(ch0>1500&&ch1<500) 			  {Remote_Control_Val=Angle_315;break;}
//		  if(ch0<500&&ch1<500) 			      {Remote_Control_Val=Angle_225;break;}
//		  break;
//		}
//		
//		if(last_Val==Angle_90||qjflag)
//		{
//			if(ch1>1250&&ch0>1250) 			  {Remote_Control_Val=AngleR_360;qjflag=1;}
//			else if(ch1>1250&&ch0<750) 			  {Remote_Control_Val=AngleL_360;qjflag=1;}
//			else qjflag=0;
//		}
//		if(last_Val==Angle_270||htflag)
//		{
//			if(ch0>1250&&ch1<750) 			  {Remote_Control_Val=AngleL_360;htflag=1;}
//			else if(ch0<750&&ch1<750) 			      {Remote_Control_Val=AngleR_360;htflag=1;}
//			else htflag=0;
//		}
//		
//		last_Val=Remote_Control_Val;		
//	}
//	else if((data1&0x01)==0x01)
//	{
//		while(1)
//		{
//		  if(ch0==1024&&ch1==1024) break;
//		  if(ch0>1500&&(ch1<1100&&ch1>1000))  {Remote_Control_Val=AngleR_360;break;}
//		  if(ch0<500&&(ch1<1100&&ch1>1000))   {Remote_Control_Val=AngleL_360;break;}
//		  if(ch1>1500&&(ch0<1100&&ch0>1000))  {Remote_Control_Val=Angle_90;break;}
//		  if(ch1<500&&(ch0<1100&&ch0>1000))   {Remote_Control_Val=Angle_270;break;}
//		  break;
//		}
//	}
//	return Remote_Control_Val;
//}





