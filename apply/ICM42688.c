#include "ICM42688.h"


#define ICM42688_REG_DEVICE_CONFIG       0x11U
#define ICM42688_REG_TEMP_DATA1          0x1DU
#define ICM42688_REG_PWR_MGMT0           0x4EU
#define ICM42688_REG_GYRO_CONFIG0        0x4FU
#define ICM42688_REG_ACCEL_CONFIG0       0x50U
#define ICM42688_REG_GYRO_ACCEL_CONFIG0  0x52U
#define ICM42688_REG_WHO_AM_I            0x75U
#define ICM42688_REG_BANK_SEL            0x76U

#define ICM42688_REG_GYRO_STATIC2        0x0BU
#define ICM42688_REG_GYRO_STATIC3        0x0CU
#define ICM42688_REG_GYRO_STATIC4        0x0DU
#define ICM42688_REG_GYRO_STATIC5        0x0EU
#define ICM42688_REG_ACCEL_STATIC2       0x03U
#define ICM42688_REG_ACCEL_STATIC3       0x04U
#define ICM42688_REG_ACCEL_STATIC4       0x05U

#define ICM42688_DEVICE_CONFIG_RESET     0x01U
#define ICM42688_PWR_GYRO_ACCEL_LN       0x0FU
#define ICM42688_ACCEL_16G_50HZ          0x09U
#define ICM42688_GYRO_2000DPS_50HZ       0x09U
#define ICM42688_UI_FILTER_20HZ          0x66U

#define ICM42688_BANK_0                  0x00U
#define ICM42688_BANK_1                  0x01U
#define ICM42688_BANK_2                  0x02U
#define ICM42688_AAF_42HZ_DELT           0x01U
#define ICM42688_AAF_42HZ_DELTSQR_LOW    0x01U
#define ICM42688_AAF_42HZ_SHIFT          0xF0U
#define ICM42688_ACCEL_AAF_42HZ_CONFIG   0x02U

#define ICM42688_RAW_SATURATION_LIMIT    32000
#define ICM42688_STATIC_GYRO_LIMIT       0.035f
#define ICM42688_STATIC_ACCEL_MIN        8.80f
#define ICM42688_STATIC_ACCEL_MAX        10.80f
#define ICM42688_CALIBRATION_RETRIES     3U
#define ICM42688_CALIBRATION_SAMPLE_MS   20U
#define ICM42688_CALIB_ACCEL_SPAN_LIMIT  0.80f
#define ICM42688_CALIB_ACCEL_ERROR_LIMIT 1.50f
#define ICM42688_CALIB_GYRO_SPAN_LIMIT   0.18f

#define ICM42688_CS_LOW()                DL_GPIO_clearPins(CS_PORT, CS_PIN_6_PIN)
#define ICM42688_CS_HIGH()               DL_GPIO_setPins(CS_PORT, CS_PIN_6_PIN)

ICM42688_t ICM42688;

static void icm42688_write_reg(uint8_t reg, uint8_t data)
{
    ICM42688_CS_LOW();
    (void)spi0_transfer_byte(reg & 0x7FU);
    (void)spi0_transfer_byte(data);
    spi0_wait_idle();
    ICM42688_CS_HIGH();
}

static void icm42688_read_regs(uint8_t reg, uint8_t *buf, uint16_t len)
{
    ICM42688_CS_LOW();
    (void)spi0_transfer_byte(reg | 0x80U);
    while (len-- != 0U) {
        *buf++ = spi0_transfer_byte(0x00U);
    }
    spi0_wait_idle();
    ICM42688_CS_HIGH();
}

static uint8_t icm42688_read_reg(uint8_t reg)
{
    uint8_t data = 0U;
    icm42688_read_regs(reg, &data, 1U);
    return data;
}

static uint8_t icm42688_write_check(uint8_t reg, uint8_t data)
{
    icm42688_write_reg(reg, data);
    delay_ms(1);
    return (icm42688_read_reg(reg) == data) ? 0U : 1U;
}

