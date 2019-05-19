#include "trajectory.h"
#include "main.h"
#include "motor.h"
#include "control.h"
#include "delay.h"
#include "led.h"


//Car_Traversing(); 速度为+是前进，-是后退，(500,-200,1200,+600)  代表左轮后退500个码盘值，速度为200，右轮前进1200个码盘值，速度为600


u8 Turn_left_90_value = 33;
u8 Turn_right_90_value = 42;

void Turn_left_90_degrees() //左转90
{
//	Car_Traversing(390*3.3,-3000,390*3.3,3000);
	Car_Traversing(390*(Turn_left_90_value/10.0),-3000,390*(Turn_left_90_value/10.0),3000);
}

void Turn_right_90_degrees()//右转90
{
	Car_Traversing(390*(Turn_right_90_value/10.0),3000,390*(Turn_right_90_value/10.0),-3000);
}

void Garage_4()//停   4号车位
{
    u8 i;
	Car_Traversing(390*13,2600,390*13,2600);
	delay_ms(1000);
	Turn_right_90_degrees();
	delay_ms(1000);
	Car_Traversing(390*4.9,3000,390*4.9,-3000);
	delay_ms(1000);
	Car_Traversing(390*5*1.15,-2600,390*5*0.85,-2600);
	//Car_Traversing(390*8,-2600,390*8,-2600);
	delay_ms(300);
	for(i=0;i<12;i++)
	{
		OnboardLED=~OnboardLED;
		delay_ms(250);
	}
	Car_Traversing(390*21,2600,390*21,2600);
	
}

void Garage_3()//停   3号车位
{
	 u8 i;
	Car_Traversing(390*13,2600,390*13,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*1.8,2600,390*1.8,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*5*1.25,-2600,390*5*0.75,-2600);	
	//Car_Traversing(390*6,-2600,390*6,-2600);	
	delay_ms(300);
	for(i=0;i<12;i++)
	{
		OnboardLED=~OnboardLED;
		delay_ms(250);
	}
	Car_Traversing(390*6,2600,390*6,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(1000);
	Car_Traversing(390*1.4,2600,390*1.4,2600);
	delay_ms(500);
	Turn_right_90_degrees();
	delay_ms(1000);
	Car_Traversing(390*13,2600,390*13,2600);
}

void Garage_2()//停   2号车位
{
	u8 i;
	Car_Traversing(390*13,2600,390*13,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*6.4,2600,390*6.4,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*5.5*1.25,-2600,390*5.5*0.75,-2600);	
	//Car_Traversing(390*7,-2600,390*7,-2600);
	delay_ms(300);
	for(i=0;i<12;i++)
	{
		OnboardLED=~OnboardLED;
		delay_ms(250);
	}
	Car_Traversing(390*6.5,2600,390*6.5,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*5.5,2600,390*5.5,2600);
	delay_ms(500);
	Turn_right_90_degrees();
	delay_ms(1000);
	Car_Traversing(390*13,2600,390*13,2600);
	
}

void Garage_1()//停1号车位
{
	u8 i;
	Car_Traversing(390*13,2600,390*13,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*11.5,2600,390*11.5,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*5.5,-2600,390*5.5,-2600);	
	//Car_Traversing(390*7,-2600,390*7,-2600);
	delay_ms(300);
	for(i=0;i<12;i++)
	{
		OnboardLED=~OnboardLED;
		delay_ms(250);
	}
	Car_Traversing(390*6,2600,390*6,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*11.2,2600,390*11.2,2600);
	delay_ms(500);
	Turn_right_90_degrees();
	delay_ms(500);
	Car_Traversing(390*13,2600,390*13,2600);
}

void Garage_5()//停5号车位
{
	u8 i;
	Car_Traversing(390*13,2600,390*13,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*10.9,2600,390*10.9,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*2.8,2600,390*2.8,2600);	
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(300);
	for(i=0;i<12;i++)
	{
		OnboardLED=~OnboardLED;
		delay_ms(250);
	}
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*2,2600,390*2,2600);
	delay_ms(500);
	Turn_right_90_degrees();
	delay_ms(500);
	Car_Traversing(390*10,2600,390*10
	,2600);
	delay_ms(500);
	Turn_right_90_degrees();
	delay_ms(500);
	Car_Traversing(390*0.1,3000,390*0.1,-3000);
	delay_ms(500);
	Car_Traversing(390*13,2600,390*13,2600);
}

void Garage_6()//停6号车位
{
	u8 i;
	Car_Traversing(390*13,2600,390*13,2600);
	delay_ms(500);
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*3.5,2600,390*3.5,2600);
	delay_ms(500);
	Turn_right_90_degrees();
	delay_ms(500);	
	Car_Traversing(390*3.08,-2600,390*3.08,-2600);
	//Car_Traversing(390*4.08,-2600,390*4.08,-2600);
	delay_ms(500);
	Turn_right_90_degrees();
	delay_ms(500);
	for(i=0;i<12;i++)
	{
		OnboardLED=~OnboardLED;
		delay_ms(250);
	}
	Turn_left_90_degrees();
	delay_ms(500);
	Car_Traversing(390*3,2600,390*3,2600);
	delay_ms(500);
	Turn_right_90_degrees();
	delay_ms(500);
	Car_Traversing(390*2,2600,390*2,2600);
	delay_ms(500);
	Turn_right_90_degrees();
	delay_ms(500);
	Car_Traversing(390*14,2600,390*14,2600);
}

extern u8 Collision;

void CarRun(u8 garage)
{
	Collision = 0;
	switch(garage)
	{
		case 1:
			Garage_1();
			break;	
		case 2:
			Garage_2();
			break;
		case 3:
			Garage_3();
			break;
		case 4:
			Garage_4();
			break;
		case 5:
			Garage_5();
			break;
		case 6:
			Garage_6();
			break;
		default:
			break;
	}
}

