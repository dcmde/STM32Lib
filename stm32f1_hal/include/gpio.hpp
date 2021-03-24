#ifndef STM32F103_GPIO
#define STM32F103_GPIO

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

enum GPIOPin {
    PIN_0 = GPIO_Pin_0,
    PIN_1 = GPIO_Pin_1,
    PIN_2 = GPIO_Pin_2,
    PIN_3 = GPIO_Pin_3,
    PIN_4 = GPIO_Pin_4,
    PIN_5 = GPIO_Pin_5,
    PIN_6 = GPIO_Pin_6,
    PIN_7 = GPIO_Pin_7,
    PIN_8 = GPIO_Pin_8,
    PIN_9 = GPIO_Pin_9,
    PIN_10 = GPIO_Pin_10,
    PIN_11 = GPIO_Pin_11,
    PIN_12 = GPIO_Pin_12,
    PIN_13 = GPIO_Pin_13
};

class Gpio {
public:

    static void setPin(GPIOPin pin, GPIO_TypeDef *port, GPIOMode_TypeDef mode,
                       GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);

    static void setGPIOA(GPIOPin pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);

    static void setGPIOB(GPIOPin pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);

    static void setGPIOC(GPIOPin pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);

    static void pinHigh(GPIOPin pin, GPIO_TypeDef *port);

    static void pinLow(GPIOPin pin, GPIO_TypeDef *port);

    static void tooglePin(GPIOPin pin, GPIO_TypeDef *port);

    static uint8_t readPin(GPIOPin pin, GPIO_TypeDef *port);

};

#endif //STM32F103_GPIO
