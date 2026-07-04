#include "EncoderExti.h"





void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) 
    {
			case Encoder1_INT_IIDX:
				if(DL_GPIO_readPins(Encoder1_PORT,Encoder1_A1_PIN)>0)
				{
					
				}
				else
				{
					
				}
				break;
			case Encoder2_INT_IIDX:
				if(DL_GPIO_readPins(Encoder2_PORT,Encoder2_A2_PIN)>0)
				{
					
					
				}
				else
				{
					
				}
				break;
		}
}
