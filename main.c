#include "headfile.h"

int main()
{
    uint32_t last_tick;
    uint32_t now_tick;
    float dt;

    SYSCFG_DL_init();

    if (ICM42688_Init() != ICM42688_NO_ERROR) {
        while (1) {
            led_up();
            delay_ms(200);
        }
    }

    IMU_Attitude_Init();
    last_tick = micros();

    while (1) {
        now_tick = micros();
        dt = (float)(now_tick - last_tick) * 0.000001f;
        last_tick = now_tick;

        IMU_Attitude_Update(dt);

        delay_ms(1);
    }
}
