#include "LCD_ST7735.h"
#include "Delay.h"
#include "spi.h"
#include "lcdfont.h"
#include "imu_attitude.h"
#include <stdio.h>

#define LCD_DC_LOW()      DL_GPIO_clearPins(DC_PORT, DC_PIN_11_PIN)
#define LCD_DC_HIGH()     DL_GPIO_setPins(DC_PORT, DC_PIN_11_PIN)
#define LCD_RES_LOW()     DL_GPIO_clearPins(RES_PORT, RES_PIN_12_PIN)
#define LCD_RES_HIGH()    DL_GPIO_setPins(RES_PORT, RES_PIN_12_PIN)
#define LCD_BL_LOW()      DL_GPIO_clearPins(BL_PORT, BL_PIN_14_PIN)
#define LCD_BL_HIGH()     DL_GPIO_setPins(BL_PORT, BL_PIN_14_PIN)

static void lcd_write_bus(uint8_t data)
{
    (void)spi1_transfer_byte(data);
}

void LCD_WR_DATA8(uint8_t data)
{
    LCD_DC_HIGH();
    lcd_write_bus(data);
}

void LCD_WR_DATA(uint16_t data)
{
    LCD_WR_DATA8((uint8_t)(data >> 8U));
    LCD_WR_DATA8((uint8_t)data);
}

void LCD_WR_REG(uint8_t reg)
{
    LCD_DC_LOW();
    lcd_write_bus(reg);
    LCD_DC_HIGH();
}

void LCD_SetBacklight(uint8_t on)
{
    if (on != 0U) {
        LCD_BL_HIGH();
    } else {
        LCD_BL_LOW();
    }
}

void LCD_DisplayOn(void)
{
    LCD_WR_REG(0x29U);
}

void LCD_DisplayOff(void)
{
    LCD_WR_REG(0x28U);
}

void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (x1 >= LCD_W) {
        x1 = LCD_W - 1U;
    }
    if (x2 >= LCD_W) {
        x2 = LCD_W - 1U;
    }
    if (y1 >= LCD_H) {
        y1 = LCD_H - 1U;
    }
    if (y2 >= LCD_H) {
        y2 = LCD_H - 1U;
    }

#if (LCD_ST7735_ROTATION == 0U) || (LCD_ST7735_ROTATION == 1U)
    x1 += 2U;
    x2 += 2U;
    y1 += 1U;
    y2 += 1U;
#else
    x1 += 1U;
    x2 += 1U;
    y1 += 2U;
    y2 += 2U;
#endif

    LCD_WR_REG(0x2AU);
    LCD_WR_DATA(x1);
    LCD_WR_DATA(x2);
    LCD_WR_REG(0x2BU);
    LCD_WR_DATA(y1);
    LCD_WR_DATA(y2);
    LCD_WR_REG(0x2CU);
}

