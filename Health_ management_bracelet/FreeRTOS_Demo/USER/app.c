#include "app.h"
#include "max30102.h"
#include "algorithm.h"
#include "usart.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "timers.h"




#define Min_HeartRate	50
#define MAX_HeartRate	250


#define MAX_BRIGHTNESS 255

uint32_t aun_ir_buffer[150]; //infrared LED sensor data
uint32_t aun_red_buffer[150];  //red LED sensor data
int32_t n_ir_buffer_length; //data length
int32_t n_spo2;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

int32_t hr_buf[16];
int32_t hrSum;
int32_t hrAvg;
int32_t spo2_buf[16];
int32_t spo2Sum;
int32_t spo2Avg;
int32_t spo2BuffFilled;
int32_t hrBuffFilled;
int32_t hrValidCnt = 0;
int32_t spo2ValidCnt = 0;
int32_t hrThrowOutSamp = 0;
int32_t spo2ThrowOutSamp = 0;
int32_t spo2Timeout = 0;
int32_t hrTimeout = 0;

// the loop routine runs over and over again forever:
char max30102_run(void)
{
    uint32_t un_min, un_max, un_prev_data, un_brightness;  //variables to calculate the on-board LED brightness that reflects the heartbeats
    int32_t i;
    float f_temp;
	char count = 0;
    un_brightness = 0;
    un_min = 0x3FFFF;
    un_max = 0;

    n_ir_buffer_length = 150; //buffer length of 150 stores 3 seconds of samples running at 50sps

    //read the first 150 samples, and determine the signal range
    for(i = 0; i < n_ir_buffer_length; i++)
    {
        while(INT == 1); //wait until the interrupt pin asserts
        maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i)); //read from MAX30102 FIFO

        if(un_min > aun_red_buffer[i])
            un_min = aun_red_buffer[i]; //update signal min
        if(un_max < aun_red_buffer[i])
            un_max = aun_red_buffer[i]; //update signal max
        /*SerialUSB.print(F("red="));
        SerialUSB.print(aun_red_buffer[i], DEC);
        SerialUSB.print(F(", ir="));
        SerialUSB.println(aun_ir_buffer[i], DEC);*/
		//Send_To_PC2(aun_red_buffer[i],aun_ir_buffer[i]);
    }
    un_prev_data = aun_red_buffer[i];
    //calculate heart rate and SpO2 after first 150 samples (first 3 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

    //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
    while(1)
    {
        i = 0;
        un_min = 0x3FFFF;
        un_max = 0;

        //dumping the first 50 sets of samples in the memory and shift the last 100 sets of samples to the top
        for(i = 50; i < 150; i++)
        {
            aun_red_buffer[i - 50] = aun_red_buffer[i];
            aun_ir_buffer[i - 50] = aun_ir_buffer[i];

            //update the signal min and max
            if(un_min > aun_red_buffer[i])
                un_min = aun_red_buffer[i];
            if(un_max < aun_red_buffer[i])
                un_max = aun_red_buffer[i];
        }

        //take 50 sets of samples before calculating the heart rate.
        for(i = 100; i < 150; i++)
        {
            un_prev_data = aun_red_buffer[i - 1];
            while(INT == 1);
            maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i));

            //calculate the brightness of the LED
            if(aun_red_buffer[i] > un_prev_data)
            {
                f_temp = aun_red_buffer[i] - un_prev_data;
                f_temp /= (un_max - un_min);
                f_temp *= MAX_BRIGHTNESS;
                f_temp = un_brightness - f_temp;
                if(f_temp < 0)
                    un_brightness = 0;
                else
                    un_brightness = (int)f_temp;
            }
            else
            {
                f_temp = un_prev_data - aun_red_buffer[i];
                f_temp /= (un_max - un_min);
                f_temp *= MAX_BRIGHTNESS;
                un_brightness += (int)f_temp;
                if(un_brightness > MAX_BRIGHTNESS)
                    un_brightness = MAX_BRIGHTNESS;
            }
			//Send_To_PC2( aun_red_buffer[i], aun_ir_buffer[i] );
            //send samples and calculation result to terminal program through UART
            /*SerialUSB.print(F("red="));
            SerialUSB.print(aun_red_buffer[i], DEC);
            SerialUSB.print(F(", ir="));
            SerialUSB.print(aun_ir_buffer[i], DEC);

            SerialUSB.print(F(", HR="));
            SerialUSB.print(n_heart_rate, DEC);

            SerialUSB.print(F(", HRvalid="));
            SerialUSB.print(ch_hr_valid, DEC);

            SerialUSB.print(F(", SPO2="));
            SerialUSB.print(n_spo2, DEC);

            SerialUSB.print(F(", SPO2Valid="));
            SerialUSB.println(ch_spo2_valid, DEC);*/

            //      SerialUSB.println(aun_ir_buffer[i], DEC);
        }
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
        if ((ch_hr_valid == 1) && (n_heart_rate < 190) && (n_heart_rate > 40))
        {
            hrTimeout = 0;

            // Throw out up to 1 out of every 5 valid samples if wacky
            if (hrValidCnt == 4)
            {
                hrThrowOutSamp = 1;
                hrValidCnt = 0;
                for (i = 12; i < 16; i++)
                {
                    if (n_heart_rate < hr_buf[i] + 10)
                    {
                        hrThrowOutSamp = 0;
                        hrValidCnt   = 4;
                    }
                }
            }
            else
            {
                hrValidCnt = hrValidCnt + 1;
            }

            if (hrThrowOutSamp == 0)
            {

                // Shift New Sample into buffer
                for(i = 0; i < 15; i++)
                {
                    hr_buf[i] = hr_buf[i + 1];
                }
                hr_buf[15] = n_heart_rate;

                // Update buffer fill value
                if (hrBuffFilled < 16)
                {
                    hrBuffFilled = hrBuffFilled + 1;
                }

                // Take moving average
                hrSum = 0;
                if (hrBuffFilled < 2)
                {
                    hrAvg = 0;
                }
                else if (hrBuffFilled < 4)
                {
                    for(i = 14; i < 16; i++)
                    {
                        hrSum = hrSum + hr_buf[i];
                    }
                    hrAvg = hrSum >> 1;
                }
                else if (hrBuffFilled < 8)
                {
                    for(i = 12; i < 16; i++)
                    {
                        hrSum = hrSum + hr_buf[i];
                    }
                    hrAvg = hrSum >> 2;
                }
                else if (hrBuffFilled < 16)
                {
                    for(i = 8; i < 16; i++)
                    {
                        hrSum = hrSum + hr_buf[i];
                    }
                    hrAvg = hrSum >> 3;
                }
                else
                {
                    for(i = 0; i < 16; i++)
                    {
                        hrSum = hrSum + hr_buf[i];
                    }
                    hrAvg = hrSum >> 4;
                }
            }
            hrThrowOutSamp = 0;
        }
        else
        {
            hrValidCnt = 0;
            if (hrTimeout == 4)
            {
                hrAvg = 0;
                hrBuffFilled = 0;
            }
            else
            {
                hrTimeout++;
            }
        }

        if ((ch_spo2_valid == 1) && (n_spo2 > 59))
        {
            spo2Timeout = 0;

            // Throw out up to 1 out of every 5 valid samples if wacky
            if (spo2ValidCnt == 4)
            {
                spo2ThrowOutSamp = 1;
                spo2ValidCnt = 0;
                for (i = 12; i < 16; i++)
                {
                    if (n_spo2 > spo2_buf[i] - 10)
                    {
                        spo2ThrowOutSamp = 0;
                        spo2ValidCnt   = 4;
                    }
                }
            }
            else
            {
                spo2ValidCnt = spo2ValidCnt + 1;
            }

            if (spo2ThrowOutSamp == 0)
            {

                // Shift New Sample into buffer
                for(i = 0; i < 15; i++)
                {
                    spo2_buf[i] = spo2_buf[i + 1];
                }
                spo2_buf[15] = n_spo2;

                // Update buffer fill value
                if (spo2BuffFilled < 16)
                {
                    spo2BuffFilled = spo2BuffFilled + 1;
                }

                // Take moving average
                spo2Sum = 0;
                if (spo2BuffFilled < 2)
                {
                    spo2Avg = 0;
                }
                else if (spo2BuffFilled < 4)
                {
                    for(i = 14; i < 16; i++)
                    {
                        spo2Sum = spo2Sum + spo2_buf[i];
                    }
                    spo2Avg = spo2Sum >> 1;
                }
                else if (spo2BuffFilled < 8)
                {
                    for(i = 12; i < 16; i++)
                    {
                        spo2Sum = spo2Sum + spo2_buf[i];
                    }
                    spo2Avg = spo2Sum >> 2;
                }
                else if (spo2BuffFilled < 16)
                {
                    for(i = 8; i < 16; i++)
                    {
                        spo2Sum = spo2Sum + spo2_buf[i];
                    }
                    spo2Avg = spo2Sum >> 3;
                }
                else
                {
                    for(i = 0; i < 16; i++)
                    {
                        spo2Sum = spo2Sum + spo2_buf[i];
                    }
                    spo2Avg = spo2Sum >> 4;
                }
            }
            spo2ThrowOutSamp = 0;
        }
        else
        {
            spo2ValidCnt = 0;
            if (spo2Timeout == 4)
            {
                spo2Avg = 0;
                spo2BuffFilled = 0;
            }
            else
            {
                spo2Timeout++;
            }
        }
		if(count++ == 6)
			return (char)spo2Avg;
