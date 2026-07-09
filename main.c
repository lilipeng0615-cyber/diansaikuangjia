#include "headfile.h"

Car_t car;
extern volatile uint8 imuflag;

void duty_100hz(void)
{
	static uint32_t last_tick = 0;
	uint32_t now_tick = micros();
	float dt = (float)(now_tick - last_tick) * 0.000001f;
	last_tick = now_tick;
	
	IMU_Attitude_Update(dt);
	
}


void duty_50hz(void)
{
	
	EncoderDataUpdate(car.Motors);
	if(car.task->TaskFlag==1)
	{
		Task1(car);
	}
	if(car.task->TaskFlag==2)
	{
		Task2(car);
	}
	if(car.task->TaskFlag==3)
	{
		Task3(car);
	}
	if(car.task->TaskFlag==4)
	{
		Task4(car);
	}
	
}


int main()
{
	SYSCFG_DL_init();
	MotorInit(&car.Motors);
	GraySensorInit(&car.GraySensor_t);
	EncodersInit(car.Motors);
	BeepInit(&car.buzzer);
	ICM42688_Init();
	Task_Init(&car.task);
	IMU_Attitude_Init();
	LCD_Init();
	Time_Init();
	
    while(1)
		{
			if(imuflag)
			{
				imuflag=0;
				duty_100hz();
			}
			Key_Task_Handle();
		}
}




