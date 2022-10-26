#ifndef _PLAYBACK_H
#define _PLAYBACK_H

#include <main.h>
#include <tone.h>

void Playback_ResetQueue(void);
void Playback_Enqueue(struct FiniteTone finiteTone);
struct FiniteTone Playback_Dequeue(void);
bool Playback_QueueEmpty(void);
bool Playback_QueueFull(void);

#endif // _PLAYBACK_H