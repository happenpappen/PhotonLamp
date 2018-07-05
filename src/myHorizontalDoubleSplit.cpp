#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];
extern CRGB fg_color;
extern CRGB bg_color;

void setupHorizontalDoubleSplit()
{
    uint16_t i = 0;
    uint16_t j = 0;

    for (i = 0; i < round(kMatrixHeight/3); i++) {
        for (j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
    for (i = round(kMatrixHeight/3); i < round(2*kMatrixHeight/3); i++) {
        for (j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
    for (i = round(2*kMatrixHeight/3); i < kMatrixHeight; i++) {
        for (j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
}

void loopHorizontalDoubleSplit()
{
    uint16_t i = 0;
    uint16_t j = 0;

    for (i = 0; i < round(kMatrixHeight/3); i++) {
        for (j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
    for (i = round(kMatrixHeight/3); i < round(2*kMatrixHeight/3); i++) {
        for (j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
    for (i = round(2*kMatrixHeight/3); i < kMatrixHeight; i++) {
        for (j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
}
