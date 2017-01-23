#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];
extern CRGB fg_color;
extern CRGB bg_color;

uint16_t j = 0;

void setupRainbowCycle()
{
    for (int i = 0; i < kMatrixHeight; i++) {
        for (int j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return ((uint32_t)(255 - WheelPos * 3) << 16) | ((uint32_t)0 <<  8) | (WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return ((uint32_t)0 << 16) | ((uint32_t)(WheelPos * 3) <<  8) | (255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return ((uint32_t)(WheelPos * 3) << 16) | ((uint32_t)(255 - WheelPos * 3) <<  8) | 0;
}

void loopRainbowCycle()
{
    uint16_t i = 0;
    uint32_t t = 0;
    
    j = j + 1 % 768;

    for (i = 0; i < kMatrixHeight; i++) {
        t = Wheel(((i * 256 / kMatrixHeight) + j) & 255);
        leds[i] = t;
        leds[2*kMatrixHeight-i] = t;
        leds[i+2*kMatrixHeight] = t;
        leds[4*kMatrixHeight-i] = t;
    }

    delay(20);
}