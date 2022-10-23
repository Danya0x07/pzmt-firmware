#include <tone.h>
#include <buzzer.h>
#include <time.h>

static uint32_t timeMarker = 0;
static uint16_t timeout;
static ToneStatus_t status = ToneStatus_Idle;

static void _StartDurationTimeout(uint16_t duration)
{
    timeMarker = Time_MsPassed();
    timeout = duration;
}

void Tone_PlayFinite(uint16_t frequency, uint16_t duration)
{
    Buzzer_SetFrequency(frequency);
    _StartDurationTimeout(duration);
    status = ToneStatus_Finite;
}

void Tone_PlayInfinite(uint16_t frequency)
{
    Buzzer_SetFrequency(frequency);
    status = ToneStatus_Infinite;
}

void Tone_PlayEmpty(uint16_t duration)
{
    Buzzer_Off();
    _StartDurationTimeout(duration);
    status = ToneStatus_Finite;
}

void Tone_Update(void)
{
    if (status == ToneStatus_Finite && 
            Time_MsPassed() - timeMarker >= timeout) {
        Tone_Stop();
    }
}

void Tone_Stop(void)
{
    Buzzer_Off();
    status = ToneStatus_Idle;
}

ToneStatus_t Tone_GetStatus(void)
{
    return status;
}
