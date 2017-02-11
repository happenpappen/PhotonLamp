#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];
extern CRGB fg_color;
extern CRGB bg_color;

void setupVerticalSplit()
{
    uint16_t i = 0;

    for (int i = 0; i < kMatrixHeight; i++) {
        for (int j = 0; j < round(kMatrixWidth/2); j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
    for (int i = 0; i < kMatrixHeight; i++) {
        for (int j = round(kMatrixWidth/2); j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
}

void loopVerticalSplit()
{
    uint16_t i = 0;

    for (int i = 0; i < kMatrixHeight; i++) {
        for (int j = 0; j < round(kMatrixWidth/2); j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
    for (int i = 0; i < kMatrixHeight; i++) {
        for (int j = round(kMatrixWidth/2); j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
}
