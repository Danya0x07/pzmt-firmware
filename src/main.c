#include <stdlib.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <led.h>
#include <button.h>
#include <buzzer.h>
#include <time.h>
#include <serialport.h>

#include <protocol.h>
#include <tone.h>
#include <playback.h>
#include <indicator.h>

#define STARTUP_DELAY   700
#define ERROR_DELAY     1000

static const PROGMEM char STARTUP_MSG1[] =   "(C) MeanderSounds 2022\n";
static const PROGMEM char STARTUP_MSG2[] =   "PzMt Firmware v" FIRMWARE_VERSION "\n";

static void SendStartupMessage(void);
static void SendReply(ReplyCode_t replyCode);
static void ReadCommand(struct Command *cmd);

int main(void)
{
    Time_Init();
    Led_Init();
    Button_Init();
    Buzzer_Init();
    SerialPort_Init();

    sei();
    Time_WaitMs(STARTUP_DELAY);
    SendStartupMessage();
    Indicator_OnStartup();

    for (;;) {
        if (SerialPort_PacketReceived()) {
            struct Command cmd;
            ReadCommand(&cmd);

            switch (cmd.type) {
                default:
                case CommandType_UNRECOGNIZABLE:
                    Tone_Stop();
                    Playback_ResetQueue();
                    SendReply(ReplyCode_WRONG_CMD);
                    Indicator_OnWrongCmd();
                    Time_WaitMs(ERROR_DELAY);
                    break;

                case CommandType_SET_VOLUME:
                    if (Tone_GetRoutine() == ToneRoutine_IDLE) {
                        Buzzer_SetRaisedVolumeMode(cmd.params.volumeRaised);
                        SendReply(ReplyCode_OK);
                    } else {
                        SendReply(ReplyCode_BUSY);
                    }
                    break;

                case CommandType_PLAY_FINITE_TONE:
                    if (!Playback_QueueFull()) {
                        Playback_Enqueue((struct FiniteTone) {
                            .frequency = cmd.params.frequency, 
                            .duration = cmd.params.duration
                        });
                        SendReply(ReplyCode_OK);
                    }
                    else {
                        SendReply(ReplyCode_BUSY);
                    }
                    break;
                
                case CommandType_PLAY_INFINITE_TONE:
                    Playback_ResetQueue();
                    Tone_PlayInfinite(cmd.params.frequency);
                    SendReply(ReplyCode_OK);
                    break;

                case CommandType_STOP_PLAYING:
                    Tone_Stop();
                    Playback_ResetQueue();
                    SendReply(ReplyCode_OK);
                    break;
            }
        }

        if (Tone_GetRoutine() != ToneRoutine_FINITE && !Playback_QueueEmpty()) {
            struct FiniteTone finiteTone = Playback_Dequeue();
            Tone_PlayFinite(finiteTone);
            if (!Playback_QueueFull()) {
                SendReply(ReplyCode_READY);
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

static void SendStartupMessage(void)
{
    char buff[30];

    strcpy_P(buff, STARTUP_MSG1);
    SerialPort_PrintString(buff);
    strcpy_P(buff, STARTUP_MSG2);
    SerialPort_PrintString(buff);
}

static void SendReply(ReplyCode_t replyCode)
{
    char line[4];

    Protocol_BuildReply(&(struct Reply) {.code = replyCode}, line);
    SerialPort_PrintString(line);
}

static void ReadCommand(struct Command *cmd)
{
    char line[16];

    SerialPort_ReadPacket(line);
    SerialPort_Flush();
    Protocol_ParseCommand(line, cmd);
}