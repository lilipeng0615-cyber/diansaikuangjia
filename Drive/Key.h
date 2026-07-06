#ifndef  __KEY_H
#define  __KEY_H

#include "databyte.h"
#include "headfile.h"
#define KEY_COUNT				4


uint8_t read_pin(uint8_t n);
uint8_t Key_check(uint8_t n,uint8_t flag);
void Key_Tick(void);
void Key_Task_Handle(void);


#endif