#include "pid.h"

/**
 * @brief 限幅宏函数，将输入值限制在 [-max, max] 区间内
 * @param input 待限幅的变量（必须是可修改左值）
 * @param max 正向最大限幅幅值，负向自动取 -max
 * @note 宏内直接修改原变量，调用时不加 ; 否则会编译报错
 */
#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }

/**
 * @brief PID结构体初始化函数
 * @param pid PID结构体指针
 * @param mode PID模式：PID_POSITION位置式 / PID_DELTA增量式
 * @param PID[3] 参数数组，依次存放 Kp Ki Kd
 * @param max_out PID总输出最大限幅
 * @param max_iout 积分项单独限幅（防止积分饱和）
 * @retval 无
 * @note 会清空误差缓存、微分项缓存、各项输出缓存
 */
void PID_init(Pid_t *pid, uint8_t mode, const fp32 PID[3], fp32 max_out, fp32 max_iout)
{
    // 空指针保护，避免访问非法内存
    if (pid == NULL || PID == NULL)
    {
        return;
    }
    // PID工作模式赋值
    pid->mode = mode;
    // 复制PID三参数
    pid->Kp = PID[0];
    pid->Ki = PID[1];
    pid->Kd = PID[2];
    // 输出限幅参数赋值
    pid->max_out = max_out;
    pid->max_iout = max_iout;
    // 微分项历史缓存清零 Dbuf[0]当前微分、Dbuf[1]上一次、Dbuf[2]上两次
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    // 误差数组、比例/积分/微分/总输出全部清零
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
}

/**
 * @brief PID计算核心函数
 * @param pid PID结构体指针
 * @param ref 反馈值（实际采集到的当前值）
 * @param set 设定值（目标期望值）
 * @retval fp32 PID计算完成后的限幅输出值
 * @note 自动区分位置式PID、增量式PID两种算法
 */
fp32 PID_Calc(Pid_t *pid, fp32 ref, fp32 set)
{
    // 空指针保护
    if (pid == NULL)
    {
        return 0.0f;
    }

    // 误差队列移位：历史误差往前递推，腾出error[0]存最新误差
    pid->error[2] = pid->error[1]; // 上一次误差 → 上两次误差
    pid->error[1] = pid->error[0]; // 当前误差 → 上一次误差
    // 保存目标值与反馈值至结构体
    pid->set = set;
    pid->fdb = ref;
    // 计算当前偏差 e(k) = 设定值 - 实际反馈值
    pid->error[0] = set - ref;

    // ====================== 位置式PID算法 ======================
    if (pid->mode == PID_POSITION)
    {
        // 比例项输出 P = Kp * e(k)
        pid->Pout = pid->Kp * pid->error[0];
        // 积分项累加 I = I + Ki * e(k) （积分累积）
        pid->Iout += pid->Ki * pid->error[0];

        // 微分项缓存移位，存储误差差值用于微分计算
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        // 微分输入：本次误差 - 上次误差 Δe(k) = e(k)-e(k-1)
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
        // 微分项输出 D = Kd * Δe(k)
        pid->Dout = pid->Kd * pid->Dbuf[0];

        // 积分项单独限幅，抑制积分饱和
        LimitMax(pid->Iout, pid->max_iout);
        // PID总输出 = 比例 + 积分 + 微分
        pid->out = pid->Pout + pid->Iout + pid->Dout;
        // 总输出限幅，防止执行机构超量程
        LimitMax(pid->out, pid->max_out);
    }
    // ====================== 增量式PID算法 ======================
    else if (pid->mode == PID_DELTA)
    {
        // 比例增量：Kp*(e(k)-e(k-1))
        pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
        // 积分增量：Ki*e(k)
        pid->Iout = pid->Ki * pid->error[0];

        // 微分项缓存移位
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        // 微分增量：Kd*(e(k)-2e(k-1)+e(k-2))
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
        pid->Dout = pid->Kd * pid->Dbuf[0];

        // 增量叠加到上次输出上，得到本次总输出
        pid->out += pid->Pout + pid->Iout + pid->Dout;
        // 增量式最终输出限幅
        LimitMax(pid->out, pid->max_out);
    }
    // 返回限幅后的PID输出
    return pid->out;
}

/**
 * @brief PID缓存清空函数，复位所有状态
 * @param pid PID结构体指针
 * @retval 无
 * @note 切换目标、启停控制时调用，清除积分累积、历史误差、微分缓存，避免输出突变
 */
void PID_clear(Pid_t *pid)
{
    // 空指针保护
    if (pid == NULL)
    {
        return;
    }

    // 三段误差缓存清零
    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    // 微分历史缓存清零
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    // 所有输出项清零
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    // 设定值、反馈值缓存清零
    pid->fdb = pid->set = 0.0f;
}