#include "diskio-spi-wrapper.h"

void SELECT(void) {
    SPI_SlaveEnable(spi_miroSd);
}

void DESELECT(void) {
    SPI_SlaveDisable(spi_miroSd);
}

void xmit_spi(BYTE Data) {
    SPI_SendReceive(spi_miroSd, Data);
}

BYTE rcvr_spi(void) {
    unsigned char Dummy;
    Dummy = 0xFF;
    return SPI_SendReceive(spi_miroSd, Dummy);
}

void rcvr_spi_m(BYTE *dst) {
    *dst = rcvr_spi();
}