printf("%d\n",spo2Avg);
        //Send_To_PC(hrAvg, spo2Avg);
		
		delay_ms(1000);
//		OLED_ShowNum(8*12,6,hrAvg,4,16);
//		
//		if(hrAvg > 10)
//		{
//			if(hrAvg < Min_HeartRate)
//			{
//				BEEP = 0;		//报警
//				
//				LED2 = 1;
//				LED3 = 1;
//				LED4 = 0;
//				LED5 = 1;
//			}else if(hrAvg > MAX_HeartRate)
//			{
//				BEEP = 0;		//报警
//				
//				LED2 = 1;
//				LED3 = 1;
//				LED4 = 0;
//				LED5 = 0;
//			}else
//			{
//				BEEP = 1;		//不报警
//				
//				LED2 = 1;
//				LED3 = 0;
//				LED4 = 1;
//				LED5 = 1;
//			}
//		}else
//		{
//			BEEP = 1;		//不报警
//			
//			LED2 = 0;
//			LED3 = 1;
//			LED4 = 1;
//			LED5 = 1;
//		}
    }
}



void SetMsTimer(TimerHandle_t Timer, uint32_t xTimerPeriodInTicks)
{
	
}

BaseType_t IsMsTimerExpired(TimerHandle_t Timer)
{
	return xTimerIsTimerActive(Timer);
}

