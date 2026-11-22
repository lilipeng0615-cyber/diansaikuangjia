#include "Beep.h"
#include "motor.h"
#include "ti_msp_dl_config.h"
#include <stddef.h>

void BeepInit(Buzzer_t **Buzzer)
{
	static Buzzer_t buzzer;
	*Buzzer=&buzzer;
	buzzer.BuzzerCount=0;
	buzzer.BuzzerFlag=0;
}

void BuzzerDataUpdate(Buzzer_t* Buzzer)
{
	if (Buzzer == NULL) {
		return;
	}
	if(Buzzer->BuzzerFlag){
		PWMStart(PWM_1_INST);
		if (Buzzer->BuzzerCount > 0) {
			Buzzer->BuzzerCount--;
		}
		if(Buzzer->BuzzerCount == 0)
			Buzzer->BuzzerFlag = 0;
	}
	else
		PWMStop(PWM_1_INST);
}

void BuzzerBee(Buzzer_t* Buzzer)
{
	if (Buzzer == NULL) {
		return;
	}
	Buzzer->BuzzerCount = 2;
	Buzzer->BuzzerFlag = 1;
}
