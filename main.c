#include "headfile.h"

Car_t car;
//extern volatile uint8 imuflag;
//extern volatile uint8 controlflag;
//extern volatile uint8 vofaflag;
uint32_t lcd_last_ms = 0;
uint32_t vofa_last_ms=0;

#define GRAY_VOFA_PERIOD_MS (50U)

//void duty_10hz(void)
//{
//	static uint32_t last_tick = 0;
//	uint32_t now_tick = micros();
//	float dt = (float)(now_tick - last_tick) * 0.000001f;
//	last_tick = now_tick;
//	
//	IMU_Attitude_Update(dt);
//	
//}


//void duty_50hz(void)
//{
//	
//	EncoderDataUpdate(car.Motors);
//	if(car.task->TaskFlag==1)
//	{
//		Task1(car);
//	}
//	if(car.task->TaskFlag==2)
//	{
//		Task2(car);
//	}
//	if(car.task->TaskFlag==3)
//	{
//		Task3(car);
//	}
//	if(car.task->TaskFlag==4)
//	{
//		Task4(car);
//	}
//	
//}


int main()
{
	SYSCFG_DL_init();
	MotorInit(&car.Motors);
	Vofa_Init(car.Motors);
	GraySensorInit(&car.GraySensor_t);
	EncodersInit(car.Motors);
	BeepInit(&car.buzzer);
	ICM42688_Init();
	Task_Init(&car.task);
	IMU_Attitude_Init();

	  LCD_Init();
    LCD_Clear(LCD_BLACK);


//	Time_Init();
	
    while(1)
		{
			uint32_t now_ms = millis();
			
//			if(controlflag)
//			{
//				controlflag=0;
//				duty_50hz();
//			}
//			if(imuflag)
//			{
//				imuflag=0;
//				duty_10hz();
//			}
//			if(vofaflag)
//			{
//				vofaflag=0;
//				Vofa_SendJustFloat();
//			}
//			
			if ((uint32_t)(now_ms - vofa_last_ms) >= GRAY_VOFA_PERIOD_MS)
			{
				vofa_last_ms = now_ms;
				GraySensorDataUpdate(car.GraySensor_t);
				Vofa_SendGrayFireWater(car.GraySensor_t);
			}

			if ((uint32_t)(now_ms - lcd_last_ms) >= 100)
			{
				lcd_last_ms = now_ms;
				LCD_DebugUpdate(car);
			}
//			 Key_Task_Handle();
//			 Vofa_Task();
		}
}
