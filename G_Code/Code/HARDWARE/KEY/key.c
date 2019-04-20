#include "key.h"

#define KEYTIMEOUT 400

#define LEVEL 0
uint8_t previous_key1 = 1 ;		//����洢��һ�ΰ���ֵ��̬����
uint8_t key1_flag = 0;			//���尴��״̬��̬����
uint16_t key1_TimeOut = 0;

uint8_t previous_key2 = 1 ;		//����洢��һ�ΰ���ֵ��̬����
uint8_t key2_flag = 0;			//���尴��״̬��̬����
//����io��ʼ��
void Key_Io_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE); //ʹ��PD �˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;		//GPIO_Pin_14,15		     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 		//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOC.14,15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;		//GPIO_Pin_14,15		     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 		//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOC.14,15

}


//�����¼�������

uint8_t Key_Event_Handler(void)
{
	uint8_t key_val = 1;
	key_val |= 1 << BUTTONLONGPRESS(1, KEY_1, &key1_TimeOut, &previous_key1, &key1_flag) ;
	key_val |= 1 << BUTTONPOINTSMOVE(3, KEY_2, &previous_key2, &key2_flag);
	return key_val - 1;
}


/**********************************************************************************************************
** Function name        :   Key_Get
** Descriptions         :   ����ֵ��ȡ����
** parameters           :   mode ������ȡģʽ��0�㶯��1������
** parameters           :   key_num ���ð�������ֵ
** parameters           :   current_key ���밴��ֵ
** parameters           :   timeout ��ʱʱ��
** parameters           :   previous_key ��һ�ΰ���ֵ
** parameters           :   key_flag ������־λ
** Returned value       :   ���·����趨����ֵ����֮����0
***********************************************************************************************************/
	
uint8_t Key_Get(uint8_t mode, uint8_t key_num, uint8_t current_key, uint16_t * keyTimeOut,uint8_t* previous_key,uint8_t *key_flag)
{
	
	if(current_key == (!LEVEL) && *previous_key == (!LEVEL) && *key_flag == 0)   	//�жϵ�ǰ����ֵ��֮ǰ�����Ƿ�Ϊ�͵�ƽ�����жϱ�־λ�Ƿ�Ϊ0
	{
		if(keyTimeOut != (uint16_t *)0)								
			*keyTimeOut = 0;											
		*key_flag = 1;
	}
	if(*key_flag == 1 && mode == 0)									//�����������ж�ģʽ				
	{
		*key_flag = 2;
		return key_num;
	}
	else if(*key_flag == 1 && mode)									//�����������ж�ģʽ
	{
		if(key_Long_press_test(keyTimeOut) == 0x01)					//���ð���������⺯��
		{
			*key_flag = 2;
			return key_num + 1;
		}
	}
	*previous_key = current_key; 				//���汾�ε�ֵ
	if((*previous_key == LEVEL) && *key_flag)
	{	
		if(mode == 1 && *key_flag == 1)
		{
			*key_flag = 0;
			return key_num;
		}
		*key_flag = 0;	
	}	
	return 0;
}

uint8_t key_Long_press_test(uint16_t * keyTimeOut)  //�����������
{
	if((*keyTimeOut) ++ > KEYTIMEOUT)	//�ж��Ƿ�ʱ�����ĺ궨��KEYTIMEOUT���Ըĳ�ʱʱ�䣩
	{
		*keyTimeOut = 0;
		return 1;					//��ʱ���1
	}
	return 0xff;					//����ʱ���0xff
}

