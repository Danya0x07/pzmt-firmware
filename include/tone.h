#ifndef _TONE_H
#define _TONE_H

#include <main.h>

typedef enum ToneStatus {
    ToneStatus_Idle,
    ToneStatus_Finite,
    ToneStatus_Infinite,

    NumOfToneStatuses
} ToneStatus_t;

void Tone_PlayFinite(uint16_t frequency, uint16_t duration);
void Tone_PlayInfinite(uint16_t frequency);
void Tone_Update(void);
void Tone_Stop(void);
ToneStatus_t Tone_GetStatus(void);

#endif // _TONE_H