// 


//				SetMsTimer(PARA_INI_TIMER,3000);//重置3S计时
//			}
//			if(PARA_INI_TIMER))
//			{
//				StepInitialStep = ERROR_STEP;
//				ret=21;
//			}


















//#include "app.h"
//#include "max30102.h"
//#include "algorithm.h"
//#include "usart.h"





//#define Min_HeartRate	50
//#define MAX_HeartRate	250


//#define MAX_BRIGHTNESS 255

//uint32_t aun_ir_buffer[150]; //infrared LED sensor data
//uint32_t aun_red_buffer[150];  //red LED sensor data
//int32_t n_ir_buffer_length = 150; //data length
//int32_t n_spo2;  //SPO2 value
//int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
//int32_t n_heart_rate; //heart rate value
//int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
//uint8_t uch_dummy;

//int32_t hr_buf[16];
//int32_t hrSum;
//int32_t hrAvg;
//int32_t spo2_buf[16];
//int32_t spo2Sum;
//int32_t spo2Avg;
//int32_t spo2BuffFilled;
//int32_t hrBuffFilled;
//int32_t hrValidCnt = 0;
//int32_t spo2ValidCnt = 0;
//int32_t hrThrowOutSamp = 0;
//int32_t spo2ThrowOutSamp = 0;
//int32_t spo2Timeout = 0;
//int32_t hrTimeout = 0;



//// the loop routine runs over and over again forever:
//void max30102_onlyone_run(void)
//{
//	uint32_t un_min, un_max, un_prev_data;  //variables to calculate the on-board LED brightness that reflects the heartbeats
//    int32_t i;
// 

//    un_min = 0x3FFFF;
//    un_max = 0;



//    //read the first 150 samples, and determine the signal range
//    for(i = 0; i < n_ir_buffer_length; i++)
//    {
//        while(INT == 1); //wait until the interrupt pin asserts
//        maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i)); //read from MAX30102 FIFO

//        if(un_min > aun_red_buffer[i])
//            un_min = aun_red_buffer[i]; //update signal min
//        if(un_max < aun_red_buffer[i])
//            un_max = aun_red_buffer[i]; //update signal max
//        /*SerialUSB.print(F("red="));
//        SerialUSB.print(aun_red_buffer[i], DEC);
//        SerialUSB.print(F(", ir="));
//        SerialUSB.println(aun_ir_buffer[i], DEC);*/
//		//Send_To_PC2(aun_red_buffer[i],aun_ir_buffer[i]);
//    }
//    un_prev_data = aun_red_buffer[i];
//    //calculate heart rate and SpO2 after first 150 samples (first 3 seconds of samples)
//    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
//}


