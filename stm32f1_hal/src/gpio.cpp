#include "gpio.hpp"

void Gpio::setPin(Pin pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed) {
    GPIO_TypeDef *port;
    GPIOPin gpio_pin;
    getPin(pin, &port, &gpio_pin);
    setPin(gpio_pin, port, mode, speed);
}

void Gpio::pinHigh(Pin pin) {
    GPIO_TypeDef *port;
    GPIOPin gpio_pin;
    getPin(pin, &port, &gpio_pin);
    pinHigh(gpio_pin, port);
}

void Gpio::pinLow(Pin pin) {
    GPIO_TypeDef *port;
    GPIOPin gpio_pin;
    getPin(pin, &port, &gpio_pin);
    pinLow(gpio_pin, port);
}

uint8_t Gpio::readPin(Pin pin) {
    GPIO_TypeDef *port;
    GPIOPin gpio_pin;
    getPin(pin, &port, &gpio_pin);
    return readPin(gpio_pin, port);
}

void Gpio::pinHigh(const GPIOPin pin, GPIO_TypeDef *port) {
    GPIO_SetBits(port, pin);
}

void Gpio::pinLow(const GPIOPin pin, GPIO_TypeDef *port) {
    GPIO_ResetBits(port, pin);
}

uint8_t Gpio::readPin(const GPIOPin pin, GPIO_TypeDef *port) {
    return GPIO_ReadInputDataBit(port, pin);
}

void Gpio::setPin(const GPIOPin pin, GPIO_TypeDef *port, const GPIOMode_TypeDef mode,
                  const GPIOSpeed_TypeDef speed) {
    GPIO_InitTypeDef GPIO_InitStructure;

    if (port == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } else if (port == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    } else if (port == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    }

    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_Speed = speed;
    GPIO_Init(port, &GPIO_InitStructure);
}

void Gpio::getPin(Pin pin, GPIO_TypeDef **port, GPIOPin *gpio_pin) {
    switch (pin) {
        case A0:
            *gpio_pin = PIN_0;
            *port = GPIOA;
            break;
        case A1:
            *gpio_pin = PIN_1;
            *port = GPIOA;
            break;
        case A2:
            *gpio_pin = PIN_2;
            *port = GPIOA;
            break;
        case A3:
            *gpio_pin = PIN_3;
            *port = GPIOA;
            break;
        case A4:
            *gpio_pin = PIN_4;
            *port = GPIOA;
            break;
        case A5:
            *gpio_pin = PIN_5;
            *port = GPIOA;
            break;
        case A6:
            *gpio_pin = PIN_6;
            *port = GPIOA;
            break;
        case A7:
            *gpio_pin = PIN_7;
            *port = GPIOA;
            break;
        case A8:
            *gpio_pin = PIN_8;
            *port = GPIOA;
            break;
        case A9:
            *gpio_pin = PIN_9;
            *port = GPIOA;
            break;
        case A10:
            *gpio_pin = PIN_10;
            *port = GPIOA;
            break;
        case A11:
            *gpio_pin = PIN_11;
            *port = GPIOA;
            break;
        case A12:
            *gpio_pin = PIN_12;
            *port = GPIOA;
            break;
        case A15:
            *gpio_pin = PIN_15;
            *port = GPIOA;
            break;
        case B0:
            *gpio_pin = PIN_0;
            *port = GPIOB;
            break;
        case B1:
            *gpio_pin = PIN_1;
            *port = GPIOB;
            break;
        case B2:
            *gpio_pin = PIN_2;
            *port = GPIOB;
            break;
        case B3:
            *gpio_pin = PIN_3;
            *port = GPIOB;
            break;
        case B4:
            *gpio_pin = PIN_4;
            *port = GPIOB;
            break;
        case B5:
            *gpio_pin = PIN_5;
            *port = GPIOB;
            break;
        case B6:
            *gpio_pin = PIN_6;
            *port = GPIOB;
            break;
        case B7:
            *gpio_pin = PIN_7;
            *port = GPIOB;
            break;
        case B8:
            *gpio_pin = PIN_8;
            *port = GPIOB;
            break;
        case B9:
            *gpio_pin = PIN_9;
            *port = GPIOB;
            break;
        case B10:
            *gpio_pin = PIN_10;
            *port = GPIOB;
            break;
        case B11:
            *gpio_pin = PIN_11;
            *port = GPIOB;
            break;
        case B12:
            *gpio_pin = PIN_12;
            *port = GPIOB;
            break;
        case B13:
            *gpio_pin = PIN_13;
            *port = GPIOB;
            break;
        case B14:
            *gpio_pin = PIN_14;
            *port = GPIOB;
            break;
        case B15:
            *gpio_pin = PIN_15;
            *port = GPIOB;
            break;
        case C13:
            *gpio_pin = PIN_13;
            *port = GPIOC;
            break;
        case C14:
            *gpio_pin = PIN_14;
            *port = GPIOC;
            break;
        case C15:
            *gpio_pin = PIN_15;
            *port = GPIOC;
            break;
    }
}
