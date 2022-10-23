#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <led.h>
#include <button.h>
#include <buzzer.h>
#include <time.h>
#include <serialport.h>

#include <protocol.h>
#include <tone.h>

int main(void)
{
    Time_Init();
    Led_Init();
    Button_Init();
    Buzzer_Init();
    SerialPort_Init();

    sei();
    SerialPort_PrintString("Hellow, Sir!\n");

    for (;;) {
        if (SerialPort_LineReceived()) {
            char line[16];
            struct Request request;
            struct Response response;

            SerialPort_ReadLine(line);
            Protocol_ParseRequest(line, &request);
            
            if (request.type == RequestType_Undefined) {
                response.type = ResponseType_BadRequest;
            } else {
                response.type = ResponseType_Acknowledge;
            }
            Protocol_BuildResponse(&response, line);
            SerialPort_PrintString(line);

            switch (request.type) {
                default: break;

                case RequestType_SetVolume:
                    if (request.content.volumeRaised) {
                        Buzzer_EnableTurbo();
                    } else {
                        Buzzer_DisableTurbo();
                    }
                    break;

                case RequestType_PlayFiniteTone:
                    Tone_PlayFinite(request.content.frequency, request.content.duration);
                    break;
                
                case RequestType_PlayEmptyFiniteTone:
                    Tone_PlayEmpty(request.content.duration);
                    break;
                
                case RequestType_PlayInfiniteTone:
                    Tone_PlayInfinite(request.content.frequency);
                    break;

                case RequestType_StopPlaying:
                    Tone_Stop();
                    break;
            }
            
        }
        if (Button_PressDetected()) {
            if (Tone_IsPlaying()) {
                Tone_Stop();
            } else {
                if (Buzzer_TurboEnabled()) {
                    Buzzer_DisableTurbo();
                } else {
                    Buzzer_EnableTurbo();
                }
            }
        }
    }
}