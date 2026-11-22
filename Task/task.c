#include "task.h"
#include "Beep.h"
#include "Encoder.h"
#include "GraySensor.h"
#include "LED.h"
#include "MotorControl.h"
#include "app_config.h"
#include "imu_attitude.h"
#include "pid.h"
#include "ti_msp_dl_config.h"
#include <math.h>

/* Return ref + 2*k*PI closest to current_yaw (preserves multi-lap continuity). */
static float NearestSameYaw(float ref_yaw, float current_yaw)
{
    const float two_pi = 2.0f * PI;
    float diff = current_yaw - ref_yaw;
    int32_t turns = (int32_t)floorf((diff / two_pi) + 0.5f);
    return ref_yaw + (float)turns * two_pi;
}

/* Return ref + (2*k + 1)*PI closest to current_yaw (180 deg opposite axis). */
static float NearestOppositeYaw(float ref_yaw, float current_yaw)
{
    return NearestSameYaw(ref_yaw + PI, current_yaw);
}

void Task_Init(Task_t **Task)
{
    static Task_t task;
    *Task = &task;
    task.AverageSpeed = 0.0f;
    task.Carstartflag = 0;
    task.Datumyaw = 0.0f;
    task.StartYaw = 0.0f;
    task.Deltayaw = 0.0f;
    task.LastTaskStat = 0;
    task.LapCount = 0;
    task.StartDelayTicks = 0;
    task.StTrPreFlag = 0;
    task.TaskFlag = 0;
    task.TaskState = 0;
}

void Task1(Car_t Car)
{
    float yaw_error;
    float leg_dist;

    if ((Car.task == NULL) || (Car.Motors == NULL) || (Car.GraySensor_t == NULL)) {
        return;
    }

    if (Car.task->StTrPreFlag > 0) {
        Car.task->StTrPreFlag--;
        led_drive(TASK2_LED_ON, LED_PORT, LED_PIN_13_PIN);
    } else {
        led_drive(TASK2_LED_OFF, LED_PORT, LED_PIN_13_PIN);
    }

    if (Car.task->Carstartflag == 0) {
        MotorStop(Car.Motors);
        return;
    }

    GraySensorDataUpdate(Car.GraySensor_t);

    if (Car.task->TaskState == TASK1_STATE_IDLE) {
        PID_clear(Car.Motors->Left);
        PID_clear(Car.Motors->Right);
        PID_clear(Car.Motors->PidSelfTurn);
        PID_clear(Car.Motors->PidYaw);
        EncoderLengthClear(Car.Motors);

        Car.task->AverageSpeed = TASK1_SPEED;
        Car.task->StartYaw = IMU_Attitude.YawTotalAngle;
        Car.task->Datumyaw = Car.task->StartYaw;
        Car.task->Deltayaw = 0.0f;
        Car.task->TaskState = TASK1_STATE_LEAVE_A;
    }

    if (Car.task->TaskState == TASK1_STATE_LEAVE_A) {
        if (Car.GraySensor_t->BinaryData == 0xFF) {
            EncoderLengthClear(Car.Motors);
            Car.task->TaskState = TASK1_STATE_FIND_B;
        }
        yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
        Car.task->Deltayaw = yaw_error;
        MotorYawPidCtrl(Car.Motors, yaw_error, TASK1_SPEED);
        MotorDataUpdate(Car.Motors);
    } else if (Car.task->TaskState == TASK1_STATE_FIND_B) {
        leg_dist = ABS(EncoderTotalLengthGet(Car.Motors));
        if (((leg_dist >= TASK_BLIND_DIST_CM) && (Car.GraySensor_t->BinaryData != 0xFF)) ||
            (leg_dist >= TASK_MAX_LEG_DIST_CM)) {
            MotorStop(Car.Motors);
            Car.task->AverageSpeed = 0.0f;
            Car.task->Carstartflag = 0;
            Car.task->TaskState = TASK1_STATE_FINISH;
            BuzzerBee(Car.buzzer);
            Car.task->StTrPreFlag = TASK2_LED_TICKS;
            return;
        }
        yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
        Car.task->Deltayaw = yaw_error;
        MotorYawPidCtrl(Car.Motors, yaw_error, TASK1_SPEED);
        MotorDataUpdate(Car.Motors);
    } else if (Car.task->TaskState == TASK1_STATE_FINISH) {
        MotorStop(Car.Motors);
    }
}

