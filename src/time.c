#include <avr/io.h>
#include <avr/interrupt.h>
#include <time.h>

#define timer0_InterruptOff()   (TIMSK &= ~_BV(OCIE0A))
#define timer0_InterruptOn()    (TIMSK |= _BV(OCIE0A))

static uint32_t millisecondsPassed = 0;

void Time_Init(void)
{
    TCCR0A = _BV(WGM01);
    TCCR0B = _BV(CS01) | _BV(CS00);  // prescaler 64
    OCR0A = 124;
    timer0_InterruptOn();
}

ISR(TIMER0_COMPA_vect)
{
    millisecondsPassed++;
}

uint32_t Time_MsPassed(void)
{
    timer0_InterruptOff();
    uint32_t ms = millisecondsPassed;
    timer0_InterruptOn();
    return ms;
}

void Time_WaitMs(uint32_t ms)
{
    uint32_t now = Time_MsPassed();
    while (Time_MsPassed() - now < ms);
}
