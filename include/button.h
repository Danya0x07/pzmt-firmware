#ifndef _BUTTON_H
#define _BUTTON_H

#include "main.h"

void Button_Init(void);
bool Button_IsPressed(void);
bool Button_PressDetected(void);
bool Button_PressDetectedAgain(void);

void Button_EnableInterrupt(void);
void Button_DisableInterrupt(void);

#endif // _BUTTON_H