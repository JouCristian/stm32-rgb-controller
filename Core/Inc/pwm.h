#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/* Call once via RGB_Init(); initializes all three channels with LEDs off. */
void PWM_RGB_Init(void);

/* Active-low LED duty: 0 = off, 1000 = fully on; larger values clamp to 1000. */
void PWM_RGB_SetDuty(uint16_t r, uint16_t g, uint16_t b);

#endif
