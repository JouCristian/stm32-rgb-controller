#ifndef DIMMER_H
#define DIMMER_H

#include <stdint.h>

/* Disabled at startup. Disabling immediately restores full global brightness. */
void Dimmer_SetEnabled(uint8_t enabled);

/* Call from the main loop; samples every 50 ms while enabled. */
void Dimmer_Update(uint32_t now_ms);

#endif