static void icm42688_select_bank(uint8_t bank)
{
    icm42688_write_reg(ICM42688_REG_BANK_SEL, bank);
    delay_ms(1U);
}

static uint8_t icm42688_configure_filters(void)
{
    uint8_t error = 0U;
    uint8_t gyro_static2;

    /*
     * At 50 Hz ODR, UI filter code 6 is about 20 Hz. AAF DELT=1,
     * DELTSQR=1 and BITSHIFT=15 select the datasheet's 42 Hz setting.
     */
    icm42688_select_bank(ICM42688_BANK_0);
    error |= icm42688_write_check(ICM42688_REG_GYRO_ACCEL_CONFIG0,
                                  ICM42688_UI_FILTER_20HZ);

    icm42688_select_bank(ICM42688_BANK_1);
    gyro_static2 = (uint8_t)(icm42688_read_reg(ICM42688_REG_GYRO_STATIC2) &
                             (uint8_t)~0x02U);
    error |= icm42688_write_check(ICM42688_REG_GYRO_STATIC2, gyro_static2);
    error |= icm42688_write_check(ICM42688_REG_GYRO_STATIC3, ICM42688_AAF_42HZ_DELT);
    error |= icm42688_write_check(ICM42688_REG_GYRO_STATIC4, ICM42688_AAF_42HZ_DELTSQR_LOW);
    error |= icm42688_write_check(ICM42688_REG_GYRO_STATIC5, ICM42688_AAF_42HZ_SHIFT);

    icm42688_select_bank(ICM42688_BANK_2);
    error |= icm42688_write_check(ICM42688_REG_ACCEL_STATIC2,
                                  ICM42688_ACCEL_AAF_42HZ_CONFIG);
    error |= icm42688_write_check(ICM42688_REG_ACCEL_STATIC3,
                                  ICM42688_AAF_42HZ_DELTSQR_LOW);
    error |= icm42688_write_check(ICM42688_REG_ACCEL_STATIC4,
                                  ICM42688_AAF_42HZ_SHIFT);

    icm42688_select_bank(ICM42688_BANK_0);
    return error;
}

static float icm42688_absf(float x)
{
    return (x >= 0.0f) ? x : -x;
}

static float icm42688_accel_norm(const ICM42688_t *imu)
{
    return sqrtf((imu->Accel[0] * imu->Accel[0]) +
                 (imu->Accel[1] * imu->Accel[1]) +
                 (imu->Accel[2] * imu->Accel[2]));
}

uint8_t ICM42688_ReadID(void)
{
    ICM42688.ChipID = icm42688_read_reg(ICM42688_REG_WHO_AM_I);
    return ICM42688.ChipID;
}

uint8_t ICM42688_Init(void)
{
    uint8_t error = ICM42688_NO_ERROR;

    memset(&ICM42688, 0, sizeof(ICM42688));
    ICM42688_CS_HIGH();
    delay_ms(3);

    icm42688_select_bank(ICM42688_BANK_0);

    if (ICM42688_ReadID() != ICM42688_CHIP_ID_VALUE) {
        delay_ms(2);
        if (ICM42688_ReadID() != ICM42688_CHIP_ID_VALUE) {
            ICM42688.InitError = ICM42688_NO_SENSOR;
            return ICM42688.InitError;
        }
    }

    icm42688_write_reg(ICM42688_REG_DEVICE_CONFIG, ICM42688_DEVICE_CONFIG_RESET);
    delay_ms(50);

    icm42688_select_bank(ICM42688_BANK_0);

    if (ICM42688_ReadID() != ICM42688_CHIP_ID_VALUE) {
        error |= ICM42688_NO_SENSOR;
    }

    error |= icm42688_write_check(ICM42688_REG_ACCEL_CONFIG0, ICM42688_ACCEL_16G_50HZ) ? ICM42688_CONFIG_ERROR : 0U;
    error |= icm42688_write_check(ICM42688_REG_GYRO_CONFIG0, ICM42688_GYRO_2000DPS_50HZ) ? ICM42688_CONFIG_ERROR : 0U;
    error |= icm42688_configure_filters() ? ICM42688_CONFIG_ERROR : 0U;

    icm42688_write_reg(ICM42688_REG_PWR_MGMT0, ICM42688_PWR_GYRO_ACCEL_LN);
    delay_ms(50);
    if ((icm42688_read_reg(ICM42688_REG_PWR_MGMT0) & ICM42688_PWR_GYRO_ACCEL_LN) != ICM42688_PWR_GYRO_ACCEL_LN) {
        error |= ICM42688_CONFIG_ERROR;
    }

    ICM42688.InitError = error;
    if (error == ICM42688_NO_ERROR) {
        ICM42688_CalibrateGyro(50U);
    }

    return ICM42688.InitError;
}

