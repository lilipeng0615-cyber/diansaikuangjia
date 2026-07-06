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

#define EncoderReadingFrequency 100.0f
#define EncoderLines 390.0f
#define TireRadius 6.5f



#endif
