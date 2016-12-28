#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];

static uint16_t x;
static uint16_t y;
static uint16_t z;

void setupNoise()
{

    speed = 3;
    scale = 311;

    x = random16();
    z = random16();
    y = random16();
}

void loopNoise()
{
    static uint8_t ihue = 0;

    fillnoise8();

    for (int i = 0; i < kMatrixHeight; i++) {
        for (int j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = CHSV(noise[j][i], 255, noise[i][j]);
        }
    }

    ihue += 1;
}