void ICM42688_Read(ICM42688_t *imu)
{
    uint8_t buf[14];
    uint8_t gyro_saturated = 0U;
    uint8_t accel_saturated = 0U;

    icm42688_read_regs(ICM42688_REG_TEMP_DATA1, buf, sizeof(buf));

    imu->RawTemp = (int16_t)(((uint16_t)buf[0] << 8U) | (uint16_t)buf[1]);
    imu->RawAccel[0] = (int16_t)(((uint16_t)buf[2] << 8U) | (uint16_t)buf[3]);
    imu->RawAccel[1] = (int16_t)(((uint16_t)buf[4] << 8U) | (uint16_t)buf[5]);
    imu->RawAccel[2] = (int16_t)(((uint16_t)buf[6] << 8U) | (uint16_t)buf[7]);
    imu->RawGyro[0] = (int16_t)(((uint16_t)buf[8] << 8U) | (uint16_t)buf[9]);
    imu->RawGyro[1] = (int16_t)(((uint16_t)buf[10] << 8U) | (uint16_t)buf[11]);
    imu->RawGyro[2] = (int16_t)(((uint16_t)buf[12] << 8U) | (uint16_t)buf[13]);

    imu->Temperature = ((float)imu->RawTemp / 132.48f) + 25.0f;

    for (uint8_t i = 0U; i < 3U; i++) {
        if ((imu->RawAccel[i] >= ICM42688_RAW_SATURATION_LIMIT) ||
            (imu->RawAccel[i] <= -ICM42688_RAW_SATURATION_LIMIT)) {
            accel_saturated = 1U;
        }
        if ((imu->RawGyro[i] >= ICM42688_RAW_SATURATION_LIMIT) ||
            (imu->RawGyro[i] <= -ICM42688_RAW_SATURATION_LIMIT)) {
            gyro_saturated = 1U;
        }
        imu->Accel[i] = (float)imu->RawAccel[i] * ICM42688_ACCEL_16G_SEN;
        imu->Gyro[i] = ((float)imu->RawGyro[i] * ICM42688_GYRO_2000DPS_SEN) - imu->GyroOffset[i];
    }

    imu->LastAccelSaturated = accel_saturated;
    imu->LastGyroSaturated = gyro_saturated;
}

