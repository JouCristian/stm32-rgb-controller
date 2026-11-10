#ifndef EFFECTS_H
#define EFFECTS_H

#include <stdint.h>

typedef enum {
    EFFECT_BREATH_RED,
    EFFECT_RGB_FADE
} EffectMode;

/* Call after RGB_Init(), from the main loop context. Restarts the effect. */
void Effects_SetMode(EffectMode mode);

/* Call frequently with the 1 ms tick. First call starts the timing interval. */
void Effects_Update(uint32_t now_ms);

#endif
