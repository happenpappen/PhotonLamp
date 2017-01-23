#ifndef __MYUTILS_H__
#define __MYUTILS_H__

#include <stdint.h>

// This is the array that we keep our computed noise values in
extern uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// use the z-axis for "time".  speed determines how fast time moves forward.  Try
// 1 for a very slow moving effect, or 60 for something that ends up looking like
// water.
extern uint16_t speed; // speed is set dynamically once we've started up

// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.  The
// higher the value of scale, the more "zoomed out" the noise iwll be.  A value
// of 1 will be so zoomed in, you'll mostly see solid colors.
extern uint16_t scale; // scale is set dynamically once we've started up

uint16_t XY(uint8_t x, uint8_t y);
void fillnoise8();
uint16_t getDistance();

#endif
