#ifndef __MAHONY_FILTER_H
#define __MAHONY_FILTER_H

#include "databyte.h"
#include "headfile.h"


#define RAD2DEG 57.295779513f
#define DEG2RAD 0.01745329252f

void Mahony_Init(Mahony_t *mahony, fp32 kp, fp32 ki, fp32 dt);
void Mahony_SetFromAccel(Mahony_t *mahony, Axis3f acc);
void Mahony_Update(Mahony_t *mahony, Axis3f gyro, Axis3f acc);
void Mahony_Output(Mahony_t *mahony);

#endif

