#include <indicator.h>
#include <led.h>
#include <buzzer.h>
#include <tone.h>

void Indicator_OnWrongCmd(void)
{
    Led_SetColor(LedColor_RED);
}

void Indicator_Update(void)
{
    static ToneRoutine_t lastToneStatus = ToneRoutine_IDLE;
    static bool lastVolumeRaised = false;

    ToneRoutine_t toneStatus = Tone_GetRoutine();
    bool volumeRaised = Buzzer_VolumeRaised();

    if (toneStatus != lastToneStatus || volumeRaised != lastVolumeRaised) {
        const LedColor_t LED_COLORS[NUM_OF_TONE_ROUTINES][2] = {
            [ToneRoutine_IDLE] = {LedColor_GREEN, LedColor_CYAN},
            [ToneRoutine_FINITE] = {LedColor_BLUE, LedColor_PURPLE},
            [ToneRoutine_INFINITE] = {LedColor_YELLOW, LedColor_WHITE}
        };

        Led_SetColor(LED_COLORS[toneStatus][volumeRaised]);

        lastToneStatus = toneStatus;
        lastVolumeRaised = volumeRaised;
    }
}
