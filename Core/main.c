#include "stm32f4xx.h"
#include "rgb.h"
#include "pwm.h"

/*static void delay_rough(void) {
    for(uint32_t i = 0U; i < 5000000U; i++) __NOP();
}*/


static volatile uint32_t g_ms = 0;

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

    uint32_t last_ms = 0;
    uint16_t duty = 0;
    uint16_t step = 0;

    PWM_RGB_SetDuty(0,0,0);

    while (1) {
        uint32_t now = g_ms;
        if(now - last_ms >= 10U) {
            last_ms = now;
            duty += step;
            if(duty >= 1000) {
                duty = 1000;
                step = -10;
            }
            else if(duty <= 0) {
                duty = 0;
                step = 10;
            }
            PWM_RGB_SetDuty(duty,0,0);
        }
    }
}
