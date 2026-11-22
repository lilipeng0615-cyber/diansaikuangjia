#include "headfile.h"

Car_t car;

static uint32_t lcd_last_ms;

void duty_100hz(void)
{
    static uint32_t last_tick;
    static uint8_t initialized;
    uint32_t now_tick = micros();
    float dt;

    if (initialized == 0U) {
        last_tick = now_tick;
        initialized = 1U;
        return;
    }

    dt = (float)(now_tick - last_tick) * 0.000001f;
    last_tick = now_tick;
    IMU_Attitude_SetStaticBiasEnable((car.task == NULL) || (car.task->Carstartflag == 0));
    IMU_Attitude_Update(dt);
}

void duty_50hz(void)
{
    BuzzerDataUpdate(car.buzzer);
    EncoderDataUpdate(car.Motors);

    if ((car.task != NULL) && (car.task->Carstartflag != 0) && (car.task->StartDelayTicks > 0)) {
        MotorStop(car.Motors);
        car.task->StartDelayTicks--;
        if ((car.task->StartDelayTicks == (TASK_START_DELAY_TICKS / 2)) ||
            (car.task->StartDelayTicks == 0)) {
            BuzzerBee(car.buzzer);
        }
        return;
    }

    if (car.task->TaskFlag == 1) {
        Task1(car);
    } else if (car.task->TaskFlag == 2) {
        Task2(car);
    } else if (car.task->TaskFlag == 3) {
        Task3(car);
    } else if (car.task->TaskFlag == 4) {
        Task4(car);
    }
}

int main(void)
{
    uint8_t imu_err;

    SYSCFG_DL_init();
    uart0_init();
    MotorInit(&car.Motors);
    Vofa_Init(car.Motors);
    GraySensorInit(&car.GraySensor_t);
    EncodersInit(car.Motors);
    BeepInit(&car.buzzer);
    imu_err = ICM42688_Init();
    Task_Init(&car.task);
    IMU_Attitude_Init();
    car.IMU = &IMU_Attitude;
    LCD_Init();
    LCD_Clear(LCD_BLACK);
    LCD_DebugInit();
    LCD_ShowBootStatus(imu_err);
    if (imu_err != ICM42688_NO_ERROR) {
        BuzzerBee(car.buzzer);
    }
    Time_Init();

    while (1) {
        if (imuflag) {
            imuflag = 0;
            duty_100hz();
        }

        if (controlflag) {
            controlflag = 0;
            duty_50hz();
        }

        if (vofaflag) {
            vofaflag = 0;
            Vofa_SendJustFloat();
        }

        Key_Task_Handle();
        Vofa_Task();

        if ((car.task->Carstartflag == 0) && ((millis() - lcd_last_ms) >= 50U)) {
            lcd_last_ms = millis();
            LCD_DebugUpdate(car);
        }
    }
}
