#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];
extern CRGB fg_color;
extern CRGB bg_color;

int direction = 1;
uint8_t cur_x = 0;
uint8_t cur_y = 0;

void setupSpiral() {
    for (int i = 0; i < kMatrixHeight; i++) {
        for (int j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }

    direction = 1;
    cur_x = 0;
    cur_y = 0;
}

void loopSpiral() {

    leds[XY(cur_x, cur_y)] = (direction >0) ? fg_color : bg_color;

    cur_x = cur_x + direction;
    cur_y = cur_y + direction;

    if (direction >0) {

    } else {
    }


	FastLED.show();
	delay(100);
}
