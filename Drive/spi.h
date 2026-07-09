#ifndef __SPI_H
#define __SPI_H

#include "databyte.h"
#include "headfile.h"

uint8_t spi0_transfer_byte(uint8_t data);
void spi0_wait_idle(void);
uint8_t spi1_transfer_byte(uint8_t data);
void spi1_wait_idle(void);

#endif

