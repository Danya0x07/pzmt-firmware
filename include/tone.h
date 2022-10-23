#ifndef _TONE_H
#define _TONE_H

#include <main.h>

void Tone_PlayFinite(uint16_t frequency, uint16_t duration);
void Tone_PlayInfinite(uint16_t frequency);
void Tone_PlayEmpty(uint16_t duration);
void Tone_Update(void);
void Tone_Stop(void);
bool Tone_IsPlaying(void);

#endif // _TONE_H