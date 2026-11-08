#include "stm32f4xx.h"

int main(void)
{
	// 1. 开启 GPIOF 外设时钟
	RCC->AHB1ENR |= (1U << 5);

	// 2. 配置 PF6 、PF7、PF8为通用输出模式
	GPIOF->MODER &= ~(63U << 12);
	GPIOF->MODER |=  (21U << 12);

	// 3. PF6、PF7、PF8输出低电平，点亮红灯
	GPIOF->BSRR = (7U << 22);

	while (1) {}
}
