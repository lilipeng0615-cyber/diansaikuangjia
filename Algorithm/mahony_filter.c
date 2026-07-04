#include "mahony_filter.h"

static float inv_sqrt(float x)
{
    return (x > 0.0f) ? (1.0f / sqrtf(x)) : 0.0f;
}

static void Mahony_RotationMatrixUpdate(Mahony_t *mahony)
{
    float q1q1 = mahony->q1 * mahony->q1;
    float q2q2 = mahony->q2 * mahony->q2;
    float q3q3 = mahony->q3 * mahony->q3;
    float q0q1 = mahony->q0 * mahony->q1;
    float q0q2 = mahony->q0 * mahony->q2;
    float q0q3 = mahony->q0 * mahony->q3;
    float q1q2 = mahony->q1 * mahony->q2;
    float q1q3 = mahony->q1 * mahony->q3;
    float q2q3 = mahony->q2 * mahony->q3;

    mahony->rMat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
    mahony->rMat[0][1] = 2.0f * (q1q2 - q0q3);
    mahony->rMat[0][2] = 2.0f * (q1q3 + q0q2);
    mahony->rMat[1][0] = 2.0f * (q1q2 + q0q3);
    mahony->rMat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
    mahony->rMat[1][2] = 2.0f * (q2q3 - q0q1);
    mahony->rMat[2][0] = 2.0f * (q1q3 - q0q2);
    mahony->rMat[2][1] = 2.0f * (q2q3 + q0q1);
    mahony->rMat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;
}

void Mahony_Output(Mahony_t *mahony)
{
    mahony->pitch = -asinf(mahony->rMat[2][0]);
    mahony->roll = atan2f(mahony->rMat[2][1], mahony->rMat[2][2]);
    mahony->yaw = atan2f(mahony->rMat[1][0], mahony->rMat[0][0]);
}

void Mahony_Init(Mahony_t *mahony, float kp, float ki, float dt)
{
    memset(mahony, 0, sizeof(Mahony_t));
    mahony->Kp = kp;
    mahony->Ki = ki;
    mahony->dt = dt;
    mahony->q0 = 1.0f;
    Mahony_RotationMatrixUpdate(mahony);
    Mahony_Output(mahony);
}

void Mahony_SetFromAccel(Mahony_t *mahony, Axis3f acc)
{
    float inv_norm = inv_sqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
    float roll;
    float pitch;
    float cr;
    float sr;
    float cp;
    float sp;

    if (inv_norm <= 0.0f) {
        return;
    }

    acc.x *= inv_norm;
    acc.y *= inv_norm;
    acc.z *= inv_norm;

    roll = atan2f(acc.y, acc.z);
    pitch = atan2f(-acc.x, sqrtf(acc.y * acc.y + acc.z * acc.z));

    cr = cosf(roll * 0.5f);
    sr = sinf(roll * 0.5f);
    cp = cosf(pitch * 0.5f);
    sp = sinf(pitch * 0.5f);

    mahony->q0 = cr * cp;
    mahony->q1 = sr * cp;
    mahony->q2 = cr * sp;
    mahony->q3 = -sr * sp;
    mahony->exInt = 0.0f;
    mahony->eyInt = 0.0f;
    mahony->ezInt = 0.0f;

    Mahony_RotationMatrixUpdate(mahony);
    Mahony_Output(mahony);
}

void Mahony_Update(Mahony_t *mahony, Axis3f gyro, Axis3f acc)
{
    float inv_norm;
    float ex;
    float ey;
    float ez;
    float q0_last;
    float q1_last;
    float q2_last;
    float q3_last;
    float half_t;

    inv_norm = inv_sqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
    if (inv_norm <= 0.0f) {
        return;
    }

    acc.x *= inv_norm;
    acc.y *= inv_norm;
    acc.z *= inv_norm;

    ex = acc.y * mahony->rMat[2][2] - acc.z * mahony->rMat[2][1];
    ey = acc.z * mahony->rMat[2][0] - acc.x * mahony->rMat[2][2];
    ez = acc.x * mahony->rMat[2][1] - acc.y * mahony->rMat[2][0];

    mahony->exInt += mahony->Ki * ex * mahony->dt;
    mahony->eyInt += mahony->Ki * ey * mahony->dt;
    mahony->ezInt += mahony->Ki * ez * mahony->dt;

    gyro.x += mahony->Kp * ex + mahony->exInt;
    gyro.y += mahony->Kp * ey + mahony->eyInt;
    gyro.z += mahony->Kp * ez + mahony->ezInt;

    q0_last = mahony->q0;
    q1_last = mahony->q1;
    q2_last = mahony->q2;
    q3_last = mahony->q3;
    half_t = mahony->dt * 0.5f;

    mahony->q0 += (-q1_last * gyro.x - q2_last * gyro.y - q3_last * gyro.z) * half_t;
    mahony->q1 += ( q0_last * gyro.x + q2_last * gyro.z - q3_last * gyro.y) * half_t;
    mahony->q2 += ( q0_last * gyro.y - q1_last * gyro.z + q3_last * gyro.x) * half_t;
    mahony->q3 += ( q0_last * gyro.z + q1_last * gyro.y - q2_last * gyro.x) * half_t;

    inv_norm = inv_sqrt(mahony->q0 * mahony->q0 + mahony->q1 * mahony->q1 +
                        mahony->q2 * mahony->q2 + mahony->q3 * mahony->q3);
    if (inv_norm <= 0.0f) {
        Mahony_Init(mahony, mahony->Kp, mahony->Ki, mahony->dt);
        return;
    }

    mahony->q0 *= inv_norm;
    mahony->q1 *= inv_norm;
    mahony->q2 *= inv_norm;
    mahony->q3 *= inv_norm;

    Mahony_RotationMatrixUpdate(mahony);
    Mahony_Output(mahony);
}
