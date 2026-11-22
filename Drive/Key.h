#ifndef  __KEY_H
#define  __KEY_H

#include <stdint.h>

#define KEY_COUNT       4

#define KEY_FLAG_HOLD   0x01U
#define KEY_FLAG_DOWN   0x02U
#define KEY_FLAG_UP     0x04U
#define KEY_FLAG_DOUBLE 0x08U
#define KEY_FLAG_SINGLE 0x10U
#define KEY_FLAG_LONG   0x20U
#define KEY_FLAG_REPEAT 0x40U


uint8_t read_pin(uint8_t n);
uint8_t Key_check(uint8_t n,uint8_t flag);
void Key_Tick(void);
void Key_Task_Handle(void);


#endif