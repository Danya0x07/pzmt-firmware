#include <boardconf.h>
#include <led.h>

void Led_Init(void)
{
    LED_DDR |= LED_BIT_R | LED_BIT_G | LED_BIT_B;
    LED_PORT &= ~(LED_BIT_R | LED_BIT_G | LED_BIT_B);
}

void Led_SetColor(enum LedColor color)
{
    LED_PORT = 
        (LED_PORT & ~(LED_BIT_R | LED_BIT_G | LED_BIT_B))
        | color;
}