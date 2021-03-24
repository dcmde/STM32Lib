#include "gpio.hpp"

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

void Gpio::pinHigh(const GPIOPin pin, GPIO_TypeDef *port) {
    GPIO_SetBits(port, pin);
}

void Gpio::pinLow(const GPIOPin pin, GPIO_TypeDef *port) {
    GPIO_ResetBits(port, pin);
}

void Gpio::tooglePin(const GPIOPin pin, GPIO_TypeDef *port) {

}

uint8_t Gpio::readPin(const GPIOPin pin, GPIO_TypeDef *port) {
    return GPIO_ReadInputDataBit(port, pin);
}

void Gpio::setGPIOA(const GPIOPin pin, const GPIOMode_TypeDef mode, const GPIOSpeed_TypeDef speed) {
    Gpio::setPin(pin, GPIOA, mode, speed);
}

void Gpio::setGPIOB(const GPIOPin pin, const GPIOMode_TypeDef mode, const GPIOSpeed_TypeDef speed) {
    Gpio::setPin(pin, GPIOB, mode, speed);
}

void Gpio::setGPIOC(const GPIOPin pin, const GPIOMode_TypeDef mode, const GPIOSpeed_TypeDef speed) {
    Gpio::setPin(pin, GPIOC, mode, speed);
}