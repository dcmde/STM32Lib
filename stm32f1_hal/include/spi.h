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

void SPI_Initialization(SPI_t *spiStruct);

void SPI_InitPin(SPI_t *spiStruct);

void SPI_SlaveEnable(SPI_t *spiStruct);

void SPI_SlaveDisable(SPI_t *spiStruct);

uint8_t SPI_SendReceive(SPI_t *spiStruct, uint8_t data);

void SPI_BufferSendReceive(SPI_t *spiStruct, uint8_t *buffer, uint8_t num_bytes_read);

#ifdef __cplusplus
}
#endif

#endif //_SPI_H
