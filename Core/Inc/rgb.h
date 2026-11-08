#ifndef RGB_H
#define RGB_H

typedef enum {
    RED, GREEN, BLUE,
    YELLOW, PURPLE, CYAN, WHITE,
    COLOR_COUNT
} Color;

void RGB_Init(void);
void RGB_SetColor(Color color);

#endif
