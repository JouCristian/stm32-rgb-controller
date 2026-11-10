#include "stm32f4xx.h"
#include "rgb.h"
#include "effects.h"

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

    /* Select EFFECT_RGB_FADE here to test the six-segment color cycle. */
    Effects_SetMode(EFFECT_RGB_FADE);

    while (1) {
        Effects_Update(g_ms);
    }
}
