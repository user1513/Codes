/**
 * 主程序
 * 作者：    hhelib
 * 最后更新：2016-03-12
 */
#include "config.h"
#include "Display.h"
#include "TimeUpdate.h"
#include "SD2405ALPI.h"
#include "app.h"
#include "delay.h"
#include "timer.h"
#include "RTC.h"
#include "Sound.h"
#include "Exti.h"
#include "PinDef.h"
#include "intrins.h"

// 储存当前时间、闹钟、工作模式的全局变量，在 "app.h" 中声明
// 时间
uint8_t hour = 12;
uint8_t minute = 59;
uint8_t second = 59;
uint8_t per25ms = 0;//计时用变量
// 闹钟
uint8_t hour_alarm = 12;
uint8_t minute_alarm = 1;
uint8_t second_alarm = 1;
uint8_t alarmEnable = 0;
// 夜间模式
uint8_t sleepMode = 1;
// 模式，有时钟模式和自定义显示模式
uint8_t mode = MODE_CLOCK;//MODE_DISPLAY
uint8_t set[8] = {10,10,10,10,10,10,10,10};//存储设定的时间信息，两个间隔位是无效的，默认显示杠
int digit = 9;//counter for the current digit being set
int num_set = 1;//the number to be set 
int set_mode_flag = 0;//= 0 normal mode = 1 set mode
int set_digit = 7;
long time_to_Set = 0;
int set_en = 0;
int digit_jmp = 0;
int wait_count = 0;
long time_correct_count = 0;//periodical time correction

void Set_time(void);
void EXTI_config(void);

sbit KEY_1 = P3^3;
sbit KEY_2 = P3^6;
sbit KEY_3 = P3^7;

extern unsigned char keyboard_self(); 
extern void key_process();

int  main(void)
{
	// 各种初始化
	EA = 0;
	displayInit();
	timerInit();
	//soundInit();
  //EXTI_config();
	Delay_ms(250);
	Delay_ms(250);
	Delay_ms(250);
	Delay_ms(250);
	getTime();
	EA = 1;
  while(1)
	{
		time_correct_count ++;
		/*adjust time under manually*/
		if(set_mode_flag == 1){
		/*function to display the adjustment process*/
		Set_time();
		}
		/*adjust time every*/
		if(time_correct_count == 100000ul){
		getTime();
		time_correct_count = 0;
		}
		/*function to display current time*/
		displayTime();
		key_process();
	}
}

#if 0
void Ext_INT2 (void) interrupt INT2_VECTOR		//进中断时已经清除标志
{
		EA = 0;
		//INT_CLKO &= 0xEF;
		Delay_ms(100000);
	  //wait_count = 0;
		if(set_mode_flag == 0){
		set_mode_flag = 1;
		digit = 9;
		set_digit = 7;
		}
		else{
		set_en = 1;
		}
		if(digit == 0){
		set_mode_flag = 0;
		}	
		digit --;
		//INT_CLKO &= 0x10;
		EA = 1;
}

void Ext_INT3 (void) interrupt INT3_VECTOR		//进中断时已经清除标志
{
		EA = 0;
		//INT_CLKO &= 0xDF;
		Delay_ms(100000);
	  //wait_count = 0;
		num_set ++;
		if(num_set == 10)
		num_set = 0;
		//INT_CLKO |= 0xB20;
		EA = 1;
}

void Ext_INT1 (void) interrupt INT1_VECTOR		//进中断时已经清除标志
{
		EA = 0;
		//INT_CLKO &= 0xBF;
		Delay_ms(100000);
	  //wait_count = 0;
		num_set --;
		if(num_set == -1)
		num_set = 9;
		//INT_CLKO |= 0x40;
		EA = 1;
}
#endif

