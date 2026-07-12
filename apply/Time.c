#include "Time.h"

volatile uint8 imuflag=0;
volatile uint8 controlflag=0;
volatile uint8 vofaflag=0;

void Time_Init(void)
{
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_0_INST);
	NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_1_INST);
	NVIC_EnableIRQ(TIMER_2_INST_INT_IRQN);
	DL_TimerA_startCounter(TIMER_2_INST);
	
}


void TIMER_0_INST_IRQHandler(void)
{
	 switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST))
	 {
		 case DL_TIMERG_IIDX_ZERO:
		 {
			 
			
			 controlflag=1;

			 
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
			imuflag=1;
		}
		break;
		 default:
			 break;
	}
}


void  TIMER_2_INST_IRQHandler(void)
{

	switch(DL_TimerA_getPendingInterrupt(TIMER_2_INST))
	{
		case  DL_TIMERA_IIDX_ZERO:
		{
			vofaflag=1;
		}
		break;
		 default:
			 break;
	}
}
