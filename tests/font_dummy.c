#include "font_dummy.h"

static glyph_t s_glyph_0;
static glyph_t s_glyph_1;
static glyph_t s_glyph_2;

ret_t font_dummy_init() {
  s_glyph_0.x = 0;
  s_glyph_0.y = -10;
  s_glyph_0.w = 10;
  s_glyph_0.h = 10;

  s_glyph_1.x = 0;
  s_glyph_1.y = -11;
  s_glyph_1.w = 11;
  s_glyph_1.h = 11;

  s_glyph_2.x = 0;
  s_glyph_2.y = -5;
  s_glyph_2.w = 12;
  s_glyph_2.h = 12;

  return RET_OK;
}

static ret_t font_dummy_find_glyph(font_t* f, wchar_t chr, glyph_t* g) {
  if (chr == 0) {
    *g = s_glyph_0;
  } else if (chr == 1) {
    *g = s_glyph_1;
  } else {
    *g = s_glyph_2;
  }

  return RET_OK;
}

static font_t s_font0;
font_t* font_dummy_0(const char* name, uint16_t size) {
  s_font0.name = name;
  s_font0.size = size;
  s_font0.find_glyph = font_dummy_find_glyph;

  return &s_font0;
}

static font_t s_font1;
font_t* font_dummy_1(const char* name, uint16_t size) {
  s_font1.name = name;
  s_font1.size = size;
  s_font1.find_glyph = font_dummy_find_glyph;

  return &s_font1;
}

static font_t s_font2;
font_t* font_dummy_2(const char* name, uint16_t size) {
  s_font2.name = name;
  s_font2.size = size;
  s_font2.find_glyph = font_dummy_find_glyph;

  return &s_font2;
}
