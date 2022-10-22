#ifndef _LED_H
#define _LED_H

enum LedColor {
    LedColor_None = 0, 
    LedColor_Blue, 
    LedColor_Green, 
    LedColor_Cyan, 
    LedColor_Red, 
    LedColor_Purple, 
    LedColor_Yellow, 
    LedColor_White,

    NumOfLedColors
};

void Led_Init(void);
void Led_SetColor(enum LedColor color);

#endif // _BOARDCONF_H