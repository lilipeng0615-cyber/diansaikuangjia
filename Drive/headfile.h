#ifndef  __HEADFILE
#define  __HEADFILE

#include "ti_msp_dl_config.h"
#include "databyte.h"
#include "LED.h"
#include "Beep.h"
#include "motor.h"
#include "uart.h"
#include "Delay.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "pid.h"
#include "spi.h"
#include "ICM42688.h"
#include "imu_attitude.h"
#include "EncoderExti.h"
#include "MotorControl.h"
#include "Encoder.h"
#include "GraySensor.h"
#include "kalman_filter.h"
#include "QuaternionEKF.h"
#include "Key.h"
#include "Time.h"
#include "LCD_ST7735.h"
#include "task.h"
#include "vofa.h"



#ifndef PI
#define PI 	   3.14159265358979323846l
#endif

#define BrushMotor_PID_mode		  PID_POSITION
#define BrushMotor_PID_KP   		10.0f
#define BrushMotor_PID_KI   		0.8f
#define BrushMotor_PID_KD   		5.0f
#define BrushMotor_PID_Maxout   1000.0f
#define BrushMotor_PID_MaxIout  1000.0f


#define SelfTurn_PID_mode 			PID_POSITION
#define SelfTurn_PID_KP   			0.8f
#define SelfTurn_PID_KI   			0.0f
#define SelfTurn_PID_KD   			0.15f
#define SelfTurn_PID_Maxout   	200.0f
#define SelfTurn_PID_MaxIout 	  10.0f

#define EncoderReadingFrequency 50.0f
#define EncoderLines 390.0f
#define TireRadius 6.5f

//第一个任务状态
#define TASK1_FLAG             1

#define TASK1_STATE_IDLE       0
#define TASK1_STATE_LEAVE_A    1
#define TASK1_STATE_FIND_B     2
#define TASK1_STATE_FINISH     3

#define TASK1_SPEED            45.0f

//第二个任务状态
#define TASK2_FLAG                 2

#define TASK2_STATE_IDLE           0
#define TASK2_STATE_AB_LEAVE_A     1
#define TASK2_STATE_AB_FIND_B      2
#define TASK2_STATE_BC_TRACE       3
#define TASK2_STATE_CD_FIND_D      4
#define TASK2_STATE_DA_TRACE       5
#define TASK2_STATE_FINISH         6

#define TASK2_STRAIGHT_SPEED       45.0f
#define TASK2_TRACE_SPEED          35.0f

#define TASK2_LINE_LOST_TICKS      5
#define TASK2_LED_TICKS            10

#define TASK2_LED_ON               0
#define TASK2_LED_OFF              1


//第三问与第四位
#define TASK3_FLAG                     3
#define TASK4_FLAG                     4

#define TASK34_STATE_IDLE              0
#define TASK34_STATE_AC_LEAVE_A        1
#define TASK34_STATE_AC_FIND_C         2
#define TASK34_STATE_CB_TRACE          3
#define TASK34_STATE_BD_FIND_D         4
#define TASK34_STATE_DA_TRACE          5
#define TASK34_STATE_FINISH            6

#define TASK34_STRAIGHT_SPEED          45.0f
#define TASK34_TRACE_SPEED             35.0f

#define TASK34_LINE_LOST_TICKS         5
#define TASK34_LED_TICKS               10

#define TASK34_LED_ON                  0
#define TASK34_LED_OFF                 1


#define TASK34_START_AC_YAW_OFFSET     0.0f//初始方向为A指向C
#define TASK34_DIAG_ANGLE              (38.66f * PI / 180.0f)
#define TASK34_NEXT_AC_YAW_OFFSET      (-TASK34_DIAG_ANGLE)//A到C
#define TASK34_BD_YAW_OFFSET           (+TASK34_DIAG_ANGLE)//B到D

#define TASK34_CB_TRACE_SIGN           1.0f
#define TASK34_DA_TRACE_SIGN           1.0f


#endif
