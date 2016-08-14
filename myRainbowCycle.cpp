#include "myLampConfig.h"
#include "myUtils.h"

extern CRGB leds[NUM_LEDS];
extern CRGB fg_color;
extern CRGB bg_color;

uint16_t j = 0;

void setupRainbowCycle() {
    for(int i = 0; i < kMatrixWidth; i++) {
        for(int j = 0; j < kMatrixHeight; j++) {
            leds[XY(i,j)] = bg_color;
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    //return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    return ((uint32_t)(255 - WheelPos * 3) << 16) | ((uint32_t)0 <<  8) | (WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    //return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    return ((uint32_t)0 << 16) | ((uint32_t)(WheelPos * 3) <<  8) | (255 - WheelPos * 3);
  }
  WheelPos -= 170;
  //return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  return ((uint32_t)(WheelPos * 3) << 16) | ((uint32_t)(255 - WheelPos * 3) <<  8) | 0;
}

void loopRainbowCycle() {
  uint16_t i = 0;

  j = j+1 % 768;

  //for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
    }
    //strip.show();
    delay(20);
  //}
}
