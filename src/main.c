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
#include <indicator.h>

int main(void)
{
    Time_Init();
    Led_Init();
    Button_Init();
    Buzzer_Init();
    SerialPort_Init();

    sei();
    Time_WaitMs(500);
    SerialPort_PrintString("Hellow, Sir!\n");
    Led_SetColor(LedColor_Green);

    for (;;) {
        if (SerialPort_LineReceived()) {
            char line[16];
            struct Request request;
            struct Response response;

            SerialPort_ReadLine(line);
            SerialPort_Flush();
            Protocol_ParseRequest(line, &request);
            
            if (request.type == RequestType_Undefined) {
                response.type = ResponseType_BadRequest;
            } else {
                response.type = ResponseType_Acknowledge;
            }
            Protocol_BuildResponse(&response, line);
            SerialPort_PrintString(line);

            switch (request.type) {
                case RequestType_Undefined:
                    Tone_Stop();
                    Indicator_ShowBadRequest();
                    Time_WaitMs(500);
                    break;

                case RequestType_SetVolume:
                    Buzzer_SetRaisedVolumeMode(request.content.volumeRaised);
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
            if (Tone_GetStatus()) {
                Tone_Stop();
            } else {
                Buzzer_ToggleVolumeMode();
            }
        }

        Tone_Update();
        Indicator_Update();
    }
}