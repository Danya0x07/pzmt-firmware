#include <boardconf.h>
#include <button.h>
#include <time.h>

#define DOUBLEPRESS_CYCLES  350

void Button_Init(void)
{
    BUTTON_DDR &= ~BUTTON_BIT;
    MCUCR = _BV(ISC01);
    GIMSK = 0;
}

bool Button_IsPressed(void)
{
    return (BUTTON_PIN & BUTTON_BIT) == 0;
}

bool Button_PressDetected(void)
{
    static bool lastState = false;
    bool wasPress = false;
    bool state = Button_IsPressed();

    if (lastState != state) {
        Time_WaitMs(5);
        state = Button_IsPressed();
    }
    wasPress = !lastState && state;
    lastState = state;
    return wasPress;
}

bool Button_PressDetectedAgain(void)
{
    for (uint16_t cycle = 0; cycle < DOUBLEPRESS_CYCLES; cycle++) {
        Time_WaitMs(1);
        if (Button_PressDetected())
            return true;
    }
    return false;
}


void Button_EnableInterrupt(void)
{
    GIMSK |= _BV(INT0);
}

void Button_DisableInterrupt(void)
{
    GIMSK &= ~_BV(INT0);
}
