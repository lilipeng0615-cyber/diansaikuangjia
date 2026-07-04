#ifndef __IMU_ATTITUDE_H
#define __IMU_ATTITUDE_H

#include "databyte.h"
#include "mahony_filter.h"

extern IMU_Attitude_t IMU_Attitude;

void IMU_Attitude_Init(void);
void IMU_Attitude_Update(float dt);

#endif

