#include "stm32f4xx.h"
#include "adc.h"

#define ADC_POLL_LIMIT 10000U

static void adc_enable(void)
{
    ADC1->CR2 |= ADC_CR2_ADON;
    /* tSTAB >= 3 us: 64 NOPs alone take 4 us at the current 16 MHz HCLK. */
    for (uint32_t i = 0U; i < 64U; ++i) __NOP();
}

void ADC1_PotInit(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    (void)RCC->AHB1ENR;
    (void)RCC->APB2ENR;

    /* PB0 analog mode, no pull-up/down; preserve all other GPIOB pins. */
    GPIOB->MODER |= (3U << 0);
    GPIOB->PUPDR &= ~(3U << 0);

    ADC1->CR2 = 0U;
    /* Independent mode, PCLK2 / 4 = 4 MHz with the current default clocks. */
    ADC->CCR &= ~(ADC_CCR_MULTI | ADC_CCR_ADCPRE);
    ADC->CCR |= ADC_CCR_ADCPRE_0;
    ADC1->CR1 = 0U;          /* 12-bit, no scan or interrupts. */
    /* CR2 = 0 selects right alignment, single conversion and software trigger. */
    ADC1->SMPR2 |= ADC_SMPR2_SMP8; /* Channel 8: 480-cycle acquisition time. */
    ADC1->SQR1 = 0U;         /* Regular sequence length = 1. */
    ADC1->SQR2 = 0U;
    ADC1->SQR3 = 8U;         /* First and only channel: ADC1_IN8. */
    ADC1->SR = 0U;
    adc_enable();
}

uint8_t ADC1_PotRead(uint16_t *raw)
{
    uint32_t timeout = ADC_POLL_LIMIT;

    if (raw == 0 || (RCC->APB2ENR & RCC_APB2ENR_ADC1EN) == 0U) return 0U;
    /* Recover from an earlier timeout before starting a fresh conversion. */
    if ((ADC1->CR2 & ADC_CR2_ADON) == 0U) adc_enable();
    ADC1->SR = 0U;
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while ((ADC1->SR & ADC_SR_EOC) == 0U) {
        if (--timeout == 0U) {
            ADC1->CR2 &= ~ADC_CR2_ADON; /* Abort; do not reuse a late result. */
            ADC1->SR = 0U;
            return 0U;
        }
    }

    *raw = (uint16_t)(ADC1->DR & 0x0FFFU); /* Reading DR clears EOC. */
    return 1U;
}
