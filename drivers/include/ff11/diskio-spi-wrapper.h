#ifndef DISKIO_SPI_WRAPPER_H
#define DISKIO_SPI_WRAPPER_H

#include "integer.h"
#include <spi.h>

SPI_t *spi_miroSd;

void SELECT(void);

void DESELECT(void);

void xmit_spi(BYTE Data);

BYTE rcvr_spi(void);

void rcvr_spi_m(BYTE *dst);

#endif //DISKIO_SPI_WRAPPER_H
