#include "base/color.h"
#include "base/utils.h"

color_t color_init(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  color_t c;

  c.color = 0;
  c.rgba.r = r;
  c.rgba.g = g;
  c.rgba.b = b;
  c.rgba.a = a;

  return c;
}

const char* color_hex_str(color_t c, char str[8]) {
  tk_snprintf(str, 8, "#%02X%02X%02X", (int)(c.rgba.r), (int)(c.rgba.g), (int)(c.rgba.b));

  return str;
}
