#ifndef _TIME_H
#define _TIME_H

#include <main.h>

void Time_Init(void);
uint32_t Time_MsPassed(void);
void Time_WaitMs(uint32_t ms);

#endif // _TIME_H