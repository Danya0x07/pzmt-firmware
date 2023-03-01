#ifndef _LED_H
#define _LED_H

typedef enum __attribute__((packed)) {
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
LedColor_t Led_GetColor(void);
void Led_Off(void);

#endif // _BOARDCONF_H