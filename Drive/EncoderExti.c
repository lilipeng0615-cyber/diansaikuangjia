#include "EncoderExti.h"

static uint16_t s_leftPrevious;
static uint16_t s_rightPrevious;

static int32_t EncoderDelta16(uint16_t current, uint16_t previous)
{
    return (int32_t)(int16_t)(uint16_t)(current - previous);
}

void EncoderExtiInit(void)
{
    DL_TimerG_setTimerCount(Encoder1_INST, 0);
    DL_TimerG_setTimerCount(Encoder2_INST, 0);

    s_leftPrevious = 0;
    s_rightPrevious = 0;

    DL_TimerG_startCounter(Encoder1_INST);
    DL_TimerG_startCounter(Encoder2_INST);
}

void EncoderExtiDataUpdate(Motors_t *motors)
{
    uint16_t leftCurrent;
    uint16_t rightCurrent;
    int32_t leftDelta;
    int32_t rightDelta;

    if ((motors == NULL) ||
        (motors->EncoderLeft == NULL) ||
        (motors->EncoderRight == NULL)) {
        return;
    }

    leftCurrent = (uint16_t)DL_TimerG_getTimerCount(Encoder1_INST);
    rightCurrent = (uint16_t)DL_TimerG_getTimerCount(Encoder2_INST);

    leftDelta = EncoderDelta16(leftCurrent, s_leftPrevious);
    rightDelta = EncoderDelta16(rightCurrent, s_rightPrevious);
    s_leftPrevious = leftCurrent;
    s_rightPrevious = rightCurrent;

    motors->EncoderLeft->EncoderCount +=
        leftDelta * ENCODER_LEFT_COUNT_SIGN;
    motors->EncoderRight->EncoderCount +=
        rightDelta * ENCODER_RIGHT_COUNT_SIGN;
}
