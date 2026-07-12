#include "Time.h"

volatile uint8 tick;
volatile uint8 num;
volatile uint8 imuflag;
volatile uint8 controlflag;

void Time_Init(void)
{
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_0_INST);
	NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_1_INST);
	NVIC_EnableIRQ(TIMER_2_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_2_INST);
	
}


void TIMER_0_INST_IRQHandler(void)
{
	 switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST))
	 {
		 case DL_TIMERG_IIDX_ZERO:
		 {
			 //执行xx任务函数
			 Key_Tick();

			 imuflag=1;
			 tick++;
			 num++;
			 if(tick>=2)
			 {
				 tick=0;
				 controlflag=1;
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


//50HZ
void TIMER_1_INST_IRQHandler (void)
{
	switch(DL_TimerG_getPendingInterrupt(TIMER_1_INST))
	{
		case  DL_TIMERG_IIDX_ZERO:
		{

		}
		break;
		 default:
			 break;
	}
}


void  TIMER_2_INST_IRQHandler(void)
{

	switch(DL_TimerG_getPendingInterrupt(TIMER_2_INST))
	{
		case  DL_TIMERG_IIDX_ZERO:
		{

		}
		break;
		 default:
			 break;
	}
}
