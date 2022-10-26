#include <tone.h>
#include <buzzer.h>
#include <time.h>

static uint32_t timeMarker = 0;
static uint16_t timeout;
static ToneRoutine_t routine = ToneRoutine_IDLE;

static void StartDurationTimeout(uint16_t duration)
{
    timeMarker = Time_MsPassed();
    timeout = duration;
}

void Tone_PlayFinite(struct FiniteTone finiteTone)
{
    if (finiteTone.frequency)
        Buzzer_SetFrequency(finiteTone.frequency);
    else
        Buzzer_Off();
    StartDurationTimeout(finiteTone.duration);
    routine = ToneRoutine_FINITE;
}

void Tone_PlayInfinite(uint16_t frequency)
{
    Buzzer_SetFrequency(frequency);
    routine = ToneRoutine_INFINITE;
}

void Tone_Update(void)
{
    if (routine == ToneRoutine_FINITE && 
            Time_MsPassed() - timeMarker >= timeout) {
        Tone_Stop();
    }
}

void Tone_Stop(void)
{
    Buzzer_Off();
    routine = ToneRoutine_IDLE;
}

ToneRoutine_t Tone_GetRoutine(void)
{
    return routine;
}
