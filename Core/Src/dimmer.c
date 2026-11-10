#include "dimmer.h"
#include "adc.h"
#include "effects.h"

#define DIMMER_INTERVAL_MS 50U
#define DIMMER_SAMPLE_COUNT 4U

static uint8_t pot_enabled;
static uint8_t start_pending;
static uint32_t last_ms;
static uint16_t samples[DIMMER_SAMPLE_COUNT];
static uint32_t sample_sum;
static uint8_t sample_count;
static uint8_t next_sample;

void Dimmer_SetEnabled(uint8_t enabled)
{
    if (enabled) {
        if (!pot_enabled) {
            start_pending = 1U;
            sample_sum = 0U;
            sample_count = 0U;
            next_sample = 0U;
        }
        pot_enabled = 1U;
    } else {
        pot_enabled = 0U;
        Effects_SetBrightness(1000U);
    }
}

void Dimmer_Update(uint32_t now_ms)
{
    uint16_t raw;

    if (!pot_enabled) return;
    if (start_pending) {
        last_ms = now_ms;
        start_pending = 0U;
        return;
    }
    if ((uint32_t)(now_ms - last_ms) < DIMMER_INTERVAL_MS) return;
    last_ms = now_ms;
    if (!ADC1_PotRead(&raw)) return;

    /* Average up to four valid samples; failures leave the entire filter intact. */
    if (sample_count == DIMMER_SAMPLE_COUNT) {
        sample_sum -= samples[next_sample];
    } else {
        ++sample_count;
    }
    samples[next_sample] = raw;
    sample_sum += raw;
    next_sample = (uint8_t)((next_sample + 1U) % DIMMER_SAMPLE_COUNT);

    Effects_SetBrightness((uint16_t)(sample_sum * 1000U / (4095U * sample_count)));
}
