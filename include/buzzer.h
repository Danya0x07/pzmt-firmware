#ifndef _BUZZER_H
#define _BUZZER_H

#include <main.h>

void Buzzer_Init(void);
void Buzzer_SetFrequency(uint16_t frequency);
void Buzzer_Off(void);

void Buzzer_SetRaisedVolumeMode(bool raised);
void Buzzer_ToggleVolumeMode(void);
bool Buzzer_VolumeRaised(void);

#endif // _BUZZER_H