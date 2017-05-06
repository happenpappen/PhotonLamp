#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];
extern CRGB fg_color;
extern CRGB bg_color;

uint16_t count = 0;

void setupRainSnow() {

    uint16_t i = 0;
    uint16_t j = 0;

    randomSeed(analogRead(A0));

    for (i = 0; i < kMatrixHeight; i++) {
        for (j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = 0;
        }
    }
    LEDS.show();
}

void loopRainSnow() {

    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t rand = 0;

    for (i = 0; i < kMatrixHeight; i++) {
        for (j = 0; j < kMatrixWidth; j++) {
            if (i == kMatrixHeight - 1) {
                if (leds[XY(i, j)] == CRGB(CRGB::Black)) {
                    rand = random(100);
                    if ((rand > 94) && (count == 0)) {
                        leds[XY(i, j)] = CRGB::White;
                    } else {
                        leds[XY(i, j)] = CRGB::Black;
                    }
                } else {
                    if (count > 5) {
                        leds[XY(i, j)] = CRGB::Black;
                        count = 0;
                    } else {
                        switch (count) {
                            case 4:
                            case 3:
                                leds[XY(i, j)] = leds[XY(i, j)] / 2;
                                count++;
                                break;
                            default:
                                leds[XY(i, j)] = CRGB::White;
                                count++;
                                break;
                        }
                    }
                }
            } else {
                leds[XY(i, j)] = leds[XY(i + 1, j)];
            }
        }
    }
    delay(50);
}
