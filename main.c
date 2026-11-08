#include "stm32f4xx.h"

int main(void)
{
    // 1. 开启 GPIOF 外设时钟
    RCC->AHB1ENR |= (1U << 5);

    // 2. 配置 PF6 为通用输出模式
    GPIOF->MODER &= ~(3U << 12);
    GPIOF->MODER |=  (1U << 12);

    // 3. PF6 输出低电平，点亮红灯
    GPIOF->BSRR = (1U << 22);
	  GPIOF->BSRR = (1U << 6);

    while (1)
    {
			
    }
}