void LCD_Init(void)
{
    DL_GPIO_clearPins(CS2_PORT, CS2_PIN_15_PIN);
    LCD_DC_HIGH();
    LCD_SetBacklight(0U);

    LCD_RES_LOW();
    delay_ms(100U);
    LCD_RES_HIGH();
    delay_ms(100U);

    LCD_WR_REG(0x11U);
    delay_ms(120U);

    LCD_WR_REG(0xB1U);
    LCD_WR_DATA8(0x05U);
    LCD_WR_DATA8(0x3CU);
    LCD_WR_DATA8(0x3CU);
    LCD_WR_REG(0xB2U);
    LCD_WR_DATA8(0x05U);
    LCD_WR_DATA8(0x3CU);
    LCD_WR_DATA8(0x3CU);
    LCD_WR_REG(0xB3U);
    LCD_WR_DATA8(0x05U);
    LCD_WR_DATA8(0x3CU);
    LCD_WR_DATA8(0x3CU);
    LCD_WR_DATA8(0x05U);
    LCD_WR_DATA8(0x3CU);
    LCD_WR_DATA8(0x3CU);

    LCD_WR_REG(0xB4U);
    LCD_WR_DATA8(0x03U);

    LCD_WR_REG(0xC0U);
    LCD_WR_DATA8(0x28U);
    LCD_WR_DATA8(0x08U);
    LCD_WR_DATA8(0x04U);
    LCD_WR_REG(0xC1U);
    LCD_WR_DATA8(0xC0U);
    LCD_WR_REG(0xC2U);
    LCD_WR_DATA8(0x0DU);
    LCD_WR_DATA8(0x00U);
    LCD_WR_REG(0xC3U);
    LCD_WR_DATA8(0x8DU);
    LCD_WR_DATA8(0x2AU);
    LCD_WR_REG(0xC4U);
    LCD_WR_DATA8(0x8DU);
    LCD_WR_DATA8(0xEEU);

    LCD_WR_REG(0xC5U);
    LCD_WR_DATA8(0x1AU);

    LCD_WR_REG(0x36U);
#if LCD_ST7735_ROTATION == 0U
    LCD_WR_DATA8(0x00U);
#elif LCD_ST7735_ROTATION == 1U
    LCD_WR_DATA8(0xC0U);
#elif LCD_ST7735_ROTATION == 2U
    LCD_WR_DATA8(0x70U);
#else
    LCD_WR_DATA8(0xA0U);
#endif

    LCD_WR_REG(0xE0U);
    LCD_WR_DATA8(0x04U);
    LCD_WR_DATA8(0x22U);
    LCD_WR_DATA8(0x07U);
    LCD_WR_DATA8(0x0AU);
    LCD_WR_DATA8(0x2EU);
    LCD_WR_DATA8(0x30U);
    LCD_WR_DATA8(0x25U);
    LCD_WR_DATA8(0x2AU);
    LCD_WR_DATA8(0x28U);
    LCD_WR_DATA8(0x26U);
    LCD_WR_DATA8(0x2EU);
    LCD_WR_DATA8(0x3AU);
    LCD_WR_DATA8(0x00U);
    LCD_WR_DATA8(0x01U);
    LCD_WR_DATA8(0x03U);
    LCD_WR_DATA8(0x13U);

    LCD_WR_REG(0xE1U);
    LCD_WR_DATA8(0x04U);
    LCD_WR_DATA8(0x16U);
    LCD_WR_DATA8(0x06U);
    LCD_WR_DATA8(0x0DU);
    LCD_WR_DATA8(0x2DU);
    LCD_WR_DATA8(0x26U);
    LCD_WR_DATA8(0x23U);
    LCD_WR_DATA8(0x27U);
    LCD_WR_DATA8(0x27U);
    LCD_WR_DATA8(0x25U);
    LCD_WR_DATA8(0x2DU);
    LCD_WR_DATA8(0x3BU);
    LCD_WR_DATA8(0x00U);
    LCD_WR_DATA8(0x01U);
    LCD_WR_DATA8(0x04U);
    LCD_WR_DATA8(0x13U);

    LCD_WR_REG(0x3AU);
    LCD_WR_DATA8(0x05U);
    LCD_DisplayOn();
    delay_ms(20U);
    LCD_Clear(LCD_BLACK);
    LCD_SetBacklight(1U);
}

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint32_t total;

    if ((xsta >= LCD_W) || (ysta >= LCD_H) || (xsta >= xend) || (ysta >= yend)) {
        return;
    }
    if (xend > LCD_W) {
        xend = LCD_W;
    }
    if (yend > LCD_H) {
        yend = LCD_H;
    }

    total = (uint32_t)(xend - xsta) * (uint32_t)(yend - ysta);
    LCD_Address_Set(xsta, ysta, xend - 1U, yend - 1U);
    while (total-- != 0U) {
        LCD_WR_DATA(color);
    }
}

void LCD_Clear(uint16_t color)
{
    LCD_Fill(0U, 0U, LCD_W, LCD_H, color);
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x >= LCD_W) || (y >= LCD_H)) {
        return;
    }

    LCD_Address_Set(x, y, x, y);
    LCD_WR_DATA(color);
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int32_t dx = (int32_t)x2 - (int32_t)x1;
    int32_t dy = (int32_t)y2 - (int32_t)y1;
    int32_t sx = (dx >= 0) ? 1 : -1;
    int32_t sy = (dy >= 0) ? 1 : -1;
    int32_t err;

    if (dx < 0) {
        dx = -dx;
    }
    if (dy < 0) {
        dy = -dy;
    }
    err = ((dx > dy) ? dx : -dy) / 2;

    while (1) {
        LCD_DrawPoint(x1, y1, color);
        if ((x1 == x2) && (y1 == y2)) {
            break;
        }
        if (err > -dx) {
            err -= dy;
            x1 = (uint16_t)((int32_t)x1 + sx);
        }
        if (err < dy) {
            err += dx;
            y1 = (uint16_t)((int32_t)y1 + sy);
        }
    }
}

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int32_t x = 0;
    int32_t y = r;
    int32_t d = 3 - ((int32_t)r * 2);

    while (x <= y) {
        LCD_DrawPoint((uint16_t)(x0 + x), (uint16_t)(y0 + y), color);
        LCD_DrawPoint((uint16_t)(x0 - x), (uint16_t)(y0 + y), color);
        LCD_DrawPoint((uint16_t)(x0 + x), (uint16_t)(y0 - y), color);
        LCD_DrawPoint((uint16_t)(x0 - x), (uint16_t)(y0 - y), color);
        LCD_DrawPoint((uint16_t)(x0 + y), (uint16_t)(y0 + x), color);
        LCD_DrawPoint((uint16_t)(x0 - y), (uint16_t)(y0 + x), color);
        LCD_DrawPoint((uint16_t)(x0 + y), (uint16_t)(y0 - x), color);
        LCD_DrawPoint((uint16_t)(x0 - y), (uint16_t)(y0 - x), color);

        if (d < 0) {
            d += (4 * x) + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    LCD_DrawCircle(x0, y0, r, color);
}

static void lcd_draw_bitmap(uint16_t x, uint16_t y, const uint8_t *bitmap,
                            uint16_t width, uint16_t height,
                            uint16_t fc, uint16_t bc, uint8_t mode)
{
    uint16_t bytes;
    uint16_t i;
    uint8_t j;
    uint16_t drawn = 0U;
    uint16_t x0 = x;

    if ((bitmap == NULL) || (width == 0U) || (height == 0U) ||
        (x >= LCD_W) || (y >= LCD_H) ||
        ((x + width) > LCD_W) || ((y + height) > LCD_H)) {
        return;
    }

    bytes = (uint16_t)(((width + 7U) / 8U) * height);
    if (mode == 0U) {
        LCD_Address_Set(x, y, x + width - 1U, y + height - 1U);
    }

    for (i = 0U; i < bytes; i++) {
        uint8_t temp = bitmap[i];
        for (j = 0U; j < 8U; j++) {
            if (mode == 0U) {
                LCD_WR_DATA((temp & (0x01U << j)) ? fc : bc);
                drawn++;
                if ((drawn % width) == 0U) {
                    break;
                }
            } else {
                if ((temp & (0x01U << j)) != 0U) {
                    LCD_DrawPoint(x, y, fc);
                }
                x++;
                if ((x - x0) == width) {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

static void lcd_next_chinese_code(uint8_t **s, uint8_t code[2])
{
    uint8_t *p = *s;

    if ((p[0] == 0xE4U) && (p[1] == 0xB8U) && (p[2] == 0xADU)) {
        code[0] = 0xD6U;
        code[1] = 0xD0U;
        *s += 3U;
    } else if ((p[0] == 0xE6U) && (p[1] == 0x99U) && (p[2] == 0xAFU)) {
        code[0] = 0xBEU;
        code[1] = 0xB0U;
        *s += 3U;
    } else if ((p[0] == 0xE5U) && (p[1] == 0x9BU) && (p[2] == 0xADU)) {
        code[0] = 0xD4U;
        code[1] = 0xB0U;
        *s += 3U;
    } else if ((p[0] == 0xE7U) && (p[1] == 0x94U) && (p[2] == 0xB5U)) {
        code[0] = 0xB5U;
        code[1] = 0xE7U;
        *s += 3U;
    } else if ((p[0] == 0xE5U) && (p[1] == 0xADU) && (p[2] == 0x90U)) {
        code[0] = 0xD7U;
        code[1] = 0xD3U;
        *s += 3U;
    } else {
        code[0] = p[0];
        code[1] = p[1];
        *s += 2U;
    }
}

void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint16_t k;
    uint16_t count = (uint16_t)(sizeof(tfont12) / sizeof(tfont12[0]));

    (void)sizey;
    for (k = 0U; k < count; k++) {
        if ((tfont12[k].Index[0] == s[0]) && (tfont12[k].Index[1] == s[1])) {
            lcd_draw_bitmap(x, y, tfont12[k].Msk, 12U, 12U, fc, bc, mode);
            return;
        }
    }
}

void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint16_t k;
    uint16_t count = (uint16_t)(sizeof(tfont16) / sizeof(tfont16[0]));

    (void)sizey;
    for (k = 0U; k < count; k++) {
        if ((tfont16[k].Index[0] == s[0]) && (tfont16[k].Index[1] == s[1])) {
            lcd_draw_bitmap(x, y, tfont16[k].Msk, 16U, 16U, fc, bc, mode);
            return;
        }
    }
}

void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint16_t k;
    uint16_t count = (uint16_t)(sizeof(tfont24) / sizeof(tfont24[0]));

    (void)sizey;
    for (k = 0U; k < count; k++) {
        if ((tfont24[k].Index[0] == s[0]) && (tfont24[k].Index[1] == s[1])) {
            lcd_draw_bitmap(x, y, tfont24[k].Msk, 24U, 24U, fc, bc, mode);
            return;
        }
    }
}

void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint16_t k;
    uint16_t count = (uint16_t)(sizeof(tfont32) / sizeof(tfont32[0]));

    (void)sizey;
    for (k = 0U; k < count; k++) {
        if ((tfont32[k].Index[0] == s[0]) && (tfont32[k].Index[1] == s[1])) {
            lcd_draw_bitmap(x, y, tfont32[k].Msk, 32U, 32U, fc, bc, mode);
            return;
        }
    }
}

void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    while (*s != 0U) {
        uint8_t code[2];

        lcd_next_chinese_code(&s, code);
        if (sizey == 12U) {
            LCD_ShowChinese12x12(x, y, code, fc, bc, sizey, mode);
        } else if (sizey == 16U) {
            LCD_ShowChinese16x16(x, y, code, fc, bc, sizey, mode);
        } else if (sizey == 24U) {
            LCD_ShowChinese24x24(x, y, code, fc, bc, sizey, mode);
        } else if (sizey == 32U) {
            LCD_ShowChinese32x32(x, y, code, fc, bc, sizey, mode);
        } else {
            return;
        }
        x = (uint16_t)(x + sizey);
    }
}

void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    const uint8_t *bitmap = NULL;
    uint8_t width = (uint8_t)(sizey / 2U);

    if ((num < ' ') || (num > '~')) {
        num = ' ';
    }
    num = (uint8_t)(num - ' ');

    if (sizey == 12U) {
        bitmap = ascii_1206[num];
    } else if (sizey == 16U) {
        bitmap = ascii_1608[num];
    } else if (sizey == 24U) {
        bitmap = ascii_2412[num];
    } else if (sizey == 32U) {
        bitmap = ascii_3216[num];
    } else {
        return;
    }

    lcd_draw_bitmap(x, y, bitmap, width, sizey, fc, bc, mode);
}

void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t width = (uint8_t)(sizey / 2U);

    while ((p != NULL) && (*p != 0U)) {
        LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
        x = (uint16_t)(x + width);
        p++;
    }
}

uint32_t mypow(uint8_t m, uint8_t n)
{
    uint32_t result = 1U;

    while (n-- != 0U) {
        result *= m;
    }
    return result;
}

void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t t;
    uint8_t temp;
    uint8_t enshow = 0U;
    uint8_t sizex = (uint8_t)(sizey / 2U);

    for (t = 0U; t < len; t++) {
        temp = (uint8_t)((num / mypow(10U, (uint8_t)(len - t - 1U))) % 10U);
        if ((enshow == 0U) && (t < (len - 1U))) {
            if (temp == 0U) {
                LCD_ShowChar((uint16_t)(x + t * sizex), y, ' ', fc, bc, sizey, 0U);
                continue;
            } else {
                enshow = 1U;
            }
        }
        LCD_ShowChar((uint16_t)(x + t * sizex), y, (uint8_t)(temp + '0'), fc, bc, sizey, 0U);
    }
}

void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t t;
    uint8_t temp;
    uint8_t sizex = (uint8_t)(sizey / 2U);
    uint32_t num1;

    if (num < 0.0f) {
        LCD_ShowChar(x, y, '-', fc, bc, sizey, 0U);
        x = (uint16_t)(x + sizex);
        num = -num;
    }

    num1 = (uint32_t)(num * 100.0f);
    for (t = 0U; t < len; t++) {
        temp = (uint8_t)((num1 / mypow(10U, (uint8_t)(len - t - 1U))) % 10U);
        if (t == (len - 2U)) {
            LCD_ShowChar((uint16_t)(x + (len - 2U) * sizex), y, '.', fc, bc, sizey, 0U);
            t++;
            len++;
        }
        LCD_ShowChar((uint16_t)(x + t * sizex), y, (uint8_t)(temp + '0'), fc, bc, sizey, 0U);
    }
}

