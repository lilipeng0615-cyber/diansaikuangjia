#include "Beep.h"


volatile int beep_time;

void Beep(uint8_t s)
{
	if(s)
	DL_GPIO_setPins(Beep_PORT,Beep_PIN_14_PIN);
	else
	DL_GPIO_clearPins(Beep_PORT,Beep_PIN_14_PIN);
}


void Beep_up(void)
{
	led(0,0);
	beep_time=10;
}

void Beep_feed(void)
{
	if(beep_time>0)
	{
		beep_time--;
		 Beep(1);
	}
	else
	{
    led(1,0);		
		Beep(0);
	}
	
	
}