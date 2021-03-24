#ifndef _SPI_H
#define _SPI_H

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_spi(SPI_TypeDef *SPIx, int status);

void spi_slave_disable();

uint8_t spi_send_receive(uint8_t data);

void spi_slave_enable();

#ifdef __cplusplus
}
#endif

#endif //_SPI_H
