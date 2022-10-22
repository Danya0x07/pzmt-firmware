#ifndef _BUZZER_H
#define _BUZZER_H

#include <main.h>

void Buzzer_Init(void);
void Buzzer_SetFrequency(uint16_t frequency);
void Buzzer_Off(void);

void Buzzer_EnableTurbo(void);
void Buzzer_DisableTurbo(void);
bool Buzzer_TurboEnabled(void);

#endif // _BUZZER_H