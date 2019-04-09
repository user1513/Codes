#ifndef _TOOL_TEST
#define _TOOL_TEST

#include "app.h"

// "硬"定时器列表

#define CONFIG_ONLYONE 0X00000000   //单次执行
#define CONFIG_LOOP    0X00000001		//重复执行

// 定时器状态
typedef enum _tTimerState
{
    tTimerCreated,          // 定时器已经创建
    tTimerStarted,          // 定时器已经启动    
    tTimerRunning,          // 定时器正在执行回调函数
    tTimerStopped,          // 定时器已经停止
    tTimerDestroyed         // 定时器已经销毁
}tTimerState;
    

typedef  void (*pFuncSystickCallBack) (void * arg);

typedef struct _tTimer
{
    
	    
    tNode linkNode;						 // 链表结点
	
    uint32_t durationTicks;    // 周期定时时的周期tick数
    uint32_t delayTicks;       // 当前定时递减计数值
    
    pFuncSystickCallBack  timerFunc;//设置回调函数
    void * arg;           // 传递给回调函数的参数
     
    uint32_t config;      // 定时器配置参数
    tTimerState state;    // 定时器状态
	
		uint8_t Count;				//定时器计数器
		uint8_t Length;       //保存长度有关变量
}tTimer;

/**********************************************************************************************************
** Function name        :   tTimerInit
** Descriptions         :   初始化定时器
** parameters           :   _timer 等待初始化的定时器
** parameters           :   _durationTicks 给周期性定时器用的周期tick数，一次性定时器无效
** parameters           :   _timerFunc 定时器回调函数
** parameters           :   _arg 传递给定时器回调函数的参数
** parameters           :   _config 定时器的初始配置
** Returned value       :   无
***********************************************************************************************************/
void tTimerInit(tTimer *_timer,
                      uint32_t _durationTicks,
                      pFuncSystickCallBack _timerFunc,
                      void *_arg,
											uint32_t _config);
/**********************************************************************************************************
** Function name        :   tTimerStart
** Descriptions         :   启动定时器
** parameters           :   timer 等待启动的定时器
** Returned value       :   无
***********************************************************************************************************/
void tTimerStart (tTimer * timer);
/**********************************************************************************************************
** Function name        :   tTimerStop
** Descriptions         :   终止定时器
** parameters           :   timer 等待启动的定时器
** Returned value       :   无
***********************************************************************************************************/
void tTimerStop (tTimer * timer);
/**********************************************************************************************************
** Function name        :   tTimerDestroy
** Descriptions         :   销毁定时器
** parameters           :   timer 销毁的定时器
** Returned value       :   无
***********************************************************************************************************/
void tTimerDestroy (tTimer * timer);
/**********************************************************************************************************
** Function name        :   tTimerCallFuncList
** Descriptions         :   遍历指定的定时器列表，调用各个定时器处理函数
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void tTimerCallFuncList (tList * timerList);
/**********************************************************************************************************
** Function name        :   tTimerModuleInit
** Descriptions         :   定时器模块初始化
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void tTimerModuleInit (void);


#endif