//void max30102_run(void)
//{
//	float f_temp;
//    //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
//	uint32_t un_min, un_max, un_prev_data, un_brightness;  //variables to calculate the on-board LED brightness that reflects the heartbeats
//    int32_t i;
//        i = 0;
//        un_min = 0x3FFFF;
//        un_max = 0;

//        //dumping the first 50 sets of samples in the memory and shift the last 100 sets of samples to the top
//        for(i = 50; i < 150; i++)
//        {
//            aun_red_buffer[i - 50] = aun_red_buffer[i];
//            aun_ir_buffer[i - 50] = aun_ir_buffer[i];

//            //update the signal min and max
//            if(un_min > aun_red_buffer[i])
//                un_min = aun_red_buffer[i];
//            if(un_max < aun_red_buffer[i])
//                un_max = aun_red_buffer[i];
//        }

//        //take 50 sets of samples before calculating the heart rate.
//        for(i = 100; i < 150; i++)
//        {
//            un_prev_data = aun_red_buffer[i - 1];
//            while(INT == 1);
//            maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i));

//            //calculate the brightness of the LED
//            if(aun_red_buffer[i] > un_prev_data)
//            {
//                f_temp = aun_red_buffer[i] - un_prev_data;
//                f_temp /= (un_max - un_min);
//                f_temp *= MAX_BRIGHTNESS;
//                f_temp = un_brightness - f_temp;
//                if(f_temp < 0)
//                    un_brightness = 0;
//                else
//                    un_brightness = (int)f_temp;
//            }
//            else
//            {
//                f_temp = un_prev_data - aun_red_buffer[i];
//                f_temp /= (un_max - un_min);
//                f_temp *= MAX_BRIGHTNESS;
//                un_brightness += (int)f_temp;
//                if(un_brightness > MAX_BRIGHTNESS)
//                    un_brightness = MAX_BRIGHTNESS;
//            }
//			//Send_To_PC2( aun_red_buffer[i], aun_ir_buffer[i] );
//            //send samples and calculation result to terminal program through UART
//            /*SerialUSB.print(F("red="));
//            SerialUSB.print(aun_red_buffer[i], DEC);
//            SerialUSB.print(F(", ir="));
//            SerialUSB.print(aun_ir_buffer[i], DEC);

//            SerialUSB.print(F(", HR="));
//            SerialUSB.print(n_heart_rate, DEC);

//            SerialUSB.print(F(", HRvalid="));
//            SerialUSB.print(ch_hr_valid, DEC);

//            SerialUSB.print(F(", SPO2="));
//            SerialUSB.print(n_spo2, DEC);

//            SerialUSB.print(F(", SPO2Valid="));
//            SerialUSB.println(ch_spo2_valid, DEC);*/

//            //      SerialUSB.println(aun_ir_buffer[i], DEC);
//        }
//        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

//        if ((ch_hr_valid == 1) && (n_heart_rate < 190) && (n_heart_rate > 40))
//        {
//            hrTimeout = 0;

//            // Throw out up to 1 out of every 5 valid samples if wacky
//            if (hrValidCnt == 4)
//            {
//                hrThrowOutSamp = 1;
//                hrValidCnt = 0;
//                for (i = 12; i < 16; i++)
//                {
//                    if (n_heart_rate < hr_buf[i] + 10)
//                    {
//                        hrThrowOutSamp = 0;
//                        hrValidCnt   = 4;
//                    }
//                }
//            }
//            else
//            {
//                hrValidCnt = hrValidCnt + 1;
//            }

//            if (hrThrowOutSamp == 0)
//            {

//                // Shift New Sample into buffer
//                for(i = 0; i < 15; i++)
//                {
//                    hr_buf[i] = hr_buf[i + 1];
//                }
//                hr_buf[15] = n_heart_rate;

//                // Update buffer fill value
//                if (hrBuffFilled < 16)
//                {
//                    hrBuffFilled = hrBuffFilled + 1;
//                }

