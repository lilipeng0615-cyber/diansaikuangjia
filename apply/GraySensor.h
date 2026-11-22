#ifndef  __GRAYSENSOR_H
#define  __GRAYSENSOR_H

#include "databyte.h"


void GraySensorInit(GraySensor_t** GraySensor);
void GraySensor_RecalcNorm(GraySensor_t* GraySensor);
void GraySensor_CalibrateSample(GraySensor_t* GraySensor, uint8_t is_white);
void GraySensor_RunCalibration(GraySensor_t* GraySensor, Buzzer_t* buzzer);
void GraySensorDataUpdate (GraySensor_t* GraySensor);
float GraySensorToTurnAngle(GraySensor_t* GraySensor);


#endif