#include "Time.h"

volatile uint8 tick;
volatile uint8 num;
volatile uint8 imuflag;
extern  void duty_50hz(void);

void Time_Init(void)
{
	 NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
	DL_TimerA_startCounter(TIMER_0_INST);
	
}


//10ms一次
void TIMER_0_INST_IRQHandler(void)
{
	 switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST))
	 {
		 case DL_TIMERG_IIDX_ZERO:
		 {
			 //执行xx任务函数
			 imuflag=1;
			 tick++;
			 num++;
			 if(tick>=2)
			 {
				 tick=0;
				 duty_50hz();
				 //执行yy任务函数
			 }
			 if(num>=3)
			 {
				 num=0;
				 //zz任务
			 }
		 }
		 break;
		 default:
			 break;
	 }
	
}