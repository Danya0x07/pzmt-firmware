#include <indicator.h>
#include <led.h>
#include <buzzer.h>
#include <tone.h>

void Indicator_ShowBadRequest(void)
{
    Led_SetColor(LedColor_Red);
}

void Indicator_Update(void)
{
    static ToneStatus_t lastToneStatus = ToneStatus_Idle;
    static bool lastVolumeRaised = false;

    ToneStatus_t toneStatus = Tone_GetStatus();
    bool volumeRaised = Buzzer_VolumeRaised();

    if (toneStatus != lastToneStatus || volumeRaised != lastVolumeRaised) {
        const enum LedColor LED_COLORS[NumOfToneStatuses][2] = {
            [ToneStatus_Idle] = {LedColor_Green, LedColor_Cyan},
            [ToneStatus_Finite] = {LedColor_Blue, LedColor_Purple},
            [ToneStatus_Infinite] = {LedColor_Yellow, LedColor_White}
        };

        Led_SetColor(LED_COLORS[toneStatus][volumeRaised]);

        lastToneStatus = toneStatus;
        lastVolumeRaised = volumeRaised;
    }
}
