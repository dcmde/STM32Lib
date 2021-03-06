#ifndef TEMPLATE_SPI_HPP
#define TEMPLATE_SPI_HPP

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>

#include <gpio.hpp>

enum SPIStatus {
    MASTER,
    SLAVE
};

class SPI {
public:

    void set(SPI_TypeDef *SPIx, SPIStatus status);

    void setSelectPin(Pin pin);

    void select(Pin pin);

    void deselect(Pin pin);

    uint8_t send_receive(uint8_t data);

private:
    SPI_TypeDef *SPIx_;
    GPIO_TypeDef *GPIOx_;
    uint16_t NSS_PIN_;
};

#endif //TEMPLATE_SPI_HPP
