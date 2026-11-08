#include "stm32f4xx.h"
#include "rgb.h"

static void delay_rough(void) {
    for(uint32_t i = 0U; i < 5000000U; i++) __NOP();
}

int main(void)
{
    RGB_Init();

    while (1) {
        for(int i = 0; i < COLOR_COUNT; i++) {
            RGB_SetColor((Color)i);
            delay_rough();
        }
    }
}
