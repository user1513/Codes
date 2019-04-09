#include "app.h"


extern tList tTimerHardList;

/**********************************************************************************************************
** Function name        :   tTimerInit
** Descriptions         :   ��ʼ����ʱ��
** parameters           :   _timer �ȴ���ʼ���Ķ�ʱ��
** parameters           :   _durationTicks �������Զ�ʱ���õ�����tick����һ���Զ�ʱ����Ч
** parameters           :   _timerFunc ��ʱ���ص�����
** parameters           :   _arg ���ݸ���ʱ���ص������Ĳ���
** parameters           :   _config ��ʱ���ĳ�ʼ����
** Returned value       :   ��
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
	
    _timer->durationTicks = _durationTicks;    // ���ڶ�ʱʱ������tick��
	
    _timer->delayTicks = _durationTicks;       // ��ǰ��ʱ�ݼ�����ֵ     
	
    _timer->config = _config;      // ��ʱ�����ò���
	
		_timer->Count = 0;
	
		_timer->Length = 0;
  
}

/**********************************************************************************************************
** Function name        :   tTimerStart
** Descriptions         :   ������ʱ��
** parameters           :   timer �ȴ������Ķ�ʱ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerStart (tTimer * timer)
{
    switch (timer->state)
    {
        case tTimerCreated:
        case tTimerStopped:
						timer->delayTicks = timer->durationTicks;       // ��ǰ��ʱ�ݼ�����ֵ 
            timer->state = tTimerStarted;

            // ���ݶ�ʱ�����ͼ�����Ӧ�Ķ�ʱ���б�
            tListAddLast(&tTimerHardList, &timer->linkNode);
            break;
        default:
            break;
    }
}

/**********************************************************************************************************
** Function name        :   tTimerStop
** Descriptions         :   ��ֹ��ʱ��
** parameters           :   timer �ȴ������Ķ�ʱ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerStop (tTimer * timer)
{
    switch (timer->state)
    {
        case tTimerStarted:
        case tTimerRunning:

            // ��Ӳ��ʱ���б����Ƴ�
            tListRemove(&tTimerHardList, &timer->linkNode);
            timer->state = tTimerStopped;
            break;
        default:
            break;
    }
}

/**********************************************************************************************************
** Function name        :   tTimerDestroy
** Descriptions         :   ���ٶ�ʱ��
** parameters           :   timer ���ٵĶ�ʱ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerDestroy (tTimer * timer)
{
    tTimerStop(timer);
    timer->state = tTimerDestroyed;
}

/**********************************************************************************************************
** Function name        :   tTimerCallFuncList
** Descriptions         :   ����ָ���Ķ�ʱ���б����ø�����ʱ��������
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerCallFuncList (tList * timerList)
{
    tNode * node;
    
    // ������������delayTicks���������0�Ļ�����1��
    for (node = timerList->headNode.nextNode; node != &(timerList->headNode); node = node->nextNode)
    {
        tTimer * timer = tNodeParent(node, tTimer, linkNode);

        // �����ʱ�ѵ�������ö�ʱ��������
        if ((timer->delayTicks == 0) || (--timer->delayTicks == 0))
        {
            // �л�Ϊ��������״̬
            timer->state = tTimerRunning;

            // ���ö�ʱ��������
            timer->timerFunc(timer->arg);

            // �л�Ϊ�Ѿ�����״̬
            timer->state = tTimerStarted;

            if (timer->config > 0)
            {
                // ����������Եģ����ظ���ʱ����ֵ
                timer->delayTicks = timer->durationTicks;
            }
            else if(timer->delayTicks == 0)
            {
                // ������һ���Լ���������ֹ��ʱ��
                tListRemove(timerList, &timer->linkNode);
                timer->state = tTimerStopped;
            }
        }
    }
}


/**********************************************************************************************************
** Function name        :   tTimerModuleInit
** Descriptions         :   ��ʱ��ģ���ʼ��
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerModuleInit (void)
{
    tListInit(&tTimerHardList);
}




























