#include "Key.h"


#define KEY_PRESSED				1
#define KEY_UNPRESSED			0

#define KEY_TIME_DOUBLE			200
#define KEY_TIME_LONG			2000
#define KEY_TIME_REPEAT			100

uint8_t Key_Flag[KEY_COUNT];

uint8_t read_pin(uint8_t n)
{
	if(n==1)
	{
		if(DL_GPIO_readPins(Key_PORT,Key_PIN_3_PIN)==0)
		{
			return KEY_PRESSED;
		}
	}
	else if(n==2)
	{
		if(DL_GPIO_readPins(Key_PORT,Key_PIN_4_PIN)==0)
		{
			return KEY_PRESSED;
		}
	}
	else if(n==3)
	{
		if(DL_GPIO_readPins(Key_PORT,Key_PIN_5_PIN)==0)
		{
			return KEY_PRESSED;
		}
	}
	else if(n==4)
	{
		if(DL_GPIO_readPins(Key_PORT,Key_PIN_3_PIN)==0)
		{
			return KEY_PRESSED;
		}
	}
	
	return KEY_UNPRESSED;
}


uint8_t Key_check(uint8_t n,uint8_t flag)
{
	if(Key_Flag[n]&flag)
	{
		if(flag!=0x01)
		{
			Key_Flag[n]&=~flag;
		}
		return 1;
	}
	return 0;
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
	if(Count>=20)
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
           Key_Flag[i]|=0x10;
           S[i]=3;					
				}
				else if(Time[i]==0)
				{
					Key_Flag[i]|=0x10;
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

    if(Key_check(0, 0x02))
    {
//			if(task==stop||task==finsh)
//        {
//					IMU_Update();
//					seg=0;
//           task=task1;
				}
			
			/*24*/
//			task=task1;
//			target_yaw=ahrs.yaw;
//			seg1=0;
//      beepflag=0;
//			clear_pluse();
    


    if(Key_check(1, 0x02))
    {
						/*24*/
//			task=task2;
//			IMU_Update();
//			beepflag=0;
//			seg2=0;
//			target_yaw=ahrs.yaw;
//			clear_pluse();
    }

    if(Key_check(2, 0x02))
    {
						/*24*/
//			task=task3;
//			seg3=0;
//			target_yaw=Angleclan(ahrs.yaw -37.9);
//			beepflag=0;
//			clear_pluse();
    }


    if(Key_check(3, 0x02))
    {
//			task=task3;
//			clear_pluse();
//			read_pins();
//			lap_set++;

//			if(lap_set>5)
//			{
//					lap_set=1;
//			}
    }
}




