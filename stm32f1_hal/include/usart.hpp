#ifndef TEMPLATE_USART_HPP
#define TEMPLATE_USART_HPP

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

class USART {

public:

    static void configure(USART_TypeDef *usart, uint32_t speed);

    static void send(USART_TypeDef *usart, const uint16_t *data);

    static void send(USART_TypeDef *usart, uint16_t data);

    static uint16_t receive(USART_TypeDef *usart);

};

#endif //TEMPLATE_USART_HPP
