#include "LED.h"

void led_drive(uint8_t s, GPIO_Regs* gpio, uint32_t pins)
{
	if(s)
		DL_GPIO_setPins(gpio,pins);
	else 
		DL_GPIO_clearPins(gpio,pins);
		
}

void led(uint8_t s, uint16_t index)
{
	switch(index)
	{
		case 0:DL_GPIO_setPins(LED_PORT,LED_PIN_13_PIN);
		//case 1:DL_GPIO_setPins();
		//case 2:DL_GPIO_setPins();
		
	}
	
}
void led_up(void)
{
	led_drive(0,LED_PORT,LED_PIN_13_PIN);
	delay_ms(50);
	led_drive(1,LED_PORT,LED_PIN_13_PIN);
}


/*可以继续写led，格式几乎一样*/