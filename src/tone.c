#include <tone.h>
#include <buzzer.h>
#include <time.h>

static uint32_t timeMarker = 0;
static uint16_t timeout;
static bool playing = false;

static void _StartDurationTimeout(uint16_t duration)
{
    timeMarker = Time_MsPassed();
    timeout = duration;
}

void Tone_PlayFinite(uint16_t frequency, uint16_t duration)
{
    Buzzer_SetFrequency(frequency);
    _StartDurationTimeout(duration);
    playing = true;
}

void Tone_PlayInfinite(uint16_t frequency)
{
    Buzzer_SetFrequency(frequency);
    playing = true;
}

void Tone_PlayEmpty(uint16_t duration)
{
    Buzzer_Off();
    _StartDurationTimeout(duration);
    playing = true;
}

void Tone_Update(void)
{
    if (Time_MsPassed() - timeMarker >= timeout) {
        Tone_Stop();
    }
}

void Tone_Stop(void)
{
    Buzzer_Off();
    playing = false;
}

bool Tone_IsPlaying(void)
{
    return playing;
}
