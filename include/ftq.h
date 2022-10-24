#ifndef _FTQ_H
#define _FTQ_H

#include <main.h>

struct FiniteTone {
    uint16_t frequency;
    uint16_t duration;
};

void Ft_ResetQueue(void);
void Ft_Enqueue(struct FiniteTone finiteTone);
struct FiniteTone Ft_Dequeue(void);
bool Ft_QueueEmpty(void);
bool Ft_QueueFull(void);

#endif // _FTQ_H