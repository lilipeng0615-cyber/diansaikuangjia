#ifndef __DATABYTE_H
#define __DATABYTE_H

#include "ti_msp_dl_config.h"
#include <stdint.h>

/* Basic integer aliases. */
typedef signed char int8;            /* signed 8-bit integer */
typedef unsigned char _u8;           /* unsigned 8-bit integer, legacy alias */
typedef unsigned char u8;            /* unsigned 8-bit integer */
typedef unsigned char uint8;         /* unsigned 8-bit integer */
typedef unsigned char byte;          /* byte data */

typedef signed short int int16;      /* signed 16-bit integer */
typedef unsigned short int uint16;   /* unsigned 16-bit integer */
typedef unsigned short int _u16;     /* unsigned 16-bit integer, legacy alias */
typedef unsigned short int u16;      /* unsigned 16-bit integer */

typedef unsigned long int _u32;      /* unsigned 32-bit integer, legacy alias */
typedef unsigned long int u32;       /* unsigned 32-bit integer */
typedef float fp32;                  /* 32-bit float */
typedef double fp64;                 /* 64-bit float */

#define ABS(X) (((X) > 0) ? (X) : -(X))          /* absolute value */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))      /* max value */
#define MIN(a, b) (((a) > (b)) ? (b) : (a))      /* min value */

#define SPEED_RECORD_NUM 5                       /* motor speed history length */
#define RING_BUF_SIZE 1024                       /* UART receive ring buffer size */
#define FRAME_HEAD1 0xAA                         /* UART frame header byte 1 */
#define FRAME_HEAD2 0x55                         /* UART frame header byte 2 */
#define FRAME_TAIL1 0xBB                         /* UART frame tail byte 1 */
#define FRAME_TAIL2 0xCC                         /* UART frame tail byte 2 */
#define MAX_DATA_LEN 256                         /* max UART frame payload length */

/* Encoder decoded feedback data. */
typedef struct {
    int v_int;                                   /* raw speed value from encoder protocol */
    int t_int;                                   /* raw torque/duty value from encoder protocol */
    int EncoderCount;                            /* accumulated encoder pulse count */
    float pos;                                   /* converted position, rad */
    float vel;                                   /* converted velocity, rad/s */
    float V;                                     /* feedback voltage or parsed voltage value */
    float X;                                     /* reserved position/displacement value */
} Encoder_t;

/* PID controller parameters and runtime state. */
typedef struct {
    uint8_t mode;                                /* PID mode: position or delta */
    fp32 Kp;                                     /* proportional gain */
    fp32 Ki;                                     /* integral gain */
    fp32 Kd;                                     /* derivative gain */
    fp32 max_out;                                /* total output limit */
    fp32 max_iout;                               /* integral output limit */
    fp32 set;                                    /* target value */
    fp32 fdb;                                    /* feedback value */
    fp32 out;                                    /* total PID output */
    fp32 Pout;                                   /* proportional output */
    fp32 Iout;                                   /* integral output */
    fp32 Dout;                                   /* derivative output */
    fp32 Dbuf[3];                                /* derivative cache: current, last, previous */
    fp32 error[3];                               /* error cache: current, last, previous */
} Pid_t;

/* Motor enable state. */
typedef enum {
    Enable = 1,                                  /* module or motor enabled */
    Disable = 0,                                 /* module or motor disabled */
} Motor_State;

/* Single motor encoder statistic state. */
typedef struct {
    uint8_t dirction;                            /* encoder direction flag, keeps original spelling */
    int32_t countnum;                            /* current encoder count */
    int32_t lastcount;                           /* previous encoder count */
    float speed;                                 /* calculated speed */
    float speed_Record[SPEED_RECORD_NUM];        /* speed moving-average buffer */
} encoder_t;

/* Motor control state. */
typedef struct {
    uint8_t dir;                                 /* motor direction */
    float pwmDuty;                               /* PWM duty ratio */
    float speed;                                 /* target or current speed */
    float encoder_speed;                         /* encoder feedback speed */
    float pos;                                   /* motor position feedback or target */
    encoder_t encoder;                           /* encoder state for this motor */
    float error;                                 /* current control error */
    float lasterror;                             /* previous control error */
    float integral;                              /* integrated control error */
} motor_t;

