#ifndef _MY1680U_H
#define _MY1680U_H

#include "pbdata.h"


///////////////////////////////////////length->3
#define PLAYMODE 0X11           //播放
#define SUSPENDMODE 0X12	    //暂停
#define THENEXTMODE 0X13        //下一曲
#define THELASTMODE 0X14        //上一曲
#define VOLUMEADDMODE 0X15      //音量加
#define VolMODE 0x16            //音量减
#define RESTMODE 0X19           //复位
#define FASTFORWARDMODE 0X1A	//快进
#define FASTBACKMODE  0X1B 		//快退
#define PLAYORSUSPENDMODE 0X1C 	//播放or暂停
#define STOPMODE 0X1E  		    //停止


///////////////////////////////////////length->4
#define VOLUMESETMODE 0X31  //设置音量
#define EQSETMODE 0X32      //设置EQ
///////////////////////////////////////length->5
#define CHOOSEPLAYSONGS 0X41//选择播放曲目

///////////////////////////////////////
#define QUERYSTATEMODE  0X20  //查询播放模式
#define QUERYVOLUMEMODE  0X21 //查询音量大小
#define QUERYEQMODE  0X22     //查询当前EQ
#define QUERYPLAYSONGS 0X2E   //查询播放曲目

///////////////////////////////////////
#define EQNOMODE 0X00      //EQ NO 模式
#define EQPOPMODE 0X01	   //EQ POP 模式
#define EQROCKMODE 0X02    //EQ ROCK 模式
#define EQJAZZMODE 0X03    //EQ JAZZ 模式
#define EQCLASSICMODE 0X04 //EQ CLASSIC 模式
#define EQBASSMODE 0X05    //EQ BASS 模式

//////////////////////////////////////
#define DATALENGTH3 0X03   //发送数据长度3
#define DATALENGTH4 0X04   //发送数据长度4
#define DATALENGTH5 0X05   //发送数据长度5
#define DATANONE 0X00      //发送数据为空

/////////////////////////////////////
u8 XorCheck(u8 *_able,u8 length);//XOR校验
//起始码 长度 操作码 校验码 结束码
myenum My1680uControl(u8 _cmd,
					u8 _cmdlength,
					u16 _data);//My1680u数据发送函数


typedef void(*pFuncSystickCallBack)(u8 *_data,u8 _length);//设置回调函数类型
//设置my1680u.h内部自用回调函数
void MYU1680U_SetCallBack(u32 ulTimeout,
                          pFuncSystickCallBack callBack,
                          u8 *_para,
						  u8 _length);

 void HAL_MYU1680U_Callback(void);//中断回调函数

void musicplaymode(u8 num);

extern myenum My1680uFlag;

#endif

