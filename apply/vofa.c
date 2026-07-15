#include "vofa.h"


VofaCommand_t vofa_command;
volatile uint8_t vofa_rx_index;
static Motors_t *vofa_motors;


void Vofa_SendByte(uint8_t data)
{
    DL_UART_Main_transmitDataBlocking(UART_2_INST, data);
}

void Vofa_SendData(const uint8_t *data, uint16_t length)
{
    while (length > 0) {
        Vofa_SendByte(*data);
        data++;
        length--;
    }
}

void Vofa_SendGrayFireWater(const GraySensor_t *gray_sensor)
{
    char frame[64];
    int length;

    if (gray_sensor == NULL) {
        return;
    }

    /*
     * VOFA+ FireWater: optional text prefix, comma-separated ASCII values,
     * and a mandatory newline. The ADC readings remain decimal integers.
     */
    length = snprintf(
        frame,
        sizeof(frame),
        "gray:%u,%u,%u,%u,%u,%u,%u,%u\r\n",
        (unsigned int)gray_sensor->adc0_raw,
        (unsigned int)gray_sensor->adc1_raw,
        (unsigned int)gray_sensor->adc2_raw,
        (unsigned int)gray_sensor->adc3_raw,
        (unsigned int)gray_sensor->adc4_raw,
        (unsigned int)gray_sensor->adc5_raw,
        (unsigned int)gray_sensor->adc6_raw,
        (unsigned int)gray_sensor->adc7_raw);

    if ((length > 0) && (length < (int)sizeof(frame))) {
        Vofa_SendData((const uint8_t *)frame, (uint16_t)length);
    }
}

static float Vofa_BytesToFloat(const uint8_t data[4])
{
    float value;

    
    memcpy(&value, data, sizeof(value));

    return value;
}

static void Vofa_FloatToBytes(float value, uint8_t data[4])
{
    memcpy(data, &value, sizeof(value));
}


void Vofa_UartReceiveByte(uint8_t byte)
{
  
    if (vofa_rx_index == 0) {
        if (byte != '@') {
            return;
        }
    }

    vofa_command.packet[vofa_rx_index++] = byte;

    if (vofa_rx_index >= VOFA_CMD_FRAME_SIZE) {
        vofa_rx_index = 0U;

        if ((vofa_command.packet[0] == '@') &&
            (vofa_command.packet[4] == '=') &&
            (vofa_command.packet[9] == '!') &&
            (vofa_command.packet[10] == '#')) {
            vofa_command.completion_flag = 1;
        }
    }
}
static Pid_t *Vofa_GetPid(uint8_t target)
{
    if (vofa_motors == NULL) {
        return NULL;
    }

    switch (target) {
        case VOFA_PID_LEFT:
            return vofa_motors->Left;

        case VOFA_PID_RIGHT:
            return vofa_motors->Right;

        case VOFA_PID_TURN:
            return vofa_motors->PidSelfTurn;

        default:
            return NULL;
    }
}

static float *Vofa_GetPidParameter(Pid_t *pid, uint8_t parameter)
{
    if (pid == NULL) {
        return NULL;
    }

    switch (parameter) {
        case VOFA_PARAM_KP:
            return &pid->Kp;

        case VOFA_PARAM_KI:
            return &pid->Ki;

        case VOFA_PARAM_KD:
            return &pid->Kd;

        default:
            return NULL;
    }
}

static uint8_t Vofa_ApplyParameter(
    Pid_t *pid,
    uint8_t parameter,
    uint8_t operation,
    float value)
{
    float *pid_parameter;

    pid_parameter = Vofa_GetPidParameter(pid, parameter);
    if (pid_parameter == NULL) {
        return 0;
    }

    switch (operation) {
        case VOFA_DIRECT:
            *pid_parameter = value;
            break;

        case VOFA_INCREASE:
            *pid_parameter += value;
            break;

        case VOFA_DECREASE:
            *pid_parameter -= value;
            break;

        default:
            return 0;
    }


    if (*pid_parameter < 0.0f) {
        *pid_parameter = 0.0f;
    }

    if (*pid_parameter > 1000.0f) {
        *pid_parameter = 1000.0f;
    }

    PID_clear(pid);
    return 1;
}

static uint8_t Vofa_ParseCommand(void)
{
    uint8_t target;
    uint8_t parameter;
    uint8_t operation;
    float value;
    Pid_t *pid;

    target    = vofa_command.packet[1];
    parameter = vofa_command.packet[2];
    operation = vofa_command.packet[3];

    value = Vofa_BytesToFloat(&vofa_command.packet[5]);

    /*
     * 排除 NaN、无穷值和异常数据。
     * 如果不检查，串口干扰可能直接破坏 PID。
     */
    if (!isfinite(value) || (value < 0.0f) || (value > 1000.0f)) {
        return 0U;
    }

    /*
     * S 表示左右轮速度环一起修改。
     */
    if (target == VOFA_PID_SPEED) {
        uint8_t left_ok;
        uint8_t right_ok;

        left_ok = Vofa_ApplyParameter(
            vofa_motors->Left,
            parameter,
            operation,
            value);

        right_ok = Vofa_ApplyParameter(
            vofa_motors->Right,
            parameter,
            operation,
            value);

        return (uint8_t)(left_ok && right_ok);
    }

    pid = Vofa_GetPid(target);

    return Vofa_ApplyParameter(
        pid,
        parameter,
        operation,
        value);
}

void Vofa_Task(void)
{
    if (vofa_command.completion_flag == 0) {
        return;
    }

    /*
     * 先清标志，防止处理时中断重复触发。
     */
    vofa_command.completion_flag = 0;

    Vofa_ParseCommand();

    memset(
        vofa_command.packet,
        0,
        sizeof(vofa_command.packet));
}

void Vofa_Init(Motors_t *motors)
{
    vofa_motors = motors;
    vofa_rx_index = 0;

    memset(&vofa_command, 0, sizeof(vofa_command));

    NVIC_ClearPendingIRQ(UART_2_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_2_INST_INT_IRQN);
}

void Vofa_SendJustFloat(void)
{
    uint8_t bytes[4];

    float channels[VOFA_CHANNEL_COUNT] = {
        vofa_motors->Left->set,
        vofa_motors->Left->fdb,
        vofa_motors->Left->out,

        vofa_motors->Right->set,
        vofa_motors->Right->fdb,
        vofa_motors->Right->out,

        vofa_motors->PidSelfTurn->error[0],
        vofa_motors->PidSelfTurn->out
    };

    static const uint8_t tail[4] = {
        0x00, 0x00, 0x80, 0x7F
    };

    for (uint8_t i = 0U; i < VOFA_CHANNEL_COUNT; i++) {
        Vofa_FloatToBytes(channels[i], bytes);
        Vofa_SendData(bytes, sizeof(bytes));
    }

    Vofa_SendData(tail, sizeof(tail));
}

//串口中断
void UART_2_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_2_INST)) {
        case DL_UART_MAIN_IIDX_RX:
        {
            uint8_t byte;

            byte = (uint8_t)DL_UART_Main_receiveData(UART_2_INST);
            Vofa_UartReceiveByte(byte);
            break;
        }

        default:
            break;
    }
}
