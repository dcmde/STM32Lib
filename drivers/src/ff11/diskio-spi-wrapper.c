#include <spi.h>
#include "diskio-spi-wrapper.h"

void SELECT(void) {
    spi_slave_enable();
}

void DESELECT(void) {
    spi_slave_disable();
}

void xmit_spi(BYTE Data) {
    spi_send_receive(Data);
}

BYTE rcvr_spi(void) {
    unsigned char Dummy, Data;
    Dummy = 0xFF;
    Data = 0;
    Data = spi_send_receive(Dummy);
    return Data;
}

void rcvr_spi_m(BYTE *dst) {
    *dst = rcvr_spi();
}