#ifndef  __MOTORCONTROL_H
#define  __MOTORCONTROL_H


#include "databyte.h"
#include "headfile.h"

void BrushMotorInit (BrushMotor_t* BrushMotor , int ERF , int EL , float TR);
void MotorInit (Motors_t** Motors);
void MotorsPWMSet(Motors_t* Motors);
void MotorStop(Motors_t* Motors);
void MotorDirectionSet(Motors_t* Motors);
void MotorPidCtrl (Motors_t* Motors,fp32 TurnAngleSet,fp32 AverageSpeedSet);
void MotorDataUpdate (Motors_t* Motors);

#endif