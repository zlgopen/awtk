#include "common.h"
#include "tkc/utf8.h"
#include "gtest/gtest.h"

#define PRINTF printf

void bitmap_dump(bitmap_t* b) {
  rgba_t rgba;
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t w = b->w;
  uint32_t h = b->h;

  PRINTF("-----------------------------------------------\n");
  for (y = 0; y < h; y++) {
    PRINTF("%02d:", y);
    for (x = 0; x < w; x++) {
      ASSERT_EQ(bitmap_get_pixel(b, x, y, &rgba), RET_OK);
      PRINTF("%02x%02x%02x%02x ", rgba.r, rgba.g, rgba.b, rgba.a);
    }
    PRINTF("\n");
  }
}

void bitmap_check(bitmap_t* b, rect_t* r, rgba_t e) {
  rgba_t rgba;
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t w = b->w;
  uint32_t h = b->h;
  uint32_t bpp = bitmap_get_bpp(b);

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      ASSERT_EQ(bitmap_get_pixel(b, x, y, &rgba), RET_OK);
      if (rect_contains(r, x, y)) {
        ASSERT_EQ(rgba.r, e.r);
        ASSERT_EQ(rgba.g, e.g);
        ASSERT_EQ(rgba.b, e.b);
        ASSERT_EQ(rgba.a, e.a);
      } else {
        ASSERT_EQ(rgba.r, 0);
        ASSERT_EQ(rgba.g, 0);
        ASSERT_EQ(rgba.b, 0);

        if (bpp == 4) {
          ASSERT_EQ(rgba.a, 0);
        }
      }
    }
  }
}

#include <string>

using std::string;

void assert_str_eq(const wchar_t* wstr, const char* utf8) {
  char str[256];

  utf8_from_utf16(wstr, str, sizeof(str));
  ASSERT_EQ(string(str), string(utf8));
}
