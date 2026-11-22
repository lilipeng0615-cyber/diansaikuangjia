#ifndef  __LED_H
#define  __LED_H

#include <stdint.h>
#include <ti/devices/msp/peripherals/hw_gpio.h>
void led_drive(uint8_t s, GPIO_Regs* gpio, uint32_t pins);
void led(uint8_t s, uint16_t index);
void led_up(void);

#endif
