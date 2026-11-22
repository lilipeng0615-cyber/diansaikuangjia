#ifndef __IMU_ATTITUDE_H
#define __IMU_ATTITUDE_H

#include "databyte.h"


extern IMU_Attitude_t IMU_Attitude;

void IMU_Attitude_Init(void);
void IMU_Attitude_SetStaticBiasEnable(uint8_t enable);
void IMU_Attitude_Update(float dt);

#endif

