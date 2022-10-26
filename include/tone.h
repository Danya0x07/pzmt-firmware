#ifndef _TONE_H
#define _TONE_H

#include <main.h>

typedef enum {
    ToneRoutine_IDLE,
    ToneRoutine_FINITE,
    ToneRoutine_INFINITE,

    NUM_OF_TONE_ROUTINES
} ToneRoutine_t;

struct FiniteTone {
    uint16_t frequency;
    uint16_t duration;
};

void Tone_PlayFinite(struct FiniteTone finiteTone);
void Tone_PlayInfinite(uint16_t frequency);
void Tone_Update(void);
void Tone_Stop(void);
ToneRoutine_t Tone_GetRoutine(void);

#endif // _TONE_H