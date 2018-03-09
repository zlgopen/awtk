#include "base/color.h"

color_t color_init(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  color_t c;
  c.rgba.r = r;
  c.rgba.g = g;
  c.rgba.b = b;
  c.rgba.a = a;

  return c;
}
