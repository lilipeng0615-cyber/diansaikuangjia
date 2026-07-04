#include "spi.h"
#include "headfile.h"

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
