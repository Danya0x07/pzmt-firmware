#include <boardconf.h>
#include <led.h>

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
}

void Led_Off(void)
{
    Led_SetColor(LedColor_NONE);
}