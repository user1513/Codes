#ifndef _MAIN_H
#define _MAIN_H

#define GUESSINGVERDICT 0x01       //��ȭ�о�
#define VERDICTPUNCH    0x02	   //��ȭ�о�

#define STONE 0x03                 //ʯͷ
#define SCISSORS 0X04              //����
#define CLOTH 0X05                 //��

#define FIRST 0x05                 //һ
#define SECOND 0X06                //��
#define THIRD 0x07                 //��
#define FOURTH 0X08                //��
#define FIFTH 0X09                 //��	


#define DECISIONMODEL 0X0A             //�о�ģʽ
#define LEARNMODEL 0X0B                 //ѧϰģʽ

#define JINGSAIMOS 0X0D                //����ģʽ


#define SCISSORSLEARN 0X10			//ʯͷѧϰ
#define STONELEARN 0X11				//����ѧϰ
#define CLOTHLEARN 0X12				//��ѧϰ
#define FIRSTLEARN 0X13				//һѧϰ
#define SECONDLEARN 0X14			//��ѧϰ
#define THIRDLEARN 0X15				//��ѧϰ
#define FOURTHLEARN 0X16			//��ѧϰ
#define FIFTHLEARN 0X17				//��ѧϰ


#define CHANNELDATA0  0X20			//ͨ��������
#define CHANNELDATA1  0X21			//ͨ��һ����
#define CHANNELDATA2  0X22			//ͨ��������
#define CHANNELDATA3  0X23			//ͨ��������
#define CHANNELDATA4  0X24			//ͨ��������
#define CHANNELDATA5  0X25			//ͨ��������
enum 
{
	Disable,
	Enable
};


extern u8 TestModeState;  //����ģʽ״ֵ̬
extern u8 PageNumState;			   //�л�ҳ��״ֵ̬

extern u8 TestKeyVal;                //��������0X01��ȭ����ֵ 0X02��ȭ����ֵ��
extern u8 LearnKeyVal;               //��������ѧϰģʽ�¼�ֵ��

extern float res[];//����ֵ
extern float Parameter_Set[];

extern u8 CompetitionKeyVal;

#endif


