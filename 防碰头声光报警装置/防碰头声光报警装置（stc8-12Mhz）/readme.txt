�汾�ţ�MainControlBoardV2stc8-24MHz-V1.0

�������ڣ�2018-08-29

�������ݣ�����ģʽ�л�  
<pbdata.h> #define MotorMode P17 
<djiReceiver_dr16.h>
if(MotorMode==0) Data_able[10] |=0x01;//�ж�P17���Ǹߵ͵�ƽ
else Data_able[10] &=~0x01;	
Ĭ�ϵ͵�ƽΪ��ͨС��,�ߵ�ƽΪ�����ķ��
