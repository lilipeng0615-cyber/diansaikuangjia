#ifndef __LCD_ST7735_H
#define __LCD_ST7735_H

#include "databyte.h"

#define LCD_ST7735_ROTATION 1U

#if (LCD_ST7735_ROTATION == 0U) || (LCD_ST7735_ROTATION == 1U)
#define LCD_W 128U
#define LCD_H 160U
#else
#define LCD_W 160U
#define LCD_H 128U
#endif

#define LCD_WHITE      0xFFFFU
#define LCD_BLACK      0x0000U
#define LCD_BLUE       0x001FU
#define LCD_RED        0xF800U
#define LCD_GREEN      0x07E0U
#define LCD_CYAN       0x07FFU
#define LCD_MAGENTA    0xF81FU
#define LCD_YELLOW     0xFFE0U
#define LCD_GRAY       0x8430U
#define LCD_LIGHTGRAY  0xC618U

#define WHITE          LCD_WHITE
#define BLACK          LCD_BLACK
#define BLUE           LCD_BLUE
#define RED            LCD_RED
#define GREEN          LCD_GREEN
#define CYAN           LCD_CYAN
#define MAGENTA        LCD_MAGENTA
#define YELLOW         LCD_YELLOW
#define GRAY           LCD_GRAY
#define LGRAY          LCD_LIGHTGRAY
#define BRED           0xF81FU
#define GRED           0xFFE0U
#define GBLUE          0x07FFU
#define BROWN          0xBC40U
#define BRRED          0xFC07U
#define DARKBLUE       0x01CFU
#define LIGHTBLUE      0x7D7CU
#define GRAYBLUE       0x5458U
#define LIGHTGREEN     0x841FU
#define LGRAYBLUE      0xA651U
#define LBBLUE         0x2B12U

void LCD_Init(void);
void LCD_SetBacklight(uint8_t on);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_WR_REG(uint8_t reg);
void LCD_WR_DATA8(uint8_t data);
void LCD_WR_DATA(uint16_t data);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void LCD_Clear(uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
uint32_t mypow(uint8_t m, uint8_t n);
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t pic[]);
void LCD_DebugUpdate(Car_t Car);

#endif