void Set_time(void)
{
	//'digit' is for display and 'digit_set' is for backgrounding settings
  while(1)
	{
		key_process();
		if(digit == 6){
			digit = 5;
		}
		if(digit == 3){
			digit = 2;
		}
		
				//display '-' while in setting mode 
				if(second % 2 == 0)//blink every seconds
				{
					/*
					wait_count++;
					if(wait_count == 50){
					wait_count = 0;
					set_mode_flag = 0;
					return;
					}
					*/
					displayNumber(digit,num_set);//blink the digit set
					displayNumber(9,11);
					if(digit != 8)
					displayNumber(8,set[7]);
					if(digit != 7)
					displayNumber(7,set[6]);
																																																					
					displayNumber(6,10);
					if(digit != 5)
					displayNumber(5,set[4]);
					if(digit != 4)
					displayNumber(4,set[3]);
				
					displayNumber(3,10);	
					if(digit != 2)
					displayNumber(2,set[1]);
					if(digit != 1)
					displayNumber(1,set[0]);				
				}
			else
				{
					displayNumber(digit,12);//blank
					displayNumber(9,11);
					if(digit != 8)
					displayNumber(8,set[7]);
					if(digit != 7)
					displayNumber(7,set[6]);
				
					displayNumber(6,10);
					if(digit != 5)
					displayNumber(5,set[4]);
					if(digit != 4)
					displayNumber(4,set[3]);
				
					displayNumber(3,10);
					if(digit != 2)
					displayNumber(2,set[1]);
					if(digit != 1)
					displayNumber(1,set[0]);				
				}
			//every time 'set' is pushed setting digit shift	
			if(set_en == 1){	
				set_en = 0;				
				if(set_digit == 5){
					set_digit = 4;//skip '-'
				}
				if(set_digit == 2){
					set_digit = 1;	
				}				
				set[set_digit --] = num_set;					

			}
			if(digit < 0)
			{
				if(set[7] < 3 && set[4] < 6 && set[1] <6){/*validate*/
				time_to_Set = set[7]*100000ul+set[6]*10000ul+set[4]*1000+set[3]*100+set[1]*10+set[0];
				}
				else{
				return;
				}
				SD2405ALPI_SetTime(time_to_Set);
				num_set = 0;
				getTime();
				set[7] = 10;
				set[6] = 10;
				set[4] = 10;
				set[3] = 10;
				set[1] = 10;
				set[0] = 10;
				set_digit = 7;
				digit = 9;
				return;
			}
	}
}

/*
sbit KEY_1 = P3^3;
sbit KEY_2 = P3^6;
sbit KEY_3 = P3^7;
*/
//no key pushed -> num_key=0 some key pushed -> num_key 
extern unsigned char keyboard_self() 
{ 
        unsigned char num_key=0;//key No.
        static unsigned char temp_code_1 = 0;
				static unsigned char temp_code_2 = 0;
				static unsigned char temp_code_3 = 0;
				static unsigned char key_value = 0;
        static unsigned char num_check=0;//de-vibrate 
        static unsigned char key_flag=0;//validity flag 

        temp_code_1 = (P3&0x08) >> 3;
        temp_code_2 = (P3&0x40) >> 6;
        temp_code_3 = (P3&0x80) >> 7;
	
        if(temp_code_1 == 0x00 || temp_code_2 == 0x00 || temp_code_3 == 0x00)
        { 
                num_check ++; 
                if(num_check == 4)
                { 
                        key_flag=1;
                        if(temp_code_1 == 0x00){
												key_value = 1;
												temp_code_1 = 0;
												}
												else if(temp_code_2 == 0x00){
												key_value = 2;
												temp_code_2 = 0;	
												}
												else if(temp_code_3 == 0x00){
												key_value = 3;
												temp_code_3 = 0;
												}
                } 
        } 
        else//release key
        { 
                num_check=0; 
                if(key_flag==1)
                { 
                        key_flag=0;//clear flag
                        switch(key_value) 
                        { 
                                case 1: num_key=1;
                                           break; 
                                case 2: num_key=2;
                                           break; 
                                case 3: num_key=3; 
                                           break; 
                        } 
                } 
        } 
        return(num_key); 
}

extern void key_process()
{
	  static int key_value_get = 0;
		/*check key*/
		key_value_get = keyboard_self();
		if(key_value_get == 1){
			/*K1*/
			num_set --;
			if(num_set == -1){
			num_set = 9;
			}
		}
		else if(key_value_get == 2){
			/*K2*/
					if(set_mode_flag == 0){
					set_mode_flag = 1;
					digit = 9;
					set_digit = 7;
					}
					else{
					set_en = 1;
					}
					if(digit == 0){
					set_mode_flag = 0;
					}	
					digit --;
					}
		else if(key_value_get == 3){
			/*K3*/
					num_set ++;
					if(num_set == 10)
					num_set = 0;
		}		
}