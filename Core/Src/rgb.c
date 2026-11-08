#include "rgb.h"
#include "stm32f4xx.h"

typedef struct {
    uint8_t r : 1;
    uint8_t g : 1;
    uint8_t b : 1;
    uint8_t : 5;
} RgbMask;

static const RgbMask colors[] = {
    [RED]       = {1,0,0},
    [GREEN]     = {0,1,0},
    [BLUE]      = {0,0,1},
    [YELLOW]    = {1,1,0},
    [PURPLE]    = {1,0,1},
    [CYAN]      = {0,1,1},
    [WHITE]     = {1,1,1}
};

static void led_setColor(RgbMask mask) {
    uint32_t bsrr = 0;
    bsrr |= mask.r ? (1U << (6 + 16)) : (1U << 6);
    bsrr |= mask.g ? (1U << (7 + 16)) : (1U << 7);
    bsrr |= mask.b ? (1U << (8 + 16)) : (1U << 8);
    GPIOF->BSRR = bsrr;
}

void RGB_Init(void)
{
    //Turn on the GPIOF Clock
    RCC->AHB1ENR |= (1U << 5);

    //Set PF6 PF7 PF8 as Output module
    GPIOF->MODER &= ~(63U << 12);
    GPIOF->MODER |=  (21U << 12);
}

void RGB_SetColor(Color color)
{
    led_setColor(colors[color]);
}
