#include "diskio-spi-wrapper.h"

void SELECT(void) {
    spi_slave_enable(spi_miroSd);
}

void DESELECT(void) {
    spi_slave_disable(spi_miroSd);
}

void xmit_spi(BYTE Data) {
    spi_send_receive(spi_miroSd, Data);
}

BYTE rcvr_spi(void) {
    unsigned char Dummy;
    Dummy = 0xFF;
    return spi_send_receive(spi_miroSd, Dummy);
}

void rcvr_spi_m(BYTE *dst) {
    *dst = rcvr_spi();
}