void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t pic[])
{
    uint32_t i;
    uint32_t total;

    if ((pic == NULL) || (width == 0U) || (height == 0U) || (x >= LCD_W) || (y >= LCD_H)) {
        return;
    }
    if ((x + width) > LCD_W) {
        width = LCD_W - x;
    }
    if ((y + height) > LCD_H) {
        height = LCD_H - y;
    }

    total = (uint32_t)width * (uint32_t)height;
    LCD_Address_Set(x, y, x + width - 1U, y + height - 1U);
    for (i = 0U; i < total; i++) {
        LCD_WR_DATA8(pic[i * 2U]);
        LCD_WR_DATA8(pic[(i * 2U) + 1U]);
    }
}

void LCD_DebugUpdate(Car_t Car)
{
    char line[17];

    if ((Car.task == NULL) || (Car.Motors == NULL) || (Car.GraySensor_t == NULL)) {
        return;
    }
    if ((Car.Motors->EncoderLeft == NULL) || (Car.Motors->EncoderRight == NULL) ||
        (Car.Motors->MotorLeft == NULL) || (Car.Motors->MotorRight == NULL)) {
        return;
    }

    LCD_ShowString(0U, 0U,  (const uint8_t *)"Gray:", LCD_WHITE, LCD_BLACK, 16U, 0U);
    LCD_ShowString(0U, 16U, (const uint8_t *)"Yaw :", LCD_WHITE, LCD_BLACK, 16U, 0U);
    LCD_ShowString(0U, 32U, (const uint8_t *)"Task:", LCD_WHITE, LCD_BLACK, 16U, 0U);
    LCD_ShowString(0U, 48U, (const uint8_t *)"Dyaw:", LCD_WHITE, LCD_BLACK, 16U, 0U);
    LCD_ShowString(0U, 64U, (const uint8_t *)"VL  :", LCD_WHITE, LCD_BLACK, 16U, 0U);
    LCD_ShowString(0U, 80U, (const uint8_t *)"VR  :", LCD_WHITE, LCD_BLACK, 16U, 0U);
    LCD_ShowString(0U, 96U, (const uint8_t *)"PWM :", LCD_WHITE, LCD_BLACK, 16U, 0U);

    (void)snprintf(line, sizeof(line), "0x%02X          ",
                   (unsigned int)Car.GraySensor_t->BinaryData);
    LCD_ShowString(48U, 0U, (const uint8_t *)line, LCD_CYAN, LCD_BLACK, 16U, 0U);

    (void)snprintf(line, sizeof(line), "%+06d       ",
                   (int)(IMU_Attitude.YawTotalAngle * 57.29578f));
    LCD_ShowString(48U, 16U, (const uint8_t *)line, LCD_YELLOW, LCD_BLACK, 16U, 0U);

    (void)snprintf(line, sizeof(line), "T%d S%d L%d    ",
                   Car.task->TaskFlag,
                   Car.task->TaskState,
                   Car.task->LapCount);
    LCD_ShowString(48U, 32U, (const uint8_t *)line, LCD_GREEN, LCD_BLACK, 16U, 0U);

    (void)snprintf(line, sizeof(line), "%+06d       ",
                   (int)(Car.task->Deltayaw * 57.29578f));
    LCD_ShowString(48U, 48U, (const uint8_t *)line, LCD_YELLOW, LCD_BLACK, 16U, 0U);

    (void)snprintf(line, sizeof(line), "%+06d       ",
                   (int)Car.Motors->EncoderLeft->V);
    LCD_ShowString(48U, 64U, (const uint8_t *)line, LCD_WHITE, LCD_BLACK, 16U, 0U);

    (void)snprintf(line, sizeof(line), "%+06d       ",
                   (int)Car.Motors->EncoderRight->V);
    LCD_ShowString(48U, 80U, (const uint8_t *)line, LCD_WHITE, LCD_BLACK, 16U, 0U);

    (void)snprintf(line, sizeof(line), "%+04d %+04d ",
                   Car.Motors->MotorLeft->Output,
                   Car.Motors->MotorRight->Output);
    LCD_ShowString(48U, 96U, (const uint8_t *)line, LCD_MAGENTA, LCD_BLACK, 16U, 0U);
}
