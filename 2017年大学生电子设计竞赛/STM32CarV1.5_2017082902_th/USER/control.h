#ifndef __CONTROL_H
#define __CONTROL_H	 

#include "sys.h"


void Car_Forward(int encoder_left_max,int left_speed,int encoder_right_max,int rightt_speed);
void Car_Reversing(int encoder_left_max,int left_speed,int encoder_right_max,int rightt_speed);
void Car_ForwardTurnCorner(int servo,int encoder_left_max,int left_speed,int encoder_right_max,int rightt_speed);
void Car_ReversingTurnCorner(int servo,int encoder_left_max,int left_speed,int encoder_right_max,int rightt_speed);
void Car_Traversing(int encoder_left_max,int left_speed,int encoder_right_max,int right_speed);
#endif
