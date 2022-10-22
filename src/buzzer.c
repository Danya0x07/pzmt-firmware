#include <boardconf.h>
#include <buzzer.h>

static bool turboEnabled = false;

static inline void timer1Start(uint8_t prescaler)
{
    TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(WGM11);
    if (turboEnabled)
        TCCR1A |= _BV(COM1B1);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | (prescaler & 0b111);
}

static inline void timer1Stop(void)
{
    TCCR1A = TCCR1B = 0;
    BUZZER_PORT &= ~(BUZZER_BIT_P | BUZZER_BIT_N);
}

void Buzzer_Init(void)
{
    BUZZER_DDR |= BUZZER_BIT_P | BUZZER_BIT_N;
    BUZZER_PORT &= ~(BUZZER_BIT_P | BUZZER_BIT_N);    
}

void Buzzer_SetFrequency(uint16_t frequency)
{
    /* frequency = F_CPU / (prescaler * (ICR + 1))
     */
    const uint32_t product = F_CPU / frequency;
    const uint16_t prescalerShifts[5] = {0, 3, 6, 8, 10};
    uint32_t regvalue;
    uint8_t cs;

    for (cs = 0; cs < 5; cs++) {
        regvalue = (product >> prescalerShifts[cs]) - 1;
        if (regvalue <= 0xFFFF)
            break;
    }

    timer1Stop();
    ICR1 = (uint16_t)regvalue;
    OCR1A = OCR1B = (uint16_t)regvalue >> 1;
    timer1Start(cs + 1);
}

void Buzzer_Off(void)
{
    timer1Stop();
}

void Buzzer_EnableTurbo(void)
{
    turboEnabled = true;
}

void Buzzer_DisableTurbo(void)
{
    turboEnabled = false;
}

bool Buzzer_TurboEnabled(void)
{
    return turboEnabled;
}
