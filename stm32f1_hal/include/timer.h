#ifndef STM32F103_TIMER_HPP
#define STM32F103_TIMER_HPP

#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    Div_1 = TIM_CKD_DIV1,
    Div_2 = TIM_CKD_DIV2,
    Div_4 = TIM_CKD_DIV4
} ClockDivision;

typedef enum {
    Up = TIM_CounterMode_Up,
    Down = TIM_CounterMode_Down,
    Center_1 = TIM_CounterMode_CenterAligned1,
    Center_2 = TIM_CounterMode_CenterAligned2,
    Center_3 = TIM_CounterMode_CenterAligned3
} CounterMode;

typedef enum {
    Timing = TIM_OCMode_Timing,
    Active = TIM_OCMode_Active,
    Inactive = TIM_OCMode_Inactive,
    Toggle = TIM_OCMode_Toggle,
    PWM_1 = TIM_OCMode_PWM1,
    PWM_2 = TIM_OCMode_PWM2
} OCMode;

typedef enum {
    High = TIM_OCPolarity_High,
    Low = TIM_OCPolarity_Low
} OCPolarity;

typedef enum OutputState {
    Enable = TIM_OutputState_Enable,
    Disable = TIM_OutputState_Disable
} OutputState;

typedef enum {
    Channel_1,
    Channel_2,
    Channel_3,
    Channel_4
} OCChannel;

typedef enum {
    Update = TIM_IT_Update,
    CC1 = TIM_IT_CC1,
    CC2 = TIM_IT_CC2,
    CC3 = TIM_IT_CC3,
    CC4 = TIM_IT_CC4,
    COM = TIM_IT_COM,
    Trigger = TIM_IT_Trigger,
    Break = TIM_IT_Break
} InterruptMode;


void timer_init(TIM_TypeDef *TIMx, ClockDivision div, uint16_t prescaler, uint16_t period, CounterMode mode);

void timer_setPWM(TIM_TypeDef *TIMx, OCMode oc_mode, OCChannel channel, uint16_t pulse, OCPolarity oc_polarity);

void timer_setTIM_PWM(TIM_TypeDef *TIMx, ClockDivision div, uint16_t prescaler, OCChannel channel, uint16_t period,
                      uint16_t pulse);

void timer_setTIMPulse(TIM_TypeDef *TIMx, uint16_t pulse);

void timer_setTIMPeriod(TIM_TypeDef *TIMx, uint16_t pulse);

void timer_setTimerInterrupt(TIM_TypeDef *TIMx, enum IRQn irq, InterruptMode itMode);

#ifdef __cplusplus
}
#endif

#endif //STM32F103_TIMER_HPP