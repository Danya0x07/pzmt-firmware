#include <avr/pgmspace.h>

#include <indicator.h>
#include <led.h>
#include <buzzer.h>
#include <tone.h>

#define INITIAL_LED_COLOR   LedColor_GREEN

static const PROGMEM LedColor_t LED_COLORS[NUM_OF_TONE_ROUTINES][2] = {
    [ToneRoutine_IDLE] = {LedColor_GREEN, LedColor_CYAN},
    [ToneRoutine_FINITE] = {LedColor_BLUE, LedColor_PURPLE},
    [ToneRoutine_INFINITE] = {LedColor_YELLOW, LedColor_WHITE}
};

void Indicator_OnStartup(void)
{
    Led_SetColor(INITIAL_LED_COLOR);
}

void Indicator_OnWrongCmd(void)
{
    Led_SetColor(LedColor_RED);
}

void Indicator_Update(void)
{
    static LedColor_t lastLedColor = INITIAL_LED_COLOR;
    static ToneRoutine_t lastToneRoutine = ToneRoutine_IDLE;
    static bool lastVolumeRaised = false;

    LedColor_t ledColor = Led_GetColor();
    ToneRoutine_t toneRoutine = Tone_GetRoutine();
    bool volumeRaised = Buzzer_VolumeRaised();

    if (ledColor != lastLedColor || toneRoutine != lastToneRoutine 
            || volumeRaised != lastVolumeRaised) {
        ledColor = pgm_read_byte(&LED_COLORS[toneRoutine][volumeRaised]);
        Led_SetColor(ledColor);
        lastLedColor = ledColor;
        lastToneRoutine = toneRoutine;
        lastVolumeRaised = volumeRaised;
    }
}
