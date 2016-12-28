#include <stdint.h>
#include "myLampConfig.h"

uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
uint16_t speed = 3; // speed is set dynamically once we've started up
uint16_t scale = 30; // scale is set dynamically once we've started up

uint16_t XY(uint8_t x, uint8_t y)
{
    uint16_t retcode = 0;

    if (x >= kMatrixHeight) { x = kMatrixHeight - 1; }
    if (x < 0) { x = 0; }
    if (y >= kMatrixWidth) { y = kMatrixWidth - 1; }
    if (y < 0) { y = 0; }

    if (y % 2 == 0) {
        retcode = (y * kMatrixHeight) + x;    
    } else {
        retcode = (y + 1) * kMatrixHeight - x -1;            
    }
#ifdef DEBUG
    Serial.printf("XY: %d, X: %d, Y: %d", retcode, x, y);
    Serial.println();
#endif
    return retcode;
}

static uint16_t x;
static uint16_t y;
static uint16_t z;

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8()
{
    // If we're runing at a low "speed", some 8-bit artifacts become visible
    // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
    // The amount of data smoothing we're doing depends on "speed".
    uint8_t dataSmoothing = 0;
    if (speed < 50) {
        dataSmoothing = 200 - (speed * 4);
    }

    for (int i = 0; i < MAX_DIMENSION; i++) {
        int ioffset = scale * i;
        for (int j = 0; j < MAX_DIMENSION; j++) {
            int joffset = scale * j;

            uint8_t data = inoise8(x + ioffset, y + joffset, z);

            // The range of the inoise8 function is roughly 16-238.
            // These two operations expand those values out to roughly 0..255
            // You can comment them out if you want the raw noise data.
            data = qsub8(data, 16);
            data = qadd8(data, scale8(data, 39));

            if (dataSmoothing) {
                uint8_t olddata = noise[i][j];
                uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
                data = newdata;
            }

            noise[i][j] = data;
        }
    }

    z += speed;

    // apply slow drift to X and Y, just for visual variation.
    x += speed / 8;
    y -= speed / 16;
}

uint16_t getDistance()
{
    uint32_t duration, cm;
    static bool init = false;
    if (!init) {
        pinMode(TRIGGER_PIN, OUTPUT);
        digitalWriteFast(TRIGGER_PIN, LOW);
        pinMode(ECHO_PIN, INPUT);
        delay(50);
        init = true;
    }

    ATOMIC_BLOCK() {
        /* Trigger the sensor by sending a HIGH pulse of 10 or more microseconds */
        digitalWriteFast(TRIGGER_PIN, HIGH);
        delayMicroseconds(10);
        digitalWriteFast(TRIGGER_PIN, LOW);
        duration = pulseIn(ECHO_PIN, HIGH);
    }
    
    /* Convert the time into a distance */
    // Sound travels at 340 m/s (29 us/cm), out and back so divide by 2
    // Ref: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
    cm = duration / 29 / 2;

    return cm;
}
