#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];
extern CRGB fg_color;
extern CRGB bg_color;

void setupCylon() { 
    for (int i = 0; i < kMatrixHeight; i++) {
        for (int j = 0; j < kMatrixWidth; j++) {
            leds[XY(i, j)] = bg_color;
        }
    }
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loopCylon() { 
	static uint8_t hue = 0;

	for(int i = 0; i < kMatrixHeight; i++) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
                leds[2*kMatrixHeight-i] = CHSV(hue, 255, 255);
                leds[i+2*kMatrixHeight] = CHSV(hue, 255, 255);
                leds[4*kMatrixHeight-i] = CHSV(hue, 255, 255);
		// Show the leds
		FastLED.show(); 
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(100);
	}

	// Now go in the other direction.  
	for(int i = (kMatrixHeight)-1; i >= 0; i--) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
                leds[2*kMatrixHeight-i] = CHSV(hue, 255, 255);
                leds[i+2*kMatrixHeight] = CHSV(hue, 255, 255);
                leds[4*kMatrixHeight-i] = CHSV(hue, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(100);
	}
}
