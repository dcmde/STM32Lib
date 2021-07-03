#include "usart.h"

void uart_configure(USART_TypeDef *usart, uint32_t speed) {
    GPIO_InitTypeDef GPIO_InitStructureTx;
    GPIO_InitTypeDef GPIO_InitStructureRx;
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = speed;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;

    GPIO_InitStructureTx.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructureTx.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructureRx.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructureRx.GPIO_Speed = GPIO_Speed_2MHz;

    if (usart == USART1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
        GPIO_InitStructureTx.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructureRx.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOA, &GPIO_InitStructureTx);
        GPIO_Init(GPIOA, &GPIO_InitStructureRx);
    } else if (usart == USART2) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        GPIO_InitStructureTx.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructureRx.GPIO_Pin = GPIO_Pin_3;
        GPIO_Init(GPIOA, &GPIO_InitStructureTx);
        GPIO_Init(GPIOA, &GPIO_InitStructureRx);
    } else if (usart == USART3) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        GPIO_InitStructureTx.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructureRx.GPIO_Pin = GPIO_Pin_11;
        GPIO_Init(GPIOB, &GPIO_InitStructureTx);
        GPIO_Init(GPIOB, &GPIO_InitStructureRx);
    }

    USART_Init(usart, &USART_InitStructure);
    USART_Cmd(usart, ENABLE);
}

void uart_send(USART_TypeDef *usart, const uint16_t data) {
    while (!(usart->SR & USART_FLAG_TXE));
    usart->DR = data;
}

void uart_send_buffer(USART_TypeDef *usart, const uint16_t *buffer, uint16_t size) {
    for (uint16_t i = 0; i < size; ++i) {
        while (!(usart->SR & USART_FLAG_TXE));
        usart->DR = buffer[i];
    }
}

uint16_t uart_receive(USART_TypeDef *usart) {
    while (!(usart->SR & USART_FLAG_RXNE));
    return usart->DR & (uint16_t) 0x01FF;
}

void uart_receive_buffer(USART_TypeDef *usart, uint16_t *buffer, uint16_t size) {
    for (uint16_t i = 0; i < size; ++i) {
        while (!(usart->SR & USART_FLAG_RXNE));
        buffer[i] = usart->DR & (uint16_t) 0x01FF;
    }
}