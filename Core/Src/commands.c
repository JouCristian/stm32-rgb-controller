#include "commands.h"
#include "effects.h"
#include "uart.h"
#include "adc.h"
#include "dimmer.h"
#include <string.h>

#define COMMAND_BUFFER_SIZE 64U

static char line[COMMAND_BUFFER_SIZE];
static uint8_t length;
static uint8_t discard_line;

static char *next_token(char **cursor)
{
    char *token;

    while (**cursor == ' ' || **cursor == '\t') ++(*cursor);
    if (**cursor == '\0') return 0;

    token = *cursor;
    while (**cursor != '\0' && **cursor != ' ' && **cursor != '\t') ++(*cursor);
    if (**cursor != '\0') *(*cursor)++ = '\0';
    return token;
}

static uint8_t parse_duty(const char *token, uint16_t *value)
{
    uint16_t result = 0U;

    if (token == 0 || *token == '\0') return 0U;
    while (*token != '\0') {
        if (*token < '0' || *token > '9') return 0U;
        result = (uint16_t)(result * 10U + (uint16_t)(*token - '0'));
        /* Checking every digit prevents overflow even for very long numbers. */
        if (result > 1000U) return 0U;
        ++token;
    }
    *value = result;
    return 1U;
}

static void execute_line(void)
{
    char *cursor = line;
    char *command = next_token(&cursor);
    uint8_t ok = 0U;

    if (command == 0) return;

    if (strcmp(command, "MODE") == 0) {
        char *mode = next_token(&cursor);
        if (mode != 0 && next_token(&cursor) == 0) {
            if (strcmp(mode, "BREATH") == 0) {
                Effects_SetMode(EFFECT_BREATH_RED);
                ok = 1U;
            } else if (strcmp(mode, "FADE") == 0) {
                Effects_SetMode(EFFECT_RGB_FADE);
                ok = 1U;
            }
        }
    } else if (strcmp(command, "RGB") == 0) {
        uint16_t r, g, b;
        if (parse_duty(next_token(&cursor), &r) &&
            parse_duty(next_token(&cursor), &g) &&
            parse_duty(next_token(&cursor), &b) && next_token(&cursor) == 0) {
            Effects_SetStaticRGB(r, g, b);
            ok = 1U;
        }
    } else if (strcmp(command, "OFF") == 0) {
        if (next_token(&cursor) == 0) {
            Effects_SetStaticRGB(0U, 0U, 0U);
            ok = 1U;
        }
    } else if (strcmp(command, "POT") == 0) {
        char *state = next_token(&cursor);
        if (state != 0 && next_token(&cursor) == 0) {
            if (strcmp(state, "ON") == 0) {
                Dimmer_SetEnabled(1U);
                ok = 1U;
            } else if (strcmp(state, "OFF") == 0) {
                Dimmer_SetEnabled(0U);
                ok = 1U;
            }
        }
    } else if (strcmp(command, "ADC") == 0) {
        uint16_t raw;
        if (next_token(&cursor) == 0 && ADC1_PotRead(&raw)) {
            char response[] = "ADC=0000\r\n";
            response[4] = (char)('0' + raw / 1000U);
            response[5] = (char)('0' + (raw / 100U) % 10U);
            response[6] = (char)('0' + (raw / 10U) % 10U);
            response[7] = (char)('0' + raw % 10U);
            UART1_SendString(response);
            ok = 1U;
        }
    } else if (strcmp(command, "HELP") == 0) {
        if (next_token(&cursor) == 0) {
            UART1_SendString("MODE BREATH\r\nMODE FADE\r\n"
                             "RGB r g b (0-1000)\r\nOFF\r\n"
                             "POT ON\r\nPOT OFF\r\nADC\r\nHELP\r\n");
            ok = 1U;
        }
    }

    UART1_SendString(ok ? "OK\r\n" : "ERR\r\n");
}

void Commands_FeedByte(uint8_t ch)
{
    if (ch == '\r' || ch == '\n') {
        if (discard_line) {
            UART1_SendString("ERR\r\n");
        } else if (length != 0U) {
            line[length] = '\0';
            execute_line();
        }
        length = 0U;
        discard_line = 0U;
        return;
    }

    if (discard_line) return;
    if ((ch != '\t' && (ch < 32U || ch > 126U)) ||
        length >= COMMAND_BUFFER_SIZE - 1U) {
        discard_line = 1U;
        return;
    }
    line[length++] = (char)ch;
}