void Task2(Car_t Car)
{
    float yaw_error;
    float turn_angle;
    float leg_dist;

    if ((Car.task == NULL) || (Car.Motors == NULL) || (Car.GraySensor_t == NULL)) {
        return;
    }

    if (Car.task->StTrPreFlag > 0) {
        Car.task->StTrPreFlag--;
        led_drive(TASK2_LED_ON, LED_PORT, LED_PIN_13_PIN);
    } else {
        led_drive(TASK2_LED_OFF, LED_PORT, LED_PIN_13_PIN);
    }

    if (Car.task->Carstartflag == 0) {
        MotorStop(Car.Motors);
        return;
    }

    GraySensorDataUpdate(Car.GraySensor_t);

    if (Car.task->TaskState == TASK2_STATE_IDLE) {
        PID_clear(Car.Motors->Left);
        PID_clear(Car.Motors->Right);
        PID_clear(Car.Motors->PidSelfTurn);
        PID_clear(Car.Motors->PidYaw);
        EncoderLengthClear(Car.Motors);
        Car.GraySensor_t->last_turn_angle = 0.0f;

        Car.task->AverageSpeed = TASK2_STRAIGHT_SPEED;
        Car.task->StartYaw = IMU_Attitude.YawTotalAngle;
        Car.task->Datumyaw = Car.task->StartYaw;
        Car.task->Deltayaw = 0.0f;
        Car.task->TaskState = TASK2_STATE_AB_LEAVE_A;
        BuzzerBee(Car.buzzer);
        Car.task->StTrPreFlag = TASK2_LED_TICKS;
    }

    if (Car.task->TaskState == TASK2_STATE_AB_LEAVE_A) {
        if (Car.GraySensor_t->BinaryData == 0xFF) {
            EncoderLengthClear(Car.Motors);
            Car.task->TaskState = TASK2_STATE_AB_FIND_B;
        }
        yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
        Car.task->Deltayaw = yaw_error;
        MotorYawPidCtrl(Car.Motors, yaw_error, TASK2_STRAIGHT_SPEED);
        MotorDataUpdate(Car.Motors);
    } else if (Car.task->TaskState == TASK2_STATE_AB_FIND_B) {
        leg_dist = ABS(EncoderTotalLengthGet(Car.Motors));
        if (leg_dist >= TASK_MAX_LEG_DIST_CM) {
            MotorStop(Car.Motors);
            Car.task->AverageSpeed = 0.0f;
            Car.task->Carstartflag = 0;
            Car.task->TaskState = TASK2_STATE_FINISH;
            BuzzerBee(Car.buzzer);
            return;
        }
        if ((leg_dist >= TASK_BLIND_DIST_CM) && (Car.GraySensor_t->BinaryData != 0xFF)) {
            BuzzerBee(Car.buzzer);
            Car.task->StTrPreFlag = TASK2_LED_TICKS;
            PID_clear(Car.Motors->PidSelfTurn);
            PID_clear(Car.Motors->PidYaw);
            Car.GraySensor_t->last_turn_angle = 0.0f;
            Car.task->AverageSpeed = TASK2_TRACE_SPEED;
            Car.task->LastTaskStat = 0;
            Car.task->TaskState = TASK2_STATE_BC_TRACE;
            return;
        }
        yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
        Car.task->Deltayaw = yaw_error;
        MotorYawPidCtrl(Car.Motors, yaw_error, TASK2_STRAIGHT_SPEED);
        MotorDataUpdate(Car.Motors);
    } else if (Car.task->TaskState == TASK2_STATE_BC_TRACE) {
        if (Car.GraySensor_t->BinaryData == 0xFF) {
            Car.task->LastTaskStat++;
            if (Car.task->LastTaskStat >= TASK2_LINE_LOST_TICKS) {
                BuzzerBee(Car.buzzer);
                Car.task->StTrPreFlag = TASK2_LED_TICKS;
                Car.task->AverageSpeed = TASK2_STRAIGHT_SPEED;
                /* Anchor C->D heading to StartYaw +/- PI instead of noisy arc-exit yaw. */
                Car.task->Datumyaw = NearestOppositeYaw(Car.task->StartYaw, IMU_Attitude.YawTotalAngle);
                Car.task->Deltayaw = 0.0f;
                Car.task->LastTaskStat = 0;
                EncoderLengthClear(Car.Motors);
                Car.task->TaskState = TASK2_STATE_CD_FIND_D;
                PID_clear(Car.Motors->PidSelfTurn);
                PID_clear(Car.Motors->PidYaw);
                return;
            }
            MotorPidCtrl(Car.Motors, Car.GraySensor_t->last_turn_angle, TASK2_TRACE_SPEED);
            MotorDataUpdate(Car.Motors);
        } else {
            Car.task->LastTaskStat = 0;
            turn_angle = GraySensorToTurnAngle(Car.GraySensor_t);
            MotorPidCtrl(Car.Motors, turn_angle, TASK2_TRACE_SPEED);
            MotorDataUpdate(Car.Motors);
        }
    } else if (Car.task->TaskState == TASK2_STATE_CD_FIND_D) {
        leg_dist = ABS(EncoderTotalLengthGet(Car.Motors));
        if (leg_dist >= TASK_MAX_LEG_DIST_CM) {
            MotorStop(Car.Motors);
            Car.task->AverageSpeed = 0.0f;
            Car.task->Carstartflag = 0;
            Car.task->TaskState = TASK2_STATE_FINISH;
            BuzzerBee(Car.buzzer);
            return;
        }
        if ((leg_dist >= TASK_BLIND_DIST_CM) && (Car.GraySensor_t->BinaryData != 0xFF)) {
            BuzzerBee(Car.buzzer);
            Car.task->StTrPreFlag = TASK2_LED_TICKS;
            PID_clear(Car.Motors->PidSelfTurn);
            PID_clear(Car.Motors->PidYaw);
            Car.GraySensor_t->last_turn_angle = 0.0f;
            Car.task->AverageSpeed = TASK2_TRACE_SPEED;
            Car.task->LastTaskStat = 0;
            Car.task->TaskState = TASK2_STATE_DA_TRACE;
            return;
        }
        yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
        Car.task->Deltayaw = yaw_error;
        MotorYawPidCtrl(Car.Motors, yaw_error, TASK2_STRAIGHT_SPEED);
        MotorDataUpdate(Car.Motors);
    } else if (Car.task->TaskState == TASK2_STATE_DA_TRACE) {
        if (Car.GraySensor_t->BinaryData == 0xFF) {
            Car.task->LastTaskStat++;
            if (Car.task->LastTaskStat >= TASK2_LINE_LOST_TICKS) {
                MotorStop(Car.Motors);
                Car.task->AverageSpeed = 0.0f;
                Car.task->Carstartflag = 0;
                Car.task->LastTaskStat = 0;
                Car.task->TaskState = TASK2_STATE_FINISH;
                BuzzerBee(Car.buzzer);
                Car.task->StTrPreFlag = TASK2_LED_TICKS;
                return;
            }
            MotorPidCtrl(Car.Motors, Car.GraySensor_t->last_turn_angle, TASK2_TRACE_SPEED);
            MotorDataUpdate(Car.Motors);
        } else {
            Car.task->LastTaskStat = 0;
            turn_angle = GraySensorToTurnAngle(Car.GraySensor_t);
            MotorPidCtrl(Car.Motors, turn_angle, TASK2_TRACE_SPEED);
            MotorDataUpdate(Car.Motors);
        }
    } else if (Car.task->TaskState == TASK2_STATE_FINISH) {
        MotorStop(Car.Motors);
    }
}

