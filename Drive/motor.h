#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>
#include <ti/devices/msp/peripherals/hw_gptimer.h>

void PWMStart(GPTIMER_Regs *gptimer);
void PWMStop(GPTIMER_Regs *gptimer);
void PWM_OUTput(uint16_t witdh1,uint16_t witdh2);


#endif
