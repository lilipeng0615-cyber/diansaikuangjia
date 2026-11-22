#ifndef  __TIME_H
#define  __TIME_H

#include <stdint.h>

extern volatile uint8_t imuflag;
extern volatile uint8_t controlflag;
extern volatile uint8_t vofaflag;
void Time_Init(void);

#endif