/* System time sample state. */
typedef struct {
    volatile float last_time;                    /* previous sample time, ms */
    volatile float current_time;                 /* current sample time, ms */
    volatile float period;                       /* sample interval, ms */
    volatile uint16_t period_int;                /* rounded interval, ms */
} systime;

/* UART receive ring queue. */
typedef struct {
    uint8_t buffer[RING_BUF_SIZE];               /* queue data buffer */
    uint16_t head;                               /* dequeue read index */
    uint16_t tail;                               /* enqueue write index */
    uint16_t size;                               /* buffered byte count */
} RingQueue_t;

/* UART frame parser state. */
typedef enum {
    PARSE_HEAD1,                                 /* waiting for header byte 1 */
    PARSE_HEAD2,                                 /* waiting for header byte 2 */
    PARSE_LEN,                                   /* waiting for payload length */
    PARSE_DATA,                                  /* receiving payload bytes */
    PARSE_CHECK,                                 /* waiting for checksum */
    PARSE_TAIL1,                                 /* waiting for tail byte 1 */
    PARSE_TAIL2                                  /* waiting for tail byte 2 */
} ParseState_t;

/* UART frame parser context. */
typedef struct {
    ParseState_t state;                          /* current parser state */
    uint8_t data_len;                            /* expected payload length */
    uint8_t data_buf[MAX_DATA_LEN];              /* payload buffer */
    uint8_t data_cnt;                            /* received payload byte count */
    uint8_t check_sum;                           /* accumulated checksum */
} FrameParse_t;

/* Three-axis vector for gyro, accel, and attitude algorithm input. */
typedef struct {
    float x;                                     /* X axis value */
    float y;                                     /* Y axis value */
    float z;                                     /* Z axis value */
} Axis3f;

/* Mahony quaternion attitude filter state. */
typedef struct {
    float Kp;                                    /* proportional feedback gain */
    float Ki;                                    /* integral feedback gain */
    float dt;                                    /* update period, s */
    float q0;                                    /* quaternion real part */
    float q1;                                    /* quaternion X imaginary part */
    float q2;                                    /* quaternion Y imaginary part */
    float q3;                                    /* quaternion Z imaginary part */
    float exInt;                                 /* X-axis error integral */
    float eyInt;                                 /* Y-axis error integral */
    float ezInt;                                 /* Z-axis error integral */
    float rMat[3][3];                            /* rotation matrix from quaternion */
    float roll;                                  /* roll angle, rad */
    float pitch;                                 /* pitch angle, rad */
    float yaw;                                   /* yaw angle, rad */
} Mahony_t;

/* ICM42688 sensor data and status. */
typedef struct {
    int16_t RawAccel[3];                         /* raw 3-axis accelerometer ADC data */
    int16_t RawGyro[3];                          /* raw 3-axis gyroscope ADC data */
    int16_t RawTemp;                             /* raw temperature ADC data */
    float Accel[3];                              /* acceleration, m/s^2 */
    float Gyro[3];                               /* angular rate, rad/s */
    float GyroOffset[3];                         /* gyroscope bias, rad/s */
    float Temperature;                           /* chip temperature, degC */
    uint8_t ChipID;                              /* WHO_AM_I chip ID */
    uint8_t InitError;                           /* initialization error code */
    uint8_t LastGyroSaturated;                   /* last gyro read was near full scale */
    uint8_t LastAccelSaturated;                  /* last accel read was near full scale */
} ICM42688_t;

/* ICM42688 initialization error flags. */
typedef enum {
    ICM42688_NO_ERROR = 0x00,                    /* no error */
    ICM42688_NO_SENSOR = 0x01,                   /* chip not found or WHO_AM_I mismatch */
    ICM42688_CONFIG_ERROR = 0x02,                /* register configuration failed */
} ICM42688_Error_t;

/* Attitude solver output state. */
typedef struct {
    float q[4];                                  /* quaternion q0/q1/q2/q3 */
    float Roll;                                  /* roll angle, rad */
    float Pitch;                                 /* pitch angle, rad */
    float Yaw;                                   /* yaw angle, rad, approximately -pi to pi */
    float YawTotalAngle;                         /* continuous yaw angle with wrap count */
    int32_t YawRoundCount;                       /* yaw wrap count */
} IMU_Attitude_t;

#endif
