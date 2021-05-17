#include "spi.h"

static SPI_TypeDef *SPIx_;
static GPIO_TypeDef *GPIOx_;
static uint16_t NSS_PIN_;

void init_spi(SPI_TypeDef *SPIx, int status) {
    uint16_t MOSI_PIN, MISO_PIN, SCK_PIN;
    SPIx_ = SPIx;
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    SPIx_ = SPIx;

    if (SPIx == SPI1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        GPIOx_ = GPIOA;
        MOSI_PIN = GPIO_Pin_7;
        MISO_PIN = GPIO_Pin_6;
        SCK_PIN = GPIO_Pin_5;
        NSS_PIN_ = GPIO_Pin_4;
    } else if (SPIx == SPI2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        GPIOx_ = GPIOB;
        MOSI_PIN = GPIO_Pin_15;
        MISO_PIN = GPIO_Pin_14;
        SCK_PIN = GPIO_Pin_13;
        NSS_PIN_ = GPIO_Pin_12;
    }

    // GPIO pins for MOSI, MISO, and SCK
    GPIO_InitStruct.GPIO_Pin = MOSI_PIN | MISO_PIN | SCK_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx_, &GPIO_InitStruct);

    // GPIO pin for SS
    GPIO_InitStruct.GPIO_Pin = NSS_PIN_;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOx_, &GPIO_InitStruct);

    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

    if (status == 0) {
        SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
        SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    } else {
        SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
        SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
        SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;
    }
    SPI_Init(SPIx, &SPI_InitStruct);
    SPI_Cmd(SPIx, ENABLE);
}

void spi_slave_disable() {
    GPIOx_->BSRR |= NSS_PIN_;
}

uint8_t spi_send_receive(uint8_t data) {
    SPIx_->DR = data;
    // Wait until transmit complete
    while (!(SPIx_->SR & (SPI_I2S_FLAG_TXE)));
    // Wait until receive complete
    while (!(SPIx_->SR & (SPI_I2S_FLAG_RXNE)));
    // Wait until SPI is not busy anymore
    while (SPIx_->SR & (SPI_I2S_FLAG_BSY));
    // Receive data from the register.
    return SPIx_->DR;
}

void spi_slave_enable() {
    GPIOx_->BRR |= NSS_PIN_;
}