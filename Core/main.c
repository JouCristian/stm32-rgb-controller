#include "stm32f4xx.h"

typedef struct {
    uint8_t r : 1;
    uint8_t g : 1;
    uint8_t b : 1;
}RgbMask;

static volatile RgbMask g_rgb = {1,0,0};

static void led_setColor(RgbMask mask) {
    uint32_t bsrr = 0;
    bsrr |= mask.r ? (1U << (6 + 16)) : (1U << 6);
    bsrr |= mask.g ? (1U << (7 + 16)) : (1U << 7);
    bsrr |= mask.b ? (1U << (8 + 16)) : (1U << 8);
    GPIOF->BSRR = bsrr;
}

int main(void)
{
	// 1. 开启 GPIOF 外设时钟
	RCC->AHB1ENR |= (1U << 5);

	// 2. 配置 PF6 、PF7、PF8为通用输出模式
	GPIOF->MODER &= ~(63U << 12);
	GPIOF->MODER |=  (21U << 12);

    led_setColor(g_rgb);
	while (1) {}
}
