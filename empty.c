#include "headfile.h"

Car_t car;
extern volatile uint8 imuflag;
extern volatile uint8 controlflag;

static uint32_t lcd_last_ms;

void duty_100hz(void)
{
    static uint32_t last_tick;
    uint32_t now_tick = micros();
    float dt = (float)(now_tick - last_tick) * 0.000001f;

    last_tick = now_tick;
    IMU_Attitude_Update(dt);
}

void duty_50hz(void)
{
    EncoderDataUpdate(car.Motors);

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
    SYSCFG_DL_init();
    MotorInit(&car.Motors);
    GraySensorInit(&car.GraySensor_t);
    EncodersInit(car.Motors);
    BeepInit(&car.buzzer);
    ICM42688_Init();
    Task_Init(&car.task);
    IMU_Attitude_Init();
    LCD_Init();
    LCD_Clear(LCD_BLACK);
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

        Key_Task_Handle();

        if ((millis() - lcd_last_ms) >= 100U) {
            lcd_last_ms = millis();
            LCD_DebugUpdate(car);
        }
    }
}
