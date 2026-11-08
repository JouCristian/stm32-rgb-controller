#include "stm32f4xx.h"

#define COLOR_COUNT 7

typedef struct {
    uint8_t r : 1;
    uint8_t g : 1;
    uint8_t b : 1;
}RgbMask;

typedef enum {
    RED, GREEN, BLUE,
    YELLOW, PURPLE, CYAN, WHITE,
} Color;

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

static void delay_rough(void) {
    for(uint32_t i = 0U; i < 5000000U; i++) __NOP();
}

int main(void)
{
	// 1. 开启 GPIOF 外设时钟
	RCC->AHB1ENR |= (1U << 5);

	// 2. 配置 PF6 、PF7、PF8为通用输出模式
	GPIOF->MODER &= ~(63U << 12);
	GPIOF->MODER |=  (21U << 12);

	while (1) {
        for(int i = 0; i < COLOR_COUNT; i++) {
            led_setColor(colors[i]);
            delay_rough();
        }
    }
}
