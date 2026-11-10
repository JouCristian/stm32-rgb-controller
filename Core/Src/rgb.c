#include "rgb.h"
#include "pwm.h"

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
    PWM_RGB_SetDuty(mask.r ? 1000U : 0U,
                    mask.g ? 1000U : 0U,
                    mask.b ? 1000U : 0U);
}

void RGB_Init(void)
{
    PWM_RGB_Init();
}

void RGB_SetColor(Color color)
{
    if ((unsigned int)color >= COLOR_COUNT) return;
    led_setColor(colors[color]);
}
