#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* PB0 / ADC1_IN8, single software-triggered 12-bit conversion. */
void ADC1_PotInit(void);

/* Returns 1 on success (0..4095), 0 on failure; leaves *raw unchanged on failure.
 * Call from the main loop after ADC1_PotInit(). EOC polling is bounded.
 */
uint8_t ADC1_PotRead(uint16_t *raw);

#endif
