#include "spi.h"

uint8_t spi0_transfer_byte(uint8_t data)
{
    while (!DL_SPI_isRXFIFOEmpty(SPI_0_INST)) {
        (void)DL_SPI_receiveData8(SPI_0_INST);
    }

    DL_SPI_transmitDataBlocking8(SPI_0_INST, data);
    return DL_SPI_receiveDataBlocking8(SPI_0_INST);
}

void spi0_wait_idle(void)
{
    while (DL_SPI_isBusy(SPI_0_INST)) {
    }
}

uint8_t spi1_transfer_byte(uint8_t data)
{
    uint8_t received;

    while (!DL_SPI_isRXFIFOEmpty(SPI_1_INST)) {
        (void)DL_SPI_receiveData8(SPI_1_INST);
    }

    DL_SPI_transmitDataBlocking8(SPI_1_INST, data);
    received = DL_SPI_receiveDataBlocking8(SPI_1_INST);

    while (DL_SPI_isBusy(SPI_1_INST)) {
    }

    return received;
}

void spi1_wait_idle(void)
{
    while (DL_SPI_isBusy(SPI_1_INST)) {
    }
}
