#include "stm32f4xx.h"
#include "uart.h"

void UART1_Init(void) {
    //Turn on GPIOA's USART1 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    (void)RCC->AHB1ENR;
    (void)RCC->APB2ENR;


    //Set PA9(TX) PA10(RX) as reuse module
    GPIOA->MODER &= ~((3U << 18) | (3U << 20));
    GPIOA->MODER |= ((2U << 18) | (2U << 20));

    //PA9、PA10 choose AF7(USART1)
    GPIOA->AFR[1] &= ~((15U << 4) | (15U << 8));
    GPIOA->AFR[1] |= ((7U << 4) | (7U << 8));

    //CTRL register clear
    USART1->CR1 = 0U;
    USART1->CR2 = 0U;
    USART1->CR3 = 0U;

    //PorterRate
    USART1->BRR = 139U;     //16M/115200(Porter)=139

    //Enable USART
    USART1->CR1 = USART_CR1_UE |
                  USART_CR1_TE |
                  USART_CR1_RE;
}

uint8_t UART1_TryRead(uint8_t *ch) {
    //No data return 
    if((USART1->SR & USART_SR_RXNE) == 0U) return 0U;

    //Received data read data
    *ch = (uint8_t)USART1->DR;
    return 1U;
}
