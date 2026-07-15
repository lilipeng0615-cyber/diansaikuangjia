#include "imu_attitude.h"



#define IMU_MAHONY_KP          0.20f
#define IMU_MAHONY_KI          0.0f
#define IMU_STATIC_BIAS_ALPHA  0.0008f
#define IMU_DEFAULT_DT         0.020f
#define IMU_MAX_VALID_DT       0.100f
#define IMU_GRAVITY            9.80665f
#define IMU_ACCEL_GATE         1.0f
#define IMU_STATIC_RATE_LIMIT  0.040f
#define IMU_INITIAL_ALIGN_SAMPLES 50U
#define IMU_SENSOR_SAMPLE_MS   20U
#define IMU_ACCEL_GATE_MIN_SQ  ((IMU_GRAVITY - IMU_ACCEL_GATE) * \
                                (IMU_GRAVITY - IMU_ACCEL_GATE))
#define IMU_ACCEL_GATE_MAX_SQ  ((IMU_GRAVITY + IMU_ACCEL_GATE) * \
                                (IMU_GRAVITY + IMU_ACCEL_GATE))
#define IMU_STATIC_RATE_MAX_SQ (IMU_STATIC_RATE_LIMIT * IMU_STATIC_RATE_LIMIT)

IMU_Attitude_t IMU_Attitude;
static Mahony_t mahony;
static float yaw_last;

static float vector_norm_sq(Axis3f value)
{
    return value.x * value.x + value.y * value.y + value.z * value.z;
}

static uint8_t accel_correction_is_trustworthy(Axis3f gyro, Axis3f acc)
{
    float accel_norm_sq = vector_norm_sq(acc);

    if ((ICM42688.LastAccelSaturated != 0U) ||
        (accel_norm_sq < IMU_ACCEL_GATE_MIN_SQ) ||
        (accel_norm_sq > IMU_ACCEL_GATE_MAX_SQ) ||
        (vector_norm_sq(gyro) > IMU_STATIC_RATE_MAX_SQ)) {
        return 0U;
    }

    return 1U;
}

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
    Axis3f acc = {0.0f, 0.0f, 0.0f};

    memset(&IMU_Attitude, 0, sizeof(IMU_Attitude));
    Mahony_Init(&mahony, IMU_MAHONY_KP, IMU_MAHONY_KI, IMU_DEFAULT_DT);

    /* Average one second of 50 Hz samples for a stable initial tilt. */
    for (uint16_t sample = 0U; sample < IMU_INITIAL_ALIGN_SAMPLES; sample++) {
        ICM42688_Read(&ICM42688);
        acc.x += ICM42688.Accel[0];
        acc.y += ICM42688.Accel[1];
        acc.z += ICM42688.Accel[2];
        delay_ms(IMU_SENSOR_SAMPLE_MS);
    }
    acc.x /= (float)IMU_INITIAL_ALIGN_SAMPLES;
    acc.y /= (float)IMU_INITIAL_ALIGN_SAMPLES;
    acc.z /= (float)IMU_INITIAL_ALIGN_SAMPLES;
    Mahony_SetFromAccel(&mahony, acc);

    IMU_Attitude.q[0] = mahony.q0;
    IMU_Attitude.q[1] = mahony.q1;
    IMU_Attitude.q[2] = mahony.q2;
    IMU_Attitude.q[3] = mahony.q3;
    IMU_Attitude.Roll = mahony.roll;
    IMU_Attitude.Pitch = mahony.pitch;
    IMU_Attitude.Yaw = wrap_pi(mahony.yaw);
    IMU_Attitude.YawTotalAngle = 0.0f;
    yaw_last = IMU_Attitude.Yaw;
}

void IMU_Attitude_Update(float dt)
{
    Axis3f gyro;
    Axis3f acc;
    float yaw_delta;

    if (dt <= 0.0f || dt > IMU_MAX_VALID_DT) {
        dt = IMU_DEFAULT_DT;
    }

    ICM42688_Read(&ICM42688);
    ICM42688_UpdateStaticGyroBias(IMU_STATIC_BIAS_ALPHA);

    if (ICM42688.LastGyroSaturated != 0U) {
        return;
    }

    gyro.x = ICM42688.Gyro[0];
    gyro.y = ICM42688.Gyro[1];
    gyro.z = ICM42688.Gyro[2];
    acc.x = ICM42688.Accel[0];
    acc.y = ICM42688.Accel[1];
    acc.z = ICM42688.Accel[2];

    mahony.dt = dt;
    if (accel_correction_is_trustworthy(gyro, acc) != 0U) {
        Mahony_Update(&mahony, gyro, acc);
    } else {
        /* Dynamic acceleration is not gravity; integrate the gyro only. */
        Mahony_UpdateGyroOnly(&mahony, gyro);
    }

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

    /* Accumulate the shortest angular change so crossing +/-pi is continuous. */
    IMU_Attitude.YawTotalAngle += wrap_pi(yaw_delta);
    yaw_last = IMU_Attitude.Yaw;
}
