#include "Key.h"
#include "Beep.h"
#include "GraySensor.h"
#include "LCD_ST7735.h"
#include "MotorControl.h"
#include "app_config.h"
#include "databyte.h"
#include "ti_msp_dl_config.h"
#include <stddef.h>

extern Car_t car;

#define KEY_PRESSED				1
#define KEY_UNPRESSED			0

#define KEY_TICK_MS             1U
#define KEY_SCAN_TICKS          (20U / KEY_TICK_MS)
#define KEY_TIME_DOUBLE         (200U / KEY_TICK_MS)
#define KEY_TIME_LONG           (2000U / KEY_TICK_MS)
#define KEY_TIME_REPEAT         (100U / KEY_TICK_MS)

volatile uint8_t Key_Flag[KEY_COUNT];

uint8_t read_pin(uint8_t n)
{
	if(n==1)
	{
		if(DL_GPIO_readPins(Key_PORT,Key_PIN_9_PIN)==0)
		{
			return KEY_PRESSED;
		}
	}
	else if(n==2)
	{
		if(DL_GPIO_readPins(Key_PORT,Key_PIN_10_PIN)==0)
		{
			return KEY_PRESSED;
		}
	}
	else if(n==3)
	{
		if(DL_GPIO_readPins(Key_PORT,Key_PIN_11_PIN)==0)
		{
			return KEY_PRESSED;
		}
	}
	else if(n==4)
	{
		if(DL_GPIO_readPins(Key_PORT,Key_PIN_12_PIN)==0)
		{
			return KEY_PRESSED;
		}
	}
	
	return KEY_UNPRESSED;
}


uint8_t Key_check(uint8_t n,uint8_t flag)
{
	uint8_t hit = 0;
	uint32_t primask;

	if (n >= KEY_COUNT) {
		return 0;
	}

	primask = __get_PRIMASK();
	__disable_irq();
	if(Key_Flag[n] & flag)
	{
		if(flag != KEY_FLAG_HOLD)
		{
			Key_Flag[n] &= (uint8_t)(~flag);
		}
		hit = 1;
	}
	__set_PRIMASK(primask);
	return hit;
}




void Key_Tick(void)
{
	static uint8_t Count, i;
	static uint8_t CurrState[KEY_COUNT], PrevState[KEY_COUNT];
	static uint8_t S[KEY_COUNT];
	static uint16_t Time[KEY_COUNT];
	
	for(i=0;i<KEY_COUNT;i++)
	{
		if(Time[i]>0)
		{
			Time[i]--;
		}
	}
	Count++;
	if(Count>=KEY_SCAN_TICKS)
	{
		Count=0;
		for(i=0;i<KEY_COUNT;i++)
		{
			PrevState[i] = CurrState[i];
			CurrState[i] = read_pin(i+1);
			if(CurrState[i]==KEY_PRESSED)
			{
				Key_Flag[i]|=0x01;
			}
			else
			{
				Key_Flag[i]&=~0x01;
			}
			if(CurrState[i] == KEY_PRESSED && PrevState[i] == KEY_UNPRESSED)
			{
				Key_Flag[i]|=0x02;
			}
			if(CurrState[i] == KEY_UNPRESSED && PrevState[i] == KEY_PRESSED)
			{
				Key_Flag[i] |=0x04;
			}
			if(S[i]==0)
			{
				if(CurrState[i]==KEY_PRESSED)
				{
					Time[i]=KEY_TIME_LONG;
					S[i]=1;
				}
			}
			else if(S[i]==1)
			{
				if(CurrState[i] == KEY_UNPRESSED)
				{
					Time[i]= KEY_TIME_DOUBLE;
					S[i]=2;
				}
				else if(Time[i]==0)
				{
					Time[i]=KEY_TIME_REPEAT;
					Key_Flag[i]|=0x20;
					S[i]=4;
				}
			}
			else if(S[i]==2)
			{
				if(CurrState[i]==KEY_PRESSED)
				{
					Key_Flag[i] |= KEY_FLAG_DOUBLE;
					S[i]=3;
				}
				else if(Time[i]==0)
				{
					Key_Flag[i] |= KEY_FLAG_SINGLE;
					S[i]=0;
				}
				
			}
			else if(S[i]==3)
			{
				if(CurrState[i]==KEY_UNPRESSED)
				{
					S[i]=0;
				}
			}
			else if(S[i]==4)
			{
				if(CurrState[i]==KEY_UNPRESSED)
				{
					S[i]=0;
				}
				else if(Time[i]==0)
				{
					Time[i]=KEY_TIME_REPEAT;
					Key_Flag[i]|=0x40;
					S[i]=4;
				}
			}
		}
	}
}


void Key_Task_Handle(void)
{
    if (car.task == NULL) {
        return;
    }

    /* Any key press while running or counting down triggers immediate E-stop. */
    if (car.task->Carstartflag != 0) {
        if (Key_check(0, KEY_FLAG_DOWN) ||
            Key_check(1, KEY_FLAG_DOWN) ||
            Key_check(2, KEY_FLAG_DOWN) ||
            Key_check(3, KEY_FLAG_DOWN)) {
            car.task->Carstartflag = 0;
            car.task->StartDelayTicks = 0;
            car.task->TaskState = 0;
            MotorStop(car.Motors);
            BuzzerBee(car.buzzer);
            LCD_ShowStatusLine("E-STOP!", LCD_RED);
        }
        return;
    }

    /* Long-press Key 1 while idle to run 2-step grayscale calibration. */
    if (Key_check(0, KEY_FLAG_LONG)) {
        MotorStop(car.Motors);
        GraySensor_RunCalibration(car.GraySensor_t, car.buzzer);
        return;
    }

    if (Key_check(0, KEY_FLAG_SINGLE)) {
        car.task->TaskFlag = 1;
        car.task->TaskState = 0;
        car.task->StartDelayTicks = TASK_START_DELAY_TICKS;
        car.task->Carstartflag = 1;
        BuzzerBee(car.buzzer);
        LCD_ShowStatusLine("T1 ARMING...", LCD_YELLOW);
    } else if (Key_check(1, KEY_FLAG_SINGLE)) {
        car.task->TaskFlag = 2;
        car.task->TaskState = 0;
        car.task->StartDelayTicks = TASK_START_DELAY_TICKS;
        car.task->Carstartflag = 1;
        BuzzerBee(car.buzzer);
        LCD_ShowStatusLine("T2 ARMING...", LCD_YELLOW);
    } else if (Key_check(2, KEY_FLAG_SINGLE)) {
        car.task->TaskFlag = 3;
        car.task->TaskState = 0;
        car.task->StartDelayTicks = TASK_START_DELAY_TICKS;
        car.task->Carstartflag = 1;
        BuzzerBee(car.buzzer);
        LCD_ShowStatusLine("T3 ARMING...", LCD_YELLOW);
    } else if (Key_check(3, KEY_FLAG_SINGLE)) {
        car.task->TaskFlag = 4;
        car.task->TaskState = 0;
        car.task->StartDelayTicks = TASK_START_DELAY_TICKS;
        car.task->Carstartflag = 1;
        BuzzerBee(car.buzzer);
        LCD_ShowStatusLine("T4 ARMING...", LCD_YELLOW);
    }
}




