/*
 * Taken from: http://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 */

#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];

static byte heat[kMatrixHeight];

void setupFire() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = 0;
    }
}

void loopFire() {

    int Cooling = 55;
    int Sparking = 120;
    int cooldown = 0;

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < kMatrixHeight; i++) {
            cooldown = random(0, ((Cooling * 10) / kMatrixHeight) + 2);

            if (cooldown > heat[i]) {
                heat[i] = 0;
            } else {
                heat[i] = heat[i] - cooldown;
        }
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int i = 0; i < kMatrixHeight; i++) {
            heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' near the bottom
    for (int i = 0; i < kMatrixHeight; i++) {
        int y = 0;
        if (random(255) < Sparking) {
            y = random(7);
            heat[y] = heat[y] + random(160, 255);
        }
    }

    // Step 4.  Convert heat to LED colors
    // Scale 'heat' down from 0-255 to 0-191
    byte t192 = 0;
    byte heatramp = 0;

    for (int i = 0; i < kMatrixHeight; i++) {
            // Scale 'heat' down from 0-255 to 0-191
            t192 = round((heat[i] / 255.0)*191);

            // calculate ramp up from
            heatramp = t192 & 0x3F; // 0..63
            heatramp <<= 2; // scale up to 0..252

            // figure out which third of the spectrum we're in:
            if (t192 > 0x80) { // hottest
                leds[XY(i, 0)] = CRGB(255, 255, heatramp);
                leds[XY(i, 1)] = CRGB(255, 255, heatramp);
                leds[XY(i, 2)] = CRGB(255, 255, heatramp);
                leds[XY(i, 3)] = CRGB(255, 255, heatramp);
            } else if (t192 > 0x40) { // middle
                leds[XY(i, 0)] = CRGB(heatramp, 255, 0);
                leds[XY(i, 1)] = CRGB(heatramp, 255, 0);
                leds[XY(i, 2)] = CRGB(heatramp, 255, 0);
                leds[XY(i, 3)] = CRGB(heatramp, 255, 0);
            } else { // coolest
                leds[XY(i, 0)] = CRGB(0, heatramp, 0);
                leds[XY(i, 1)] = CRGB(0, heatramp, 0);
                leds[XY(i, 2)] = CRGB(0, heatramp, 0);
                leds[XY(i, 3)] = CRGB(0, heatramp, 0);
            }
    }

    delay(120);
}
