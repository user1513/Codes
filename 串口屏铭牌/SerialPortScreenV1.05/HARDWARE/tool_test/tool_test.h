#ifndef _TOOL_TEST
#define _TOOL_TEST

#include "app.h"

// "Ӳ"��ʱ���б�

#define CONFIG_ONLYONE 0X00000000   //����ִ��
#define CONFIG_LOOP    0X00000001		//�ظ�ִ��

// ��ʱ��״̬
typedef enum _tTimerState
{
    tTimerCreated,          // ��ʱ���Ѿ�����
    tTimerStarted,          // ��ʱ���Ѿ�����    
    tTimerRunning,          // ��ʱ������ִ�лص�����
    tTimerStopped,          // ��ʱ���Ѿ�ֹͣ
    tTimerDestroyed         // ��ʱ���Ѿ�����
}tTimerState;
    

typedef  void (*pFuncSystickCallBack) (void * arg);

typedef struct _tTimer
{
    
	    
    tNode linkNode;						 // ������
	
    uint32_t durationTicks;    // ���ڶ�ʱʱ������tick��
    uint32_t delayTicks;       // ��ǰ��ʱ�ݼ�����ֵ
    
    pFuncSystickCallBack  timerFunc;//���ûص�����
    void * arg;           // ���ݸ��ص������Ĳ���
     
    uint32_t config;      // ��ʱ�����ò���
    tTimerState state;    // ��ʱ��״̬
	
		uint8_t Count;				//��ʱ��������
		uint8_t Length;       //���泤���йر���
}tTimer;

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
											uint32_t _config);
/**********************************************************************************************************
** Function name        :   tTimerStart
** Descriptions         :   ������ʱ��
** parameters           :   timer �ȴ������Ķ�ʱ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerStart (tTimer * timer);
/**********************************************************************************************************
** Function name        :   tTimerStop
** Descriptions         :   ��ֹ��ʱ��
** parameters           :   timer �ȴ������Ķ�ʱ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerStop (tTimer * timer);
/**********************************************************************************************************
** Function name        :   tTimerDestroy
** Descriptions         :   ���ٶ�ʱ��
** parameters           :   timer ���ٵĶ�ʱ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerDestroy (tTimer * timer);
/**********************************************************************************************************
** Function name        :   tTimerCallFuncList
** Descriptions         :   ����ָ���Ķ�ʱ���б����ø�����ʱ��������
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerCallFuncList (tList * timerList);
/**********************************************************************************************************
** Function name        :   tTimerModuleInit
** Descriptions         :   ��ʱ��ģ���ʼ��
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void tTimerModuleInit (void);


#endif
