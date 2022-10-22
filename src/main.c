#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <led.h>
#include <button.h>
#include <buzzer.h>
#include <time.h>
#include <serialport.h>

int main(void)
{
    Time_Init();
    Led_Init();
    Button_Init();
    Buzzer_Init();
    SerialPort_Init();
    uint16_t frequency = 0, duration = 0;

    sei();
    SerialPort_PrintString("Hellow, Sir!\n");

    for (;;) {
        if (SerialPort_LineReceived()) {
            char line[16];
            SerialPort_ReadLine(line);
            SerialPort_PrintString(line);
            char *p = strtok(line, ",");
            if (p) {
                frequency = atoi(p);
                p = strtok(NULL, ",");
                if (p)
                    duration = atoi(p);
            }
            if (duration == 0) {
                Buzzer_Off();
                Led_SetColor(LedColor_None);
            } else if (frequency == 0) {
                Buzzer_Off();
                Time_WaitMs(duration);
                Led_SetColor(LedColor_Red);
            } else {
                Led_SetColor(LedColor_Blue);
                Buzzer_SetFrequency(frequency);
                Time_WaitMs(duration);
                Buzzer_Off();
                Led_SetColor(LedColor_None);
            }
        }
        if (Button_PressDetected()) {
            SerialPort_PrintString("Hellow, Sir!\n");
            if (Button_PressDetectedAgain()) {
                Buzzer_SetFrequency(300);
                Time_WaitMs(1000);
            } else {
                if (Buzzer_TurboEnabled()) {
                    Led_SetColor(LedColor_Yellow);
                    Buzzer_DisableTurbo();
                } else {
                    Led_SetColor(LedColor_Purple);
                    Buzzer_EnableTurbo();
                }
            }
        }
    }
}