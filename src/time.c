#include <avr/io.h>
#include <avr/interrupt.h>
#include <time.h>

#define TIMER0_INTERRUPT_OFF()   (TIMSK &= ~_BV(OCIE0A))
#define TIMER0_INTERRUPT_ON()    (TIMSK |= _BV(OCIE0A))

static uint32_t millisecondsPassed = 0;

void Time_Init(void)
{
    TCCR0A = _BV(WGM01);
    TCCR0B = _BV(CS01) | _BV(CS00);  // prescaler 64
    OCR0A = 124;
    TIMER0_INTERRUPT_ON();
}

ISR(TIMER0_COMPA_vect)
{
    millisecondsPassed++;
}

uint32_t Time_MsPassed(void)
{
    TIMER0_INTERRUPT_OFF();
    uint32_t ms = millisecondsPassed;
    TIMER0_INTERRUPT_ON();
    return ms;
}

void Time_WaitMs(uint32_t ms)
{
    uint32_t now = Time_MsPassed();
    while (Time_MsPassed() - now < ms);
}
