版本号：MainControlBoardV2stc8-24MHz-V1.0

更新日期：2018-08-29

更新内容：增加模式切换  
<pbdata.h> #define MotorMode P17 
<djiReceiver_dr16.h>
if(MotorMode==0) Data_able[10] |=0x01;//判断P17口是高低电平
else Data_able[10] &=~0x01;	
默认低电平为普通小车,高电平为麦克纳姆轮
