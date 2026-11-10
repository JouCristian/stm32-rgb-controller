#include "stm32f4xx.h"
#include "pwm.h"

/* Retain the board-tested TIM10 setup for all three single-channel timers. */
static void pwm_channel_init(TIM_TypeDef *tim)
{
    tim->CR1 &= ~TIM_CR1_CEN;
    tim->CCER &= ~TIM_CCER_CC1E;

    /* Default HSI/APB clocks: 16 MHz / (15 + 1) / (999 + 1) = 1 kHz. */
    tim->PSC = 15U;
    tim->ARR = 999U;
    tim->CCR1 = 0U;

    /* Output channel 1, PWM mode 1, compare and auto-reload preload. */
    tim->CCMR1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_OC1M);
    tim->CCMR1 |= (6U << 4) | TIM_CCMR1_OC1PE;
    tim->CR1 |= TIM_CR1_ARPE;

    /* Invert PWM1: CNT < CCR1 drives low, so duty directly means brightness. */
    tim->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
    tim->CCER |= TIM_CCER_CC1P;

    tim->CNT = 0U;
    tim->EGR = TIM_EGR_UG;
    tim->SR = 0U;
    tim->CCER |= TIM_CCER_CC1E;
    tim->CR1 |= TIM_CR1_CEN;
}

void PWM_RGB_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM10EN | RCC_APB2ENR_TIM11EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
    /* Read back each bus enable before accessing its peripherals. */
    (void)RCC->AHB1ENR;
    (void)RCC->APB2ENR;
    (void)RCC->APB1ENR;

    pwm_channel_init(TIM10);
    pwm_channel_init(TIM11);
    pwm_channel_init(TIM13);

    /* PF6/PF7: AF3 TIM10_CH1/TIM11_CH1; PF8: AF9 TIM13_CH1. */
    GPIOF->AFR[0] &= ~((15U << 24) | (15U << 28));
    GPIOF->AFR[0] |= (3U << 24) | (3U << 28);
    GPIOF->AFR[1] &= ~(15U << 0);
    GPIOF->AFR[1] |= (9U << 0);
    GPIOF->OTYPER &= ~((1U << 6) | (1U << 7) | (1U << 8));
    GPIOF->PUPDR &= ~(63U << 12);
    GPIOF->OSPEEDR &= ~(63U << 12);

    /* Attach pins only after the timers are producing the off (high) level. */
    GPIOF->MODER &= ~(63U << 12);
    GPIOF->MODER |= (42U << 12);
}

void PWM_RGB_SetDuty(uint16_t r, uint16_t g, uint16_t b)
{
    TIM10->CCR1 = (r > 1000U) ? 1000U : r;
    TIM11->CCR1 = (g > 1000U) ? 1000U : g;
    TIM13->CCR1 = (b > 1000U) ? 1000U : b;
}
