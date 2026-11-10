#include "effects.h"
#include "pwm.h"

#define EFFECT_INTERVAL_MS 10U
#define EFFECT_DUTY_MAX    1000
#define EFFECT_DUTY_STEP   10

static EffectMode current_mode = EFFECT_BREATH_RED;
static uint32_t last_ms;
static uint8_t start_pending = 1U;
static int16_t duty;
static int16_t step = EFFECT_DUTY_STEP;
static uint8_t fade_phase;
static uint16_t base_r, base_g, base_b;
static uint16_t global_brightness = 1000U;

static void apply_output(void)
{
    PWM_RGB_SetDuty((uint16_t)((uint32_t)base_r * global_brightness / 1000U),
                    (uint16_t)((uint32_t)base_g * global_brightness / 1000U),
                    (uint16_t)((uint32_t)base_b * global_brightness / 1000U));
}

static void set_base_rgb(uint16_t r, uint16_t g, uint16_t b)
{
    base_r = r > 1000U ? 1000U : r;
    base_g = g > 1000U ? 1000U : g;
    base_b = b > 1000U ? 1000U : b;
    apply_output();
}

void Effects_SetBrightness(uint16_t brightness)
{
    if (brightness > 1000U) brightness = 1000U;
    if (brightness == global_brightness) return;
    global_brightness = brightness;
    apply_output();
}

void Effects_SetMode(EffectMode mode)
{
    if (mode != EFFECT_BREATH_RED && mode != EFFECT_RGB_FADE &&
        mode != EFFECT_STATIC) return;

    current_mode = mode;
    duty = 0;
    step = EFFECT_DUTY_STEP;
    fade_phase = 0U;
    start_pending = 1U;
    set_base_rgb(mode == EFFECT_RGB_FADE ? EFFECT_DUTY_MAX : 0U, 0U, 0U);
}

void Effects_SetStaticRGB(uint16_t r, uint16_t g, uint16_t b)
{
    current_mode = EFFECT_STATIC;
    set_base_rgb(r, g, b);
}

void Effects_Update(uint32_t now_ms)
{
    if (current_mode == EFFECT_STATIC) return;

    if (start_pending) {
        last_ms = now_ms;
        start_pending = 0U;
        return;
    }

    /* Unsigned subtraction also handles the uint32_t tick wrapping to zero. */
    if ((uint32_t)(now_ms - last_ms) < EFFECT_INTERVAL_MS) return;
    last_ms = now_ms;

    if (current_mode == EFFECT_BREATH_RED) {
        duty += step;
        if (duty >= EFFECT_DUTY_MAX) {
            duty = EFFECT_DUTY_MAX;
            step = -EFFECT_DUTY_STEP;
        } else if (duty <= 0) {
            duty = 0;
            step = EFFECT_DUTY_STEP;
        }
        set_base_rgb((uint16_t)duty, 0U, 0U);
        return;
    }

    duty += EFFECT_DUTY_STEP;
    /* Each 1 s segment linearly changes one channel; endpoints are shared. */
    switch (fade_phase) {
    case 0U: /* Red -> yellow: increase green. */
        set_base_rgb(EFFECT_DUTY_MAX, (uint16_t)duty, 0U);
        break;
    case 1U: /* Yellow -> green: decrease red. */
        set_base_rgb((uint16_t)(EFFECT_DUTY_MAX - duty), EFFECT_DUTY_MAX, 0U);
        break;
    case 2U: /* Green -> cyan: increase blue. */
        set_base_rgb(0U, EFFECT_DUTY_MAX, (uint16_t)duty);
        break;
    case 3U: /* Cyan -> blue: decrease green. */
        set_base_rgb(0U, (uint16_t)(EFFECT_DUTY_MAX - duty), EFFECT_DUTY_MAX);
        break;
    case 4U: /* Blue -> purple: increase red. */
        set_base_rgb((uint16_t)duty, 0U, EFFECT_DUTY_MAX);
        break;
    case 5U: /* Purple -> red: decrease blue. */
        set_base_rgb(EFFECT_DUTY_MAX, 0U, (uint16_t)(EFFECT_DUTY_MAX - duty));
        break;
    }

    if (duty >= EFFECT_DUTY_MAX) {
        duty = 0;
        fade_phase = (uint8_t)((fade_phase + 1U) % 6U);
    }
}
