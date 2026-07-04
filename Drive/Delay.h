#ifndef __DELAY_H
#define __DELAY_H

#include "databyte.h"
#include "headfile.h"

void SysTick_Handler(void);
uint32_t micros(void);
uint32_t millis(void);
void delayMicroseconds(uint32_t us);
void delay(uint32_t ms);
void delay_ms(uint32_t x);
void delay_us(uint32_t x);
void Delay_Ms(uint32_t time);
void Delay_Us(uint32_t time);
void get_systime(systime *sys);
float get_systime_ms(void);
uint32_t get_systick_ms(void);

#endif

