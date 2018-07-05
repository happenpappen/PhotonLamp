#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];
extern CRGB fg_color;
extern CRGB bg_color;

void setupVerticalSplit()
{
    uint16_t i = 0;
    uint16_t j = 0;

    for (i = 0; i < kMatrixHeight; i++) {
        for (j = 0; j < round(kMatrixWidth/2); j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
    for (i = 0; i < kMatrixHeight; i++) {
        for (j = round(kMatrixWidth/2); j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
}

void loopVerticalSplit()
{
    uint16_t i = 0;
    uint16_t j = 0;

    for (i = 0; i < kMatrixHeight; i++) {
        for (j = 0; j < round(kMatrixWidth/2); j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
    for (i = 0; i < kMatrixHeight; i++) {
        for (j = round(kMatrixWidth/2); j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
}
