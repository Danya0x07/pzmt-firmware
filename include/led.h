#ifndef _LED_H
#define _LED_H

typedef enum {
    LedColor_NONE = 0, 
    LedColor_BLUE, 
    LedColor_GREEN, 
    LedColor_CYAN, 
    LedColor_RED, 
    LedColor_PURPLE, 
    LedColor_YELLOW, 
    LedColor_WHITE,

    NUM_OF_LED_COLORS
} LedColor_t;

void Led_Init(void);
void Led_SetColor(LedColor_t color);
void Led_Off(void);

#endif // _BOARDCONF_H