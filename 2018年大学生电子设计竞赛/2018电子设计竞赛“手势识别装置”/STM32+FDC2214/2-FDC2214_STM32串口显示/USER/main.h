#ifndef _MAIN_H
#define _MAIN_H

#define GUESSINGVERDICT 0x01       //猜拳判决
#define VERDICTPUNCH    0x02	   //划拳判决

#define STONE 0x03                 //石头
#define SCISSORS 0X04              //剪刀
#define CLOTH 0X05                 //布

#define FIRST 0x05                 //一
#define SECOND 0X06                //二
#define THIRD 0x07                 //三
#define FOURTH 0X08                //四
#define FIFTH 0X09                 //五	


#define DECISIONMODEL 0X0A             //判决模式
#define LEARNMODEL 0X0B                 //学习模式

#define JINGSAIMOS 0X0D                //竞赛模式


#define SCISSORSLEARN 0X10			//石头学习
#define STONELEARN 0X11				//剪刀学习
#define CLOTHLEARN 0X12				//布学习
#define FIRSTLEARN 0X13				//一学习
#define SECONDLEARN 0X14			//二学习
#define THIRDLEARN 0X15				//三学习
#define FOURTHLEARN 0X16			//四学习
#define FIFTHLEARN 0X17				//五学习


#define CHANNELDATA0  0X20			//通道零数据
#define CHANNELDATA1  0X21			//通道一数据
#define CHANNELDATA2  0X22			//通道二数据
#define CHANNELDATA3  0X23			//通道三数据
#define CHANNELDATA4  0X24			//通道四数据
#define CHANNELDATA5  0X25			//通道五数据
enum 
{
	Disable,
	Enable
};


extern u8 TestModeState;  //测试模式状态值
extern u8 PageNumState;			   //切换页面状态值

extern u8 TestKeyVal;                //串口屏（0X01猜拳按键值 0X02划拳按键值）
extern u8 LearnKeyVal;               //串口屏（学习模式下键值）

extern float res[];//数据值
extern float Parameter_Set[];

extern u8 CompetitionKeyVal;

#endif


