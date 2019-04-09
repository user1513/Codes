#include "app.h"


extern tList tTimerHardList;

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
											uint32_t _config)
{
	   tNodeInit(&_timer->linkNode);
	
    _timer->state = tTimerCreated;	
	
    _timer->timerFunc = _timerFunc;
	
    _timer->arg = _arg;
	
    _timer->durationTicks = _durationTicks;    // 周期定时时的周期tick数
	
    _timer->delayTicks = _durationTicks;       // 当前定时递减计数值     
	
    _timer->config = _config;      // 定时器配置参数
	
		_timer->Count = 0;
	
		_timer->Length = 0;
  
}

/**********************************************************************************************************
** Function name        :   tTimerStart
** Descriptions         :   启动定时器
** parameters           :   timer 等待启动的定时器
** Returned value       :   无
***********************************************************************************************************/
void tTimerStart (tTimer * timer)
{
    switch (timer->state)
    {
        case tTimerCreated:
        case tTimerStopped:
						timer->delayTicks = timer->durationTicks;       // 当前定时递减计数值 
            timer->state = tTimerStarted;

            // 根据定时器类型加入相应的定时器列表
            tListAddLast(&tTimerHardList, &timer->linkNode);
            break;
        default:
            break;
    }
}

/**********************************************************************************************************
** Function name        :   tTimerStop
** Descriptions         :   终止定时器
** parameters           :   timer 等待启动的定时器
** Returned value       :   无
***********************************************************************************************************/
void tTimerStop (tTimer * timer)
{
    switch (timer->state)
    {
        case tTimerStarted:
        case tTimerRunning:

            // 从硬定时器列表中移除
            tListRemove(&tTimerHardList, &timer->linkNode);
            timer->state = tTimerStopped;
            break;
        default:
            break;
    }
}

/**********************************************************************************************************
** Function name        :   tTimerDestroy
** Descriptions         :   销毁定时器
** parameters           :   timer 销毁的定时器
** Returned value       :   无
***********************************************************************************************************/
void tTimerDestroy (tTimer * timer)
{
    tTimerStop(timer);
    timer->state = tTimerDestroyed;
}

/**********************************************************************************************************
** Function name        :   tTimerCallFuncList
** Descriptions         :   遍历指定的定时器列表，调用各个定时器处理函数
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void tTimerCallFuncList (tList * timerList)
{
    tNode * node;
    
    // 检查所有任务的delayTicks数，如果不0的话，减1。
    for (node = timerList->headNode.nextNode; node != &(timerList->headNode); node = node->nextNode)
    {
        tTimer * timer = tNodeParent(node, tTimer, linkNode);

        // 如果延时已到，则调用定时器处理函数
        if ((timer->delayTicks == 0) || (--timer->delayTicks == 0))
        {
            // 切换为正在运行状态
            timer->state = tTimerRunning;

            // 调用定时器处理函数
            timer->timerFunc(timer->arg);

            // 切换为已经启动状态
            timer->state = tTimerStarted;

            if (timer->config > 0)
            {
                // 如果是周期性的，则重复延时计数值
                timer->delayTicks = timer->durationTicks;
            }
            else if(timer->delayTicks == 0)
            {
                // 否则，是一次性计数器，中止定时器
                tListRemove(timerList, &timer->linkNode);
                timer->state = tTimerStopped;
            }
        }
    }
}


/**********************************************************************************************************
** Function name        :   tTimerModuleInit
** Descriptions         :   定时器模块初始化
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void tTimerModuleInit (void)
{
    tListInit(&tTimerHardList);
}




