//                // Take moving average
//                hrSum = 0;
//                if (hrBuffFilled < 2)
//                {
//                    hrAvg = 0;
//                }
//                else if (hrBuffFilled < 4)
//                {
//                    for(i = 14; i < 16; i++)
//                    {
//                        hrSum = hrSum + hr_buf[i];
//                    }
//                    hrAvg = hrSum >> 1;
//                }
//                else if (hrBuffFilled < 8)
//                {
//                    for(i = 12; i < 16; i++)
//                    {
//                        hrSum = hrSum + hr_buf[i];
//                    }
//                    hrAvg = hrSum >> 2;
//                }
//                else if (hrBuffFilled < 16)
//                {
//                    for(i = 8; i < 16; i++)
//                    {
//                        hrSum = hrSum + hr_buf[i];
//                    }
//                    hrAvg = hrSum >> 3;
//                }
//                else
//                {
//                    for(i = 0; i < 16; i++)
//                    {
//                        hrSum = hrSum + hr_buf[i];
//                    }
//                    hrAvg = hrSum >> 4;
//                }
//            }
//            hrThrowOutSamp = 0;
//        }
//        else
//        {
//            hrValidCnt = 0;
//            if (hrTimeout == 4)
//            {
//                hrAvg = 0;
//                hrBuffFilled = 0;
//            }
//            else
//            {
//                hrTimeout++;
//            }
//        }

//        if ((ch_spo2_valid == 1) && (n_spo2 > 59))
//        {
//            spo2Timeout = 0;

//            // Throw out up to 1 out of every 5 valid samples if wacky
//            if (spo2ValidCnt == 4)
//            {
//                spo2ThrowOutSamp = 1;
//                spo2ValidCnt = 0;
//                for (i = 12; i < 16; i++)
//                {
//                    if (n_spo2 > spo2_buf[i] - 10)
//                    {
//                        spo2ThrowOutSamp = 0;
//                        spo2ValidCnt   = 4;
//                    }
//                }
//            }
//            else
//            {
//                spo2ValidCnt = spo2ValidCnt + 1;
//            }

//            if (spo2ThrowOutSamp == 0)
//            {

//                // Shift New Sample into buffer
//                for(i = 0; i < 15; i++)
//                {
//                    spo2_buf[i] = spo2_buf[i + 1];
//                }
//                spo2_buf[15] = n_spo2;

//                // Update buffer fill value
//                if (spo2BuffFilled < 16)
//                {
//                    spo2BuffFilled = spo2BuffFilled + 1;
//                }

//                // Take moving average
//                spo2Sum = 0;
//                if (spo2BuffFilled < 2)
//                {
//                    spo2Avg = 0;
//                }
//                else if (spo2BuffFilled < 4)
//                {
//                    for(i = 14; i < 16; i++)
//                    {
//                        spo2Sum = spo2Sum + spo2_buf[i];
//                    }
//                    spo2Avg = spo2Sum >> 1;
//                }
//                else if (spo2BuffFilled < 8)
//                {
//                    for(i = 12; i < 16; i++)
//                    {
//                        spo2Sum = spo2Sum + spo2_buf[i];
//                    }
//                    spo2Avg = spo2Sum >> 2;
//                }
//                else if (spo2BuffFilled < 16)
//                {
//                    for(i = 8; i < 16; i++)
//                    {
//                        spo2Sum = spo2Sum + spo2_buf[i];
//                    }
//                    spo2Avg = spo2Sum >> 3;
//                }
//                else
//                {
//                    for(i = 0; i < 16; i++)
//                    {
//                        spo2Sum = spo2Sum + spo2_buf[i];
//                    }
//                    spo2Avg = spo2Sum >> 4;
//                }
//            }
//            spo2ThrowOutSamp = 0;
//        }
//        else
//        {
//            spo2ValidCnt = 0;
//            if (spo2Timeout == 4)
//            {
//                spo2Avg = 0;
//                spo2BuffFilled = 0;
//            }
//            else
//            {
//                spo2Timeout++;
//            }
//        }

//        //Send_To_PC(hrAvg, spo2Avg);
//		printf("%d\n",hrAvg);
////		OLED_ShowNum(8*12,6,hrAvg,4,16);
////		
////		if(hrAvg > 10)
////		{
////			if(hrAvg < Min_HeartRate)
////			{
////				BEEP = 0;		//报警
////				
////				LED2 = 1;
////				LED3 = 1;
////				LED4 = 0;
////				LED5 = 1;
////			}else if(hrAvg > MAX_HeartRate)
////			{
////				BEEP = 0;		//报警
////				
////				LED2 = 1;
////				LED3 = 1;
////				LED4 = 0;
////				LED5 = 0;
////			}else
////			{
////				BEEP = 1;		//不报警
////				
////				LED2 = 1;
////				LED3 = 0;
////				LED4 = 1;
////				LED5 = 1;
////			}
////		}else
////		{
////			BEEP = 1;		//不报警
////			
////			LED2 = 0;
////			LED3 = 1;
////			LED4 = 1;
////			LED5 = 1;
////		}
//}

