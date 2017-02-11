#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];
extern CRGB fg_color;
extern CRGB bg_color;

void setupHorizontalSplit()
{
    uint16_t i = 0;

    for (int i = 0; i < round(2*kMatrixHeight/3); i++) {
        for (int j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
    for (int i = round(2*kMatrixHeight/3); i < kMatrixHeight; i++) {
        for (int j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
}

void loopHorizontalSplit()
{
    uint16_t i = 0;

    for (int i = 0; i < round(2*kMatrixHeight/3); i++) {
        for (int j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
    for (int i = round(2*kMatrixHeight/3); i < kMatrixHeight; i++) {
        for (int j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = fg_color;
        }
    }
}