void Task34_Run(Car_t Car, int16 lap)
{
    float yaw_error;
    float turn_angle;
    float leg_dist;
    float ab_axis_yaw;

    if ((Car.task == NULL) || (Car.Motors == NULL) || (Car.GraySensor_t == NULL)) {
        return;
    }

    if (Car.task->StTrPreFlag > 0) {
        Car.task->StTrPreFlag--;
        led_drive(TASK34_LED_ON, LED_PORT, LED_PIN_13_PIN);
    } else {
        led_drive(TASK34_LED_OFF, LED_PORT, LED_PIN_13_PIN);
    }

    if (Car.task->Carstartflag == 0) {
        MotorStop(Car.Motors);
        return;
    }

    GraySensorDataUpdate(Car.GraySensor_t);

    if (Car.task->TaskState == TASK34_STATE_IDLE) {
        PID_clear(Car.Motors->Left);
        PID_clear(Car.Motors->Right);
        PID_clear(Car.Motors->PidSelfTurn);
        PID_clear(Car.Motors->PidYaw);
        EncoderLengthClear(Car.Motors);
        Car.GraySensor_t->last_turn_angle = 0.0f;

        Car.task->AverageSpeed = TASK34_STRAIGHT_SPEED;
        Car.task->StartYaw = IMU_Attitude.YawTotalAngle;
        Car.task->Datumyaw = Car.task->StartYaw + TASK34_START_AC_YAW_OFFSET;
        Car.task->LapCount = 0;
        Car.task->LastTaskStat = 0;
        Car.task->Deltayaw = 0.0f;
        BuzzerBee(Car.buzzer);
        Car.task->StTrPreFlag = TASK34_LED_TICKS;
        Car.task->TaskState = TASK34_STATE_AC_LEAVE_A;
    } else if (Car.task->TaskState == TASK34_STATE_AC_LEAVE_A) {
        if (Car.GraySensor_t->BinaryData == 0xFF) {
            EncoderLengthClear(Car.Motors);
            Car.task->TaskState = TASK34_STATE_AC_FIND_C;
        }
        yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
        Car.task->Deltayaw = yaw_error;
        MotorYawPidCtrl(Car.Motors, yaw_error, TASK34_STRAIGHT_SPEED);
        MotorDataUpdate(Car.Motors);
    } else if (Car.task->TaskState == TASK34_STATE_AC_FIND_C) {
        leg_dist = ABS(EncoderTotalLengthGet(Car.Motors));
        if (leg_dist >= TASK_MAX_LEG_DIST_CM) {
            MotorStop(Car.Motors);
            Car.task->AverageSpeed = 0.0f;
            Car.task->Carstartflag = 0;
            Car.task->TaskState = TASK34_STATE_FINISH;
            BuzzerBee(Car.buzzer);
            return;
        }
        if ((leg_dist >= TASK_BLIND_DIST_CM) && (Car.GraySensor_t->BinaryData != 0xFF)) {
            BuzzerBee(Car.buzzer);
            Car.task->AverageSpeed = TASK34_TRACE_SPEED;
            Car.task->StTrPreFlag = TASK34_LED_TICKS;
            Car.task->LastTaskStat = 0;
            Car.task->TaskState = TASK34_STATE_CB_TRACE;
            PID_clear(Car.Motors->PidSelfTurn);
            PID_clear(Car.Motors->PidYaw);
            Car.GraySensor_t->last_turn_angle = 0.0f;
            return;
        }
        yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
        Car.task->Deltayaw = yaw_error;
        MotorYawPidCtrl(Car.Motors, yaw_error, TASK34_STRAIGHT_SPEED);
        MotorDataUpdate(Car.Motors);
    } else if (Car.task->TaskState == TASK34_STATE_CB_TRACE) {
        if (Car.GraySensor_t->BinaryData == 0xFF) {
            Car.task->LastTaskStat++;
            if (Car.task->LastTaskStat >= TASK34_LINE_LOST_TICKS) {
                BuzzerBee(Car.buzzer);
                Car.task->StTrPreFlag = TASK34_LED_TICKS;
                Car.task->AverageSpeed = TASK34_STRAIGHT_SPEED;
                /* Anchor B->D heading to initial AB axis (derived from StartYaw). */
                ab_axis_yaw = Car.task->StartYaw - TASK34_NEXT_AC_YAW_OFFSET;
                Car.task->Datumyaw = NearestOppositeYaw(ab_axis_yaw, IMU_Attitude.YawTotalAngle)
                                     + TASK34_BD_YAW_OFFSET;
                Car.task->Deltayaw = 0.0f;
                Car.task->LastTaskStat = 0;
                EncoderLengthClear(Car.Motors);
                Car.task->TaskState = TASK34_STATE_BD_FIND_D;

                PID_clear(Car.Motors->PidSelfTurn);
                PID_clear(Car.Motors->PidYaw);
                return;
            }
            turn_angle = TASK34_CB_TRACE_SIGN * Car.GraySensor_t->last_turn_angle;
            MotorPidCtrl(Car.Motors, turn_angle, TASK34_TRACE_SPEED);
            MotorDataUpdate(Car.Motors);
        } else {
            Car.task->LastTaskStat = 0;
            turn_angle = TASK34_CB_TRACE_SIGN * GraySensorToTurnAngle(Car.GraySensor_t);
            MotorPidCtrl(Car.Motors, turn_angle, TASK34_TRACE_SPEED);
            MotorDataUpdate(Car.Motors);
        }
    } else if (Car.task->TaskState == TASK34_STATE_BD_FIND_D) {
        leg_dist = ABS(EncoderTotalLengthGet(Car.Motors));
        if (leg_dist >= TASK_MAX_LEG_DIST_CM) {
            MotorStop(Car.Motors);
            Car.task->AverageSpeed = 0.0f;
            Car.task->Carstartflag = 0;
            Car.task->TaskState = TASK34_STATE_FINISH;
            BuzzerBee(Car.buzzer);
            return;
        }
        if ((leg_dist >= TASK_BLIND_DIST_CM) && (Car.GraySensor_t->BinaryData != 0xFF)) {
            BuzzerBee(Car.buzzer);
            Car.task->AverageSpeed = TASK34_TRACE_SPEED;
            Car.task->TaskState = TASK34_STATE_DA_TRACE;
            Car.task->LastTaskStat = 0;
            Car.task->StTrPreFlag = TASK34_LED_TICKS;
            PID_clear(Car.Motors->PidSelfTurn);
            PID_clear(Car.Motors->PidYaw);
            Car.GraySensor_t->last_turn_angle = 0.0f;
            return;
        }
        yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
        Car.task->Deltayaw = yaw_error;
        MotorYawPidCtrl(Car.Motors, yaw_error, TASK34_STRAIGHT_SPEED);
        MotorDataUpdate(Car.Motors);
    } else if (Car.task->TaskState == TASK34_STATE_DA_TRACE) {
        if (Car.GraySensor_t->BinaryData == 0xFF) {
            Car.task->LastTaskStat++;
            if (Car.task->LastTaskStat >= TASK34_LINE_LOST_TICKS) {
                BuzzerBee(Car.buzzer);
                Car.task->StTrPreFlag = TASK34_LED_TICKS;
                Car.task->LapCount++;
                Car.task->LastTaskStat = 0;
                if (Car.task->LapCount >= lap) {
                    MotorStop(Car.Motors);
                    Car.task->AverageSpeed = 0.0f;
                    Car.task->Carstartflag = 0;
                    Car.task->TaskState = TASK34_STATE_FINISH;
                    return;
                }
                Car.task->AverageSpeed = TASK34_STRAIGHT_SPEED;
                /* Anchor next lap's A->C heading to initial AB axis (no arc-exit drift). */
                ab_axis_yaw = Car.task->StartYaw - TASK34_NEXT_AC_YAW_OFFSET;
                Car.task->Datumyaw = NearestSameYaw(ab_axis_yaw, IMU_Attitude.YawTotalAngle)
                                     + TASK34_NEXT_AC_YAW_OFFSET;
                Car.task->Deltayaw = 0.0f;
                EncoderLengthClear(Car.Motors);
                Car.task->TaskState = TASK34_STATE_AC_LEAVE_A;

                PID_clear(Car.Motors->PidSelfTurn);
                PID_clear(Car.Motors->PidYaw);
                return;
            }
            turn_angle = TASK34_DA_TRACE_SIGN * Car.GraySensor_t->last_turn_angle;
            MotorPidCtrl(Car.Motors, turn_angle, TASK34_TRACE_SPEED);
            MotorDataUpdate(Car.Motors);
        } else {
            Car.task->LastTaskStat = 0;
            turn_angle = TASK34_DA_TRACE_SIGN * GraySensorToTurnAngle(Car.GraySensor_t);
            MotorPidCtrl(Car.Motors, turn_angle, TASK34_TRACE_SPEED);
            MotorDataUpdate(Car.Motors);
        }
    } else if (Car.task->TaskState == TASK34_STATE_FINISH) {
        MotorStop(Car.Motors);
    }
}

void Task3(Car_t Car)
{
    Task34_Run(Car, 1);
}

void Task4(Car_t Car)
{
    Task34_Run(Car, 4);
}
