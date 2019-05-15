#include "tkc/mem.h"
#include "tkc/color.h"
#include "tkc/utils.h"

color_t color_init(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  color_t c;

  c.color = 0;
  c.rgba.r = r;
  c.rgba.g = g;
  c.rgba.b = b;
  c.rgba.a = a;

  return c;
}

color_t* color_create(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  color_t* c = TKMEM_ZALLOC(color_t);
  return_value_if_fail(c != NULL, NULL);

  *c = color_init(r, g, b, a);

  return c;
}

ret_t color_destroy(color_t* c) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(c);

  return RET_OK;
}

uint8_t color_r(color_t* c) {
  return_value_if_fail(c != NULL, 0);

  return c->rgba.r;
}

uint8_t color_g(color_t* c) {
  return_value_if_fail(c != NULL, 0);

  return c->rgba.g;
}

uint8_t color_b(color_t* c) {
  return_value_if_fail(c != NULL, 0);

  return c->rgba.b;
}

uint8_t color_a(color_t* c) {
  return_value_if_fail(c != NULL, 0);

  return c->rgba.a;
}

color_t* color_cast(color_t* color) {
  return_value_if_fail(color != NULL, NULL);

  return color;
}

const char* color_hex_str(color_t c, char str[TK_COLOR_HEX_LEN + 1]) {
  if (c.rgba.a == 0xff) {
    tk_snprintf(str, TK_COLOR_HEX_LEN, "#%02X%02X%02X", (int)(c.rgba.r), (int)(c.rgba.g),
                (int)(c.rgba.b));
  } else {
    tk_snprintf(str, TK_COLOR_HEX_LEN, "#%02x%02x%02x%02x", (int)(c.rgba.r), (int)(c.rgba.g),
                (int)(c.rgba.b), (int)(c.rgba.a));
  }

  return str;
}

const char* color_rgba_str(color_t c, char str[TK_COLOR_RGBA_LEN + 1]) {
  float a = c.rgba.a / 255.0f;

  tk_snprintf(str, TK_COLOR_RGBA_LEN, "rgba(%d,%d,%d,%1.1f)", (int)(c.rgba.r), (int)(c.rgba.g),
              (int)(c.rgba.b), a);

  return str;
}
