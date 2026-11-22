#include "Time.h"
#include "ti_msp_dl_config.h"

volatile uint8_t imuflag = 0;
volatile uint8_t controlflag = 0;
volatile uint8_t vofaflag = 0;

void Time_Init(void)
{
    DL_TimerG_clearInterruptStatus(TIMER_0_INST, DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableInterrupt(TIMER_0_INST, DL_TIMERG_INTERRUPT_ZERO_EVENT);
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerG_startCounter(TIMER_0_INST);

    DL_TimerG_clearInterruptStatus(TIMER_1_INST, DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableInterrupt(TIMER_1_INST, DL_TIMERG_INTERRUPT_ZERO_EVENT);
    NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
    DL_TimerG_startCounter(TIMER_1_INST);

    DL_TimerA_clearInterruptStatus(TIMER_2_INST, DL_TIMERA_INTERRUPT_ZERO_EVENT);
    DL_TimerA_enableInterrupt(TIMER_2_INST, DL_TIMERA_INTERRUPT_ZERO_EVENT);
    NVIC_ClearPendingIRQ(TIMER_2_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_2_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_2_INST);
}

/* TIMER_0 (TIMG6): 10ms -> 100Hz IMU update */
void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMERG_IIDX_ZERO:
            imuflag = 1;
            break;
        default:
            break;
    }
}

/* TIMER_1 (TIMG0): 20ms -> 50Hz motor/encoder/task control loop */
void TIMER_1_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_1_INST)) {
        case DL_TIMERG_IIDX_ZERO:
            controlflag = 1;
            break;
        default:
            break;
    }
}

/* TIMER_2 (TIMA1): 50ms -> 20Hz VOFA telemetry */
void TIMER_2_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(TIMER_2_INST)) {
        case DL_TIMERA_IIDX_ZERO:
            vofaflag = 1;
            break;
        default:
            break;
    }
}
