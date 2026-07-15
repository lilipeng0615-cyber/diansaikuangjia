#include "headfile.h"

Car_t car;
//extern volatile uint8 imuflag;
//extern volatile uint8 controlflag;
//extern volatile uint8 vofaflag;
uint32_t lcd_last_ms = 0;
uint32_t gray_last_ms = 0;
uint32_t imu_update_last_us = 0;
uint32_t euler_vofa_last_us = 0;
uint8_t imu_status = ICM42688_NO_SENSOR;

#define GRAY_UPDATE_PERIOD_MS (50U)
#define IMU_UPDATE_PERIOD_US  (20000U)
#define EULER_VOFA_PERIOD_US  (30000U)

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
	DL_GPIO_setPins(CS_PORT, CS_PIN_6_PIN);
	MotorInit(&car.Motors);
	Vofa_Init(car.Motors);
	GraySensorInit(&car.GraySensor_t);
	EncodersInit(car.Motors);
	BeepInit(&car.buzzer);
	imu_status = ICM42688_Init();
	Task_Init(&car.task);
	if (imu_status == ICM42688_NO_ERROR)
	{
		IMU_Attitude_Init();
	}

	  LCD_Init();
    LCD_Clear(LCD_BLACK);
	imu_update_last_us = micros();
	euler_vofa_last_us = imu_update_last_us;
	gray_last_ms = imu_update_last_us / 1000;
	lcd_last_ms = gray_last_ms;


//	Time_Init();
	
    while(1)
		{
			uint32_t now_us = micros();
			uint32_t now_ms = now_us / 1000;
			
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
			if ((imu_status == ICM42688_NO_ERROR) &&
				((uint32_t)(now_us - imu_update_last_us) >= IMU_UPDATE_PERIOD_US))
			{
				float dt = (float)(now_us - imu_update_last_us) * 0.000001f;

				imu_update_last_us = now_us;
				IMU_Attitude_Update(dt);
			}

			if ((imu_status == ICM42688_NO_ERROR) &&
				((uint32_t)(now_us - euler_vofa_last_us) >= EULER_VOFA_PERIOD_US))
			{
				euler_vofa_last_us = now_us;
				Vofa_SendEulerFireWater(&IMU_Attitude);
			}

			if ((uint32_t)(now_ms - gray_last_ms) >= GRAY_UPDATE_PERIOD_MS)
			{
				gray_last_ms = now_ms;
				GraySensorDataUpdate(car.GraySensor_t);
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
