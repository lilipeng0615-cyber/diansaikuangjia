#include "motor.h"

void PWMStart(GPTIMER_Regs *gptimer)
{
	DL_TimerA_startCounter(gptimer);
}      

void PWMStop(GPTIMER_Regs *gptimer)
{
	DL_TimerA_stopCounter(gptimer);
}  


void PWM_OUTput(uint16_t witdh1,uint16_t witdh2)
{
	uint16_t pwm[4]={0};
	
	pwm[0]=witdh1;
	pwm[1]=witdh2;
	
	
	DL_TimerG_setCaptureCompareValue(PWM_0_INST,pwm[0],GPIO_PWM_0_C0_IDX);
	DL_TimerG_setCaptureCompareValue(PWM_0_INST,pwm[1],GPIO_PWM_0_C1_IDX);
	
}