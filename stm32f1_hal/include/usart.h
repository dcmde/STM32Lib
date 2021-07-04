#ifndef TEMPLATE_USART_H
#define TEMPLATE_USART_H

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

#ifdef __cplusplus
extern "C" {
#endif

void uart_configure(USART_TypeDef *usart, uint32_t speed);

void uart_configure_interrupt(USART_TypeDef *usart, IRQn_Type IRQn, uint16_t USART_IT);

void uart_send(USART_TypeDef *usart, uint16_t data);

void uart_send_buffer(USART_TypeDef *usart, const uint16_t *buffer, uint16_t size);

uint16_t uart_receive(USART_TypeDef *usart);

void uart_receive_buffer(USART_TypeDef *usart, uint16_t *buffer, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif //TEMPLATE_USART_H
