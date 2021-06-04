#ifndef _SPI_H
#define _SPI_H

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLAVE = 0,
    MASTER = 1
} SPI_MODE;

typedef struct {
    SPI_TypeDef *SPIx;
    SPI_MODE mode;
    GPIO_TypeDef *GPIOx;
    uint16_t NSS_PIN;
} SPI_t;

void init_spi(SPI_t *spi);

void spi_slave_enable(SPI_t *spi);

void spi_slave_disable(SPI_t *spi);

uint8_t spi_send_receive(SPI_t *spi, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif //_SPI_H
