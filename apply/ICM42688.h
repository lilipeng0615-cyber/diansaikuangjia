#ifndef __ICM42688_H
#define __ICM42688_H

#include "databyte.h"
#include "headfile.h"

#define ICM42688_CHIP_ID_VALUE      0x47

#define ICM42688_ACCEL_16G_SEN      (9.80665f * 16.0f / 32768.0f)
#define ICM42688_GYRO_2000DPS_SEN   (2000.0f * 0.01745329252f / 32768.0f)

extern ICM42688_t ICM42688;

uint8_t ICM42688_Init(void);
uint8_t ICM42688_ReadID(void);
void ICM42688_Read(ICM42688_t *imu);
void ICM42688_CalibrateGyro(uint16_t samples);
void ICM42688_UpdateStaticGyroBias(float alpha);

#endif