void ICM42688_CalibrateGyro(uint16_t samples)
{
    float saved_offset[3];

    if (samples == 0U) {
        return;
    }

    for (uint8_t i = 0U; i < 3U; i++) {
        saved_offset[i] = ICM42688.GyroOffset[i];
        ICM42688.GyroOffset[i] = 0.0f;
    }

    for (uint8_t retry = 0U; retry < ICM42688_CALIBRATION_RETRIES; retry++) {
        float gyro_sum[3] = {0.0f, 0.0f, 0.0f};
        float gyro_min[3] = {0.0f, 0.0f, 0.0f};
        float gyro_max[3] = {0.0f, 0.0f, 0.0f};
        float accel_norm_sum = 0.0f;
        float accel_norm_min = 0.0f;
        float accel_norm_max = 0.0f;
        uint16_t valid_samples = 0U;

        for (uint16_t n = 0U; n < samples; n++) {
            float accel_norm;

            ICM42688_Read(&ICM42688);
            if ((ICM42688.LastGyroSaturated != 0U) ||
                (ICM42688.LastAccelSaturated != 0U)) {
                delay_ms(ICM42688_CALIBRATION_SAMPLE_MS);
                continue;
            }

            accel_norm = icm42688_accel_norm(&ICM42688);
            if (valid_samples == 0U) {
                accel_norm_min = accel_norm;
                accel_norm_max = accel_norm;
                for (uint8_t axis = 0U; axis < 3U; axis++) {
                    gyro_min[axis] = ICM42688.Gyro[axis];
                    gyro_max[axis] = ICM42688.Gyro[axis];
                }
            } else {
                if (accel_norm < accel_norm_min) {
                    accel_norm_min = accel_norm;
                }
                if (accel_norm > accel_norm_max) {
                    accel_norm_max = accel_norm;
                }
                for (uint8_t axis = 0U; axis < 3U; axis++) {
                    if (ICM42688.Gyro[axis] < gyro_min[axis]) {
                        gyro_min[axis] = ICM42688.Gyro[axis];
                    }
                    if (ICM42688.Gyro[axis] > gyro_max[axis]) {
                        gyro_max[axis] = ICM42688.Gyro[axis];
                    }
                }
            }

            accel_norm_sum += accel_norm;
            for (uint8_t axis = 0U; axis < 3U; axis++) {
                gyro_sum[axis] += ICM42688.Gyro[axis];
            }
            valid_samples++;
            delay_ms(ICM42688_CALIBRATION_SAMPLE_MS);
        }

        if ((valid_samples == 0U) ||
            (valid_samples < (samples / 2U)) ||
            ((accel_norm_max - accel_norm_min) > ICM42688_CALIB_ACCEL_SPAN_LIMIT) ||
            (icm42688_absf((accel_norm_sum / (float)valid_samples) - 9.80665f) >
             ICM42688_CALIB_ACCEL_ERROR_LIMIT)) {
            continue;
        }

        if (((gyro_max[0] - gyro_min[0]) > ICM42688_CALIB_GYRO_SPAN_LIMIT) ||
            ((gyro_max[1] - gyro_min[1]) > ICM42688_CALIB_GYRO_SPAN_LIMIT) ||
            ((gyro_max[2] - gyro_min[2]) > ICM42688_CALIB_GYRO_SPAN_LIMIT)) {
            continue;
        }

        for (uint8_t axis = 0U; axis < 3U; axis++) {
            ICM42688.GyroOffset[axis] = saved_offset[axis] +
                                        (gyro_sum[axis] / (float)valid_samples);
        }
        return;
    }

    /* Keep the previous offset if the board moved during every attempt. */
    for (uint8_t axis = 0U; axis < 3U; axis++) {
        ICM42688.GyroOffset[axis] = saved_offset[axis];
    }
}

void ICM42688_UpdateStaticGyroBias(float alpha)
{
    float acc_norm = icm42688_accel_norm(&ICM42688);

    if ((acc_norm < ICM42688_STATIC_ACCEL_MIN) || (acc_norm > ICM42688_STATIC_ACCEL_MAX)) {
        return;
    }
    if ((icm42688_absf(ICM42688.Gyro[0]) > ICM42688_STATIC_GYRO_LIMIT) ||
        (icm42688_absf(ICM42688.Gyro[1]) > ICM42688_STATIC_GYRO_LIMIT) ||
        (icm42688_absf(ICM42688.Gyro[2]) > ICM42688_STATIC_GYRO_LIMIT)) {
        return;
    }

    ICM42688.GyroOffset[0] += ICM42688.Gyro[0] * alpha;
    ICM42688.GyroOffset[1] += ICM42688.Gyro[1] * alpha;
    ICM42688.GyroOffset[2] += ICM42688.Gyro[2] * alpha;
}
