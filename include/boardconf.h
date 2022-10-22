#ifndef _BOARDCONF_H
#define _BOARDCONF_H

#include <avr/io.h>

#ifndef F_CPU
#   define F_CPU    8000000L  // 8 МГц от внутреннего генератора
#endif

#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_BIT_R   _BV(PB2)
#define LED_BIT_G   _BV(PB1)
#define LED_BIT_B   _BV(PB0)

#define BUZZER_DDR  DDRB
#define BUZZER_PORT PORTB
#define BUZZER_BIT_P    _BV(PB3)
#define BUZZER_BIT_N    _BV(PB4)

#define BUTTON_DDR  DDRD
#define BUTTON_PORT PORTD
#define BUTTON_PIN  PIND
#define BUTTON_BIT  _BV(PD2)

#endif // _BOARDCONF_H