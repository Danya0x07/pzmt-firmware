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
#include <ftq.h>
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
    SerialPort_PrintChar('H');
    Led_SetColor(LedColor_Green);

    for (;;) {
        if (SerialPort_LineReceived()) {
            char line[16];
            struct Request request;

            SerialPort_ReadLine(line);
            SerialPort_Flush();
            Protocol_ParseRequest(line, &request);

            struct Response response;

            switch (request.type) {
                case RequestType_Undefined:
                    Tone_Stop();
                    Indicator_ShowBadRequest();
                    Time_WaitMs(500);
                    response.type = ResponseType_BadRequest;
                    Protocol_BuildResponse(&response, line);
                    SerialPort_PrintString(line);
                    break;

                case RequestType_SetVolume:
                    Buzzer_SetRaisedVolumeMode(request.content.volumeRaised);
                    response.type = ResponseType_Acknowledge;
                    Protocol_BuildResponse(&response, line);
                    SerialPort_PrintString(line);
                    break;

                case RequestType_PlayFiniteTone:
                    if (Ft_QueueFull()) {
                        response.type = ResponseType_BadRequest;
                        Protocol_BuildResponse(&response, line);
                        SerialPort_PrintString(line);
                    }
                    else {
                        Ft_Enqueue((struct FiniteTone) {
                            .frequency = request.content.frequency, 
                            .duration = request.content.duration
                        });
                    }
                    break;
                
                case RequestType_PlayInfiniteTone:
                    Tone_PlayInfinite(request.content.frequency);
                    response.type = ResponseType_Acknowledge;
                    Protocol_BuildResponse(&response, line);
                    SerialPort_PrintString(line);
                    break;

                case RequestType_StopPlaying:
                    Tone_Stop();
                    response.type = ResponseType_Acknowledge;
                    Protocol_BuildResponse(&response, line);
                    SerialPort_PrintString(line);
                    break;
            }
        }

        if (Tone_GetStatus() != ToneStatus_Finite && !Ft_QueueEmpty()) {
            struct FiniteTone finiteTone = Ft_Dequeue();
            Tone_PlayFinite(finiteTone.frequency, finiteTone.duration);
            SerialPort_PrintDecimal(finiteTone.frequency);
            SerialPort_PrintChar('\t');
            SerialPort_PrintDecimal(finiteTone.duration);
            SerialPort_PrintChar('\n');
            if (!Ft_QueueFull()) {
                struct Response response;
                char line[4];
                response.type = ResponseType_Acknowledge;
                Protocol_BuildResponse(&response, line);
                SerialPort_PrintString(line);
            }
        }

        if (Button_PressDetected()) {
            if (Tone_GetStatus() != ToneStatus_Idle) {
                Tone_Stop();
                Ft_ResetQueue();
            } else {
                Buzzer_ToggleVolumeMode();
            }
        }

        Tone_Update();
        Indicator_Update();
    }
}