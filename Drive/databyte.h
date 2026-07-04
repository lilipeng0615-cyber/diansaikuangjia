#ifndef __DATABYTE_H
#define __DATABYTE_H

#include "ti_msp_dl_config.h"
#include <stdint.h>

/* 基础整数类型别名 */
typedef signed char int8;            /* 有符号 8 位整数 */
typedef unsigned char _u8;           /* 无符号 8 位整数，兼容旧代码别名 */
typedef unsigned char u8;            /* 无符号 8 位整数 */
typedef unsigned char uint8;         /* 无符号 8 位整数 */
typedef unsigned char byte;          /* 字节数据 */

typedef signed short int int16;      /* 有符号 16 位整数 */
typedef unsigned short int uint16;   /* 无符号 16 位整数 */
typedef unsigned short int _u16;     /* 无符号 16 位整数，兼容旧代码别名 */
typedef unsigned short int u16;      /* 无符号 16 位整数 */

typedef unsigned long int _u32;      /* 无符号 32 位整数，兼容旧代码别名 */
typedef unsigned long int u32;       /* 无符号 32 位整数 */
typedef float fp32;                  /* 32 位浮点数 */
typedef double fp64;                 /* 64 位浮点数 */

#define ABS(X) (((X) > 0) ? (X) : -(X))          /* 取绝对值 */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))      /* 取较大值 */
#define MIN(a, b) (((a) > (b)) ? (b) : (a))      /* 取较小值 */

#define SPEED_RECORD_NUM 5                       /* 电机速度历史记录长度 */
#define RING_BUF_SIZE 1024                       /* 串口接收环形缓冲区大小 */
#define FRAME_HEAD1 0xAA                         /* 串口数据帧帧头字节 1 */
#define FRAME_HEAD2 0x55                         /* 串口数据帧帧头字节 2 */
#define FRAME_TAIL1 0xBB                         /* 串口数据帧帧尾字节 1 */
#define FRAME_TAIL2 0xCC                         /* 串口数据帧帧尾字节 2 */
#define MAX_DATA_LEN 256                         /* 串口数据帧最大有效数据长度 */

/* 编码器数据结构*/
typedef struct {
    int v_int;                                   /* 原始速度值 */
    int t_int;                                   /* 占空比值 */
    int EncoderCount;                            /* 编码器累计脉冲计数 */
    float pos;                                   /* 换算后的位置，单位 rad */
    float vel;                                   /* 换算后的速度，单位 rad/s */
    float V;                                     /* 反馈电压或解析得到的电压值 */
    float X;                                     /* 预留的位置或位移数据 */
} Encoder_t;

/* PID 控制器参数和运行状态 */
typedef struct {
    uint8_t mode;                                /* PID 模式：位置式或增量式 */
    fp32 Kp;                                     /* 比例系数 */
    fp32 Ki;                                     /* 积分系数 */
    fp32 Kd;                                     /* 微分系数 */
    fp32 max_out;                                /* PID 总输出限幅 */
    fp32 max_iout;                               /* 积分项输出限幅 */
    fp32 set;                                    /* 目标值 */
    fp32 fdb;                                    /* 反馈值 */
    fp32 out;                                    /* PID 总输出 */
    fp32 Pout;                                   /* 比例项输出 */
    fp32 Iout;                                   /* 积分项输出 */
    fp32 Dout;                                   /* 微分项输出 */
    fp32 Dbuf[3];                                /* 微分缓存：当前值、上一次、上上次 */
    fp32 error[3];                               /* 误差缓存：当前误差、上一次、上上次 */
} Pid_t;

/* 电机使能状态 */
typedef enum {
    Enable = 1,                                  /* 模块或电机使能 */
    Disable = 0,                                 /* 模块或电机失能 */
} Motor_State;

/* 电机结构 */
typedef struct {
	Motor_State state;                           /*电机状态*/
	  int ERF;                                   /*编码器读取频率*/
	  int EL;                                     /*编玛器线数*/
	  float TR;                                  /*轮胎半径*/	  
	  int ExpectOutput;
	  int Output;
} BrushMotor_t;

/* 电机组结构 */
typedef struct {
    Pid_t* PidSelfTurn;
	  BrushMotor_t* MotorLeft;
	  BrushMotor_t* MotorRight;
	  Encoder_t* EncoderLeft;
	  Encoder_t* EncoderRight;
	  Pid_t* Left;
	  Pid_t* Right;
} Motors_t;

/* 系统采样时间状态 */
typedef struct {
    volatile float last_time;                    /* 上一次采样时间，单位 ms */
    volatile float current_time;                 /* 当前采样时间，单位 ms */
    volatile float period;                       /* 采样周期，单位 ms */
    volatile uint16_t period_int;                /* 取整后的采样周期，单位 ms */
} systime;

