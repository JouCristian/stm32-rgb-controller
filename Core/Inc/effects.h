#ifndef EFFECTS_H
#define EFFECTS_H

#include <stdint.h>

typedef enum {
    EFFECT_BREATH_RED,
    EFFECT_RGB_FADE,
    EFFECT_STATIC
} EffectMode;

/* Call after RGB_Init(), from the main loop context. Restarts the effect. */
void Effects_SetMode(EffectMode mode);

/* Select static mode and store base RGB duties, each clamped to 0..1000. */
void Effects_SetStaticRGB(uint16_t r, uint16_t g, uint16_t b);

/* Clamp global brightness to 0..1000 and immediately reapply the current RGB. */
void Effects_SetBrightness(uint16_t brightness);

/* Call frequently with the 1 ms tick. First call starts the timing interval. */
void Effects_Update(uint32_t now_ms);

#endif
