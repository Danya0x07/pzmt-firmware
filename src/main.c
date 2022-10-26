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
#include <playback.h>
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
    Led_SetColor(LedColor_GREEN);

    for (;;) {
        if (SerialPort_LineReceived()) {
            char line[16];
            struct Command cmd;

            SerialPort_ReadLine(line);
            SerialPort_Flush();
            Protocol_ParseCommand(line, &cmd);

            struct Reply reply;

            switch (cmd.type) {
                default:
                case CommandType_UNRECOGNIZABLE:
                    Tone_Stop();
                    Indicator_OnWrongCmd();
                    Time_WaitMs(500);
                    reply.code = ReplyCode_WRONG_CMD;
                    Protocol_BuildReply(&reply, line);
                    SerialPort_PrintString(line);
                    break;

                case CommandType_SET_VOLUME:
                    Buzzer_SetRaisedVolumeMode(cmd.params.volumeRaised);
                    reply.code = ReplyCode_OK;
                    Protocol_BuildReply(&reply, line);
                    SerialPort_PrintString(line);
                    break;

                case CommandType_PLAY_FINITE_TONE:
                    if (Playback_QueueFull()) {
                        reply.code = ReplyCode_WRONG_CMD;
                        Protocol_BuildReply(&reply, line);
                        SerialPort_PrintString(line);
                    }
                    else {
                        Playback_Enqueue((struct FiniteTone) {
                            .frequency = cmd.params.frequency, 
                            .duration = cmd.params.duration
                        });
                    }
                    break;
                
                case CommandType_PLAY_INFINITE_TONE:
                    Tone_PlayInfinite(cmd.params.frequency);
                    reply.code = ReplyCode_OK;
                    Protocol_BuildReply(&reply, line);
                    SerialPort_PrintString(line);
                    break;

                case CommandType_STOP_PLAYING:
                    Tone_Stop();
                    reply.code = ReplyCode_OK;
                    Protocol_BuildReply(&reply, line);
                    SerialPort_PrintString(line);
                    break;
            }
        }

        if (Tone_GetRoutine() != ToneRoutine_FINITE && !Playback_QueueEmpty()) {
            struct FiniteTone finiteTone = Playback_Dequeue();
            Tone_PlayFinite(finiteTone);
            if (!Playback_QueueFull()) {
                struct Reply reply;
                char line[4];
                reply.code = ReplyCode_OK;
                Protocol_BuildReply(&reply, line);
                SerialPort_PrintString(line);
            }
        }

        if (Button_PressDetected()) {
            if (Tone_GetRoutine() != ToneRoutine_IDLE) {
                Tone_Stop();
                Playback_ResetQueue();
            } else {
                Buzzer_ToggleVolumeMode();
            }
        }

        Tone_Update();
        Indicator_Update();
    }
}