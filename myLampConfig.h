#ifndef __LAMPCONFIG_H__
#define __LAMPCONFIG_H__

#include "FastLED.h"
FASTLED_USING_NAMESPACE;

const bool kMatrixSerpentineLayout = true;
const uint8_t kMatrixWidth  = 4;
const uint8_t kMatrixHeight = 27;

#define MAX_DIMENSION ((kMatrixWidth > kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

#endif
