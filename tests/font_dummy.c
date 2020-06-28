#include "tkc/utils.h"
#include "font_dummy.h"

static glyph_t s_glyph_0;
static glyph_t s_glyph_1;
static glyph_t s_glyph_2;

static uint8_t glyph_data[] = {0x1, 0x2, 0x3, 0x4, 0x5};

typedef struct _font_dummy_t {
  font_t base;
  uint16_t font_size;
} font_dummy_t;

ret_t font_dummy_init() {
  s_glyph_0.x = 0;
  s_glyph_0.y = -10;
  s_glyph_0.w = 10;
  s_glyph_0.h = 10;
  s_glyph_0.data = glyph_data;

  s_glyph_1.x = 0;
  s_glyph_1.y = -11;
  s_glyph_1.w = 11;
  s_glyph_1.h = 11;
  s_glyph_1.data = glyph_data;

  s_glyph_2.x = 0;
  s_glyph_2.y = -5;
  s_glyph_2.w = 12;
  s_glyph_2.h = 12;
  s_glyph_2.data = glyph_data;

  return RET_OK;
}

static ret_t font_dummy_get_glyph(font_t* f, wchar_t chr, uint16_t font_size, glyph_t* g) {
  if (chr == 0) {
    *g = s_glyph_0;
  } else if (chr == 1) {
    *g = s_glyph_1;
  } else {
    *g = s_glyph_2;
  }

  (void)font_size;

  return RET_OK;
}

static bool_t font_dummy_match(font_t* f, const char* name, uint16_t font_size) {
  font_dummy_t* font = (font_dummy_t*)f;

  return font->font_size == font_size;
}

static font_dummy_t s_font0;
font_t* font_dummy_0(const char* name, uint16_t size) {
  s_font0.font_size = size;
  s_font0.base.match = font_dummy_match;
  s_font0.base.get_glyph = font_dummy_get_glyph;

  tk_strncpy(s_font0.base.name, name, TK_NAME_LEN);

  return &s_font0.base;
}

static font_dummy_t s_font1;
font_t* font_dummy_1(const char* name, uint16_t size) {
  s_font1.font_size = size;
  s_font1.base.match = font_dummy_match;
  s_font1.base.get_glyph = font_dummy_get_glyph;

  tk_strncpy(s_font1.base.name, name, TK_NAME_LEN);

  return &s_font1.base;
}

static font_dummy_t s_font2;
font_t* font_dummy_2(const char* name, uint16_t size) {
  s_font2.font_size = size;
  s_font2.base.match = font_dummy_match;
  s_font2.base.get_glyph = font_dummy_get_glyph;
  tk_strncpy(s_font2.base.name, name, TK_NAME_LEN);

  return &s_font2.base;
}
