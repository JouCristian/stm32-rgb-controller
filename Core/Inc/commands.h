#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

/* Feed received ASCII bytes from the main loop, after RGB/UART initialization.
 * Lines may end in CR, LF or CRLF; at most 63 bytes before the terminator.
 * Commands are case-sensitive. Invalid/overlong lines are discarded to EOL.
 */
void Commands_FeedByte(uint8_t ch);

#endif
