#include "imu_attitude.h"
#include "ICM42688.h"
#include <string.h>


#define IMU_MAHONY_KP          1.0f
#define IMU_MAHONY_KI          0.001f
#define IMU_STATIC_BIAS_ALPHA  0.0008f

IMU_Attitude_t IMU_Attitude;
static Mahony_t mahony;
static float yaw_last;

static float wrap_pi(float angle)
{
    while (angle > 3.1415926f) {
        angle -= 6.2831852f;
    }
    while (angle < -3.1415926f) {
        angle += 6.2831852f;
    }
    return angle;
}

void IMU_Attitude_Init(void)
{
    Axis3f acc;

    memset(&IMU_Attitude, 0, sizeof(IMU_Attitude));
    Mahony_Init(&mahony, IMU_MAHONY_KP, IMU_MAHONY_KI, 0.001f);

    ICM42688_Read(&ICM42688);
    acc.x = ICM42688.Accel[0];
    acc.y = ICM42688.Accel[1];
    acc.z = ICM42688.Accel[2];
    Mahony_SetFromAccel(&mahony, acc);

    IMU_Attitude.q[0] = mahony.q0;
    IMU_Attitude.q[1] = mahony.q1;
    IMU_Attitude.q[2] = mahony.q2;
    IMU_Attitude.q[3] = mahony.q3;
    yaw_last = mahony.yaw;
}

void IMU_Attitude_Update(float dt)
{
    Axis3f gyro;
    Axis3f acc;
    float yaw_delta;

    if (dt <= 0.0f || dt > 0.02f) {
        dt = 0.001f;
    }

    ICM42688_Read(&ICM42688);
    ICM42688_UpdateStaticGyroBias(IMU_STATIC_BIAS_ALPHA);

    gyro.x = ICM42688.Gyro[0];
    gyro.y = ICM42688.Gyro[1];
    gyro.z = ICM42688.Gyro[2];
    acc.x = ICM42688.Accel[0];
    acc.y = ICM42688.Accel[1];
    acc.z = ICM42688.Accel[2];

    mahony.dt = dt;
    Mahony_Update(&mahony, gyro, acc);

    IMU_Attitude.q[0] = mahony.q0;
    IMU_Attitude.q[1] = mahony.q1;
    IMU_Attitude.q[2] = mahony.q2;
    IMU_Attitude.q[3] = mahony.q3;
    IMU_Attitude.Roll = mahony.roll;
    IMU_Attitude.Pitch = mahony.pitch;
    IMU_Attitude.Yaw = wrap_pi(mahony.yaw);

    yaw_delta = IMU_Attitude.Yaw - yaw_last;
    if (yaw_delta > 3.1415926f) {
        IMU_Attitude.YawRoundCount--;
    } else if (yaw_delta < -3.1415926f) {
        IMU_Attitude.YawRoundCount++;
    }
    IMU_Attitude.YawTotalAngle = 6.2831852f * (float)IMU_Attitude.YawRoundCount + IMU_Attitude.Yaw;
    yaw_last = IMU_Attitude.Yaw;
}
