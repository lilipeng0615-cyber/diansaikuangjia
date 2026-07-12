#include "headfile.h"

Car_t car;
extern volatile uint8 imuflag;
extern volatile uint8 controlflag;
extern volatile uint8 vofaflag;
uint32_t lcd_last_ms = 0;
uint32_t vofa_last_ms=0;

void duty_10hz(void)
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
	Vofa_Init(car.Motors);
	GraySensorInit(&car.GraySensor_t);
	EncodersInit(car.Motors);
	BeepInit(&car.buzzer);
	ICM42688_Init();
	Task_Init(&car.task);
	IMU_Attitude_Init();
	LCD_Init();
	LCD_Clear(LCD_BLACK);
	Time_Init();
	
    while(1)
		{
			
			if(controlflag)
			{
				controlflag=0;
				duty_50hz();
			}
			if(imuflag)
			{
				imuflag=0;
				duty_10hz();
			}
			if(vofaflag)
			{
				vofaflag=0;
				Vofa_SendJustFloat();
			}
			
			if ((millis()-lcd_last_ms)>=100)
			{
				lcd_last_ms = millis();
				LCD_DebugUpdate(car);
			}
			 Key_Task_Handle();
			 Vofa_Task();
		}
}


