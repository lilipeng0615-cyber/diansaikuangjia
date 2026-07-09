#ifndef  __BEEP_H
#define  __BEEP_H


#include "databyte.h"
#include "headfile.h"

void BeepInit(Buzzer_t **Buzzer);
void BuzzerDataUpdate(Buzzer_t* Buzzer);
void BuzzerBee(Buzzer_t* Buzzer);



#endif
