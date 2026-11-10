#include "stm32f4xx.h"
#include "rgb.h"
#include "effects.h"
#include "uart.h"
#include "commands.h"
#include "adc.h"

static volatile uint32_t g_ms = 0;

/* Referenced by the startup vector table. */
void SysTick_Handler(void);

void SysTick_Handler(void) {
    g_ms++;
}

static void SysTick_Init(void) {
    SysTick->LOAD = SystemCoreClock / 1000U - 1U;
    SysTick->VAL = 0U;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}

int main(void)
{
    RGB_Init();
    SysTick_Init();
    UART1_Init();
    ADC1_PotInit();

    Effects_SetMode(EFFECT_RGB_FADE);

    while (1) {
        Effects_Update(g_ms);

        uint8_t ch;
        if(UART1_TryRead(&ch)) {
            Commands_FeedByte(ch);
        }
    }
}
