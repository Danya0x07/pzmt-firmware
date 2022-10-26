#include <boardconf.h>
#include <led.h>

static LedColor_t currentColor = LedColor_NONE;

void Led_Init(void)
{
    LED_DDR |= LED_BIT_R | LED_BIT_G | LED_BIT_B;
    LED_PORT &= ~(LED_BIT_R | LED_BIT_G | LED_BIT_B);
}

void Led_SetColor(LedColor_t color)
{
    LED_PORT = 
        (LED_PORT & ~(LED_BIT_R | LED_BIT_G | LED_BIT_B))
        | color;
    currentColor = color;
}

LedColor_t Led_GetColor(void)
{
    return currentColor;
}

void Led_Off(void)
{
    Led_SetColor(LedColor_NONE);
}