#ifndef DISKIO_SPI_WRAPPER_H
#define DISKIO_SPI_WRAPPER_H

#include "integer.h"

void SELECT(void);
void DESELECT(void);
void xmit_spi(BYTE Data);
BYTE rcvr_spi(void);
void rcvr_spi_m(BYTE *dst);

#endif //DISKIO_SPI_WRAPPER_H
