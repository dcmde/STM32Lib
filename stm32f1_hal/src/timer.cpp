#include <misc.h>
#include "timer.hpp"

void Timer::set(TIM_TypeDef *TIMx, const ClockDivision div, const uint16_t prescaler, const uint16_t period,
                const CounterMode mode) {
    TIM_TimeBaseInitTypeDef TIM_BaseInitStruct;

    if (TIMx == TIM1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    } else if (TIMx == TIM2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    } else if (TIMx == TIM3) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    } else if (TIMx == TIM4) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    }

    TIM_BaseInitStruct.TIM_ClockDivision = div;
    TIM_BaseInitStruct.TIM_Period = period;
    TIM_BaseInitStruct.TIM_CounterMode = mode;
    TIM_BaseInitStruct.TIM_Prescaler = prescaler;

    TIM_TimeBaseInit(TIMx, &TIM_BaseInitStruct);
    TIM_ARRPreloadConfig(TIMx, ENABLE);
    TIM_Cmd(TIMx, ENABLE);
}

void Timer::setPWM(TIM_TypeDef *TIMx, OCMode oc_mode, OCChannel channel, uint16_t pulse, OCPolarity oc_polarity) {
    TIM_OCInitTypeDef OC_InitStruct;

    OC_InitStruct.TIM_OCMode = oc_mode;
    OC_InitStruct.TIM_OCPolarity = oc_polarity;
    OC_InitStruct.TIM_OutputState = TIM_OutputState_Enable;
    OC_InitStruct.TIM_Pulse = pulse;

    switch (channel) {
        case Channel_1:
            TIM_OC1Init(TIMx, &OC_InitStruct);
            TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
            TIM_CtrlPWMOutputs(TIMx, ENABLE);
            break;
        case Channel_2:
            TIM_OC2Init(TIMx, &OC_InitStruct);
//            TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case Channel_3:
            TIM_OC3Init(TIMx, &OC_InitStruct);
//            TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case Channel_4:
            TIM_OC4Init(TIMx, &OC_InitStruct);
//            TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
    }
}

void Timer::setTIM_PWM(TIM_TypeDef *TIMx, ClockDivision div, uint16_t prescaler, OCChannel channel, uint16_t period,
                       uint16_t pulse) {
    set(TIMx, div, prescaler, period, Up);
    setPWM(TIMx, PWM_1, channel, pulse);
}

void Timer::setTIMPulse(TIM_TypeDef *TIMx, uint16_t pulse) {
    TIMx->CCR1 = pulse;
}

void Timer::setTIMPeriod(TIM_TypeDef *TIMx, uint16_t pulse) {
    TIMx->ARR = pulse;
}

void Timer::setTimerInterrupt(TIM_TypeDef *TIMx, IRQn irq, InterruptMode itMode) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_TimeBaseStructure.TIM_Period = 1000;
//    TIM_TimeBaseStructure.TIM_Prescaler = 720;
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIMx, itMode, ENABLE);
//    TIM_Cmd(TIMx, ENABLE);
}
