#include "diskio-spi-wrapper.h"

void SELECT(void) {
    spi_slave_enable(spi_microSd);
}

void DESELECT(void) {
    spi_slave_disable(spi_microSd);
}

void xmit_spi(BYTE Data) {
    spi_send_receive(spi_microSd, Data);
}

BYTE rcvr_spi(void) {
    unsigned char Dummy, Data;
    Dummy = 0xFF;
    Data = 0;
    Data = spi_send_receive(spi_microSd, Dummy);
    return Data;
}

void rcvr_spi_m(BYTE *dst) {
    *dst = rcvr_spi();
}