/* 串口接收环形队列 */
typedef struct {
    uint8_t buffer[RING_BUF_SIZE];               /* 队列数据缓存区 */
    uint16_t head;                               /* 出队读取下标 */
    uint16_t tail;                               /* 入队写入下标 */
    uint16_t size;                               /* 当前已缓存字节数 */
} RingQueue_t;

/* 串口数据帧解析状态 */
typedef enum {
    PARSE_HEAD1,                                 /* 等待帧头字节 1 */
    PARSE_HEAD2,                                 /* 等待帧头字节 2 */
    PARSE_LEN,                                   /* 等待有效数据长度 */
    PARSE_DATA,                                  /* 接收有效数据字节 */
    PARSE_CHECK,                                 /* 等待校验和 */
    PARSE_TAIL1,                                 /* 等待帧尾字节 1 */
    PARSE_TAIL2                                  /* 等待帧尾字节 2 */
} ParseState_t;

/* 串口数据帧解析上下文 */
typedef struct {
    ParseState_t state;                          /* 当前解析状态 */
    uint8_t data_len;                            /* 期望接收的有效数据长度 */
    uint8_t data_buf[MAX_DATA_LEN];              /* 有效数据缓存区 */
    uint8_t data_cnt;                            /* 已接收的有效数据字节数 */
    uint8_t check_sum;                           /* 累加校验和 */
} FrameParse_t;

/* 三轴向量，用于陀螺仪、加速度计和姿态算法输入 */
typedef struct {
    float x;                                     /* X 轴数值 */
    float y;                                     /* Y 轴数值 */
    float z;                                     /* Z 轴数值 */
} Axis3f;

/* Mahony 四元数姿态滤波器状态 */
typedef struct {
    float Kp;                                    /* 比例反馈增益 */
    float Ki;                                    /* 积分反馈增益 */
    float dt;                                    /* 姿态更新周期，单位 s */
    float q0;                                    /* 四元数实部 */
    float q1;                                    /* 四元数 X 轴虚部 */
    float q2;                                    /* 四元数 Y 轴虚部 */
    float q3;                                    /* 四元数 Z 轴虚部 */
    float exInt;                                 /* X 轴误差积分 */
    float eyInt;                                 /* Y 轴误差积分 */
    float ezInt;                                 /* Z 轴误差积分 */
    float rMat[3][3];                            /* 由四元数计算得到的旋转矩阵 */
    float roll;                                  /* 横滚角，单位 rad */
    float pitch;                                 /* 俯仰角，单位 rad */
    float yaw;                                   /* 偏航角，单位 rad */
} Mahony_t;

/* ICM42688 传感器数据和状态 */
typedef struct {
    int16_t RawAccel[3];                         /* 三轴加速度计原始 ADC 数据 */
    int16_t RawGyro[3];                          /* 三轴陀螺仪原始 ADC 数据 */
    int16_t RawTemp;                             /* 温度原始 ADC 数据 */
    float Accel[3];                              /* 加速度，单位 m/s^2 */
    float Gyro[3];                               /* 角速度，单位 rad/s */
    float GyroOffset[3];                         /* 陀螺仪零偏，单位 rad/s */
    float Temperature;                           /* 芯片温度，单位 摄氏度 */
    uint8_t ChipID;                              /* WHO_AM_I 芯片 ID */
    uint8_t InitError;                           /* 初始化错误码 */
    uint8_t LastGyroSaturated;                   /* 上一次陀螺仪读数是否接近量程上限 */
    uint8_t LastAccelSaturated;                  /* 上一次加速度计读数是否接近量程上限 */
} ICM42688_t;

/* ICM42688 初始化错误标志 */
typedef enum {
    ICM42688_NO_ERROR = 0x00,                    /* 无错误 */
    ICM42688_NO_SENSOR = 0x01,                   /* 未找到芯片或 WHO_AM_I 不匹配 */
    ICM42688_CONFIG_ERROR = 0x02,                /* 寄存器配置失败 */
} ICM42688_Error_t;

/* 姿态解算输出状态 */
typedef struct {
    float q[4];                                  /* 四元数 q0/q1/q2/q3 */
    float Roll;                                  /* 横滚角，单位 rad */
    float Pitch;                                 /* 俯仰角，单位 rad */
    float Yaw;                                   /* 偏航角，单位 rad，范围约为 -pi 到 pi */
    float YawTotalAngle;                         /* 带圈数累计的连续偏航角 */
    int32_t YawRoundCount;                       /* 偏航角跨越正负 pi 的圈数计数 */
} IMU_Attitude_t;

#endif
