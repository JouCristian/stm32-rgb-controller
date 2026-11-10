#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART1_Init(void);
uint8_t UART1_TryRead(uint8_t *ch);

/* Poll TXE to send a NUL-terminated string; no formatted output. */
void UART1_SendString(const char *text);

#endif
