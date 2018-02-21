
#include "tools/font_gen/font_gen.h"
#include <stdlib.h>
#include "base/array.h"
#include "base/font.h"
#include "base/mem.h"
#include "base/widget.h"
#include "font/font_bitmap.h"
#include "font/font_stb.h"
#include "gtest/gtest.h"
#include "tools/common/utils.h"

#include <string>
using std::string;

#define TTF_FILE LFTK_ROOT "/data/fonts/starthere.ttf"
#define BUFF_SIZE 1024 * 1024

TEST(FontGen, basic) {
  uint32_t size = 0;
  uint8_t* bmp_buff = (uint8_t*)MEM_ALLOC(BUFF_SIZE);
  uint8_t* ttf_buff = (uint8_t*)read_file(TTF_FILE, &size);
  font_t* ttf_font = font_stb_create("default", 20, ttf_buff);
  const char* str = "helloworldHELLOWORLD1243541helloworldHELLOWORLD1243541";

  uint32_t ret = font_gen_buff(ttf_font, str, bmp_buff, BUFF_SIZE);
  font_t* bmp_font = font_bitmap_create(bmp_buff);

  for (uint32_t i = 0; str[i]; i++) {
    glyph_t g1;
    glyph_t g2;
    char c = str[i];
    ASSERT_EQ(font_find_glyph(ttf_font, c, &g1), RET_OK);
    ASSERT_EQ(font_find_glyph(bmp_font, c, &g2), RET_OK);

    ASSERT_EQ(g1.x, g2.x);
    ASSERT_EQ(g1.y, g2.y);
    ASSERT_EQ(g1.w, g2.w);
    ASSERT_EQ(g1.h, g2.h);

    ASSERT_EQ(memcmp(g1.data, g2.data, g1.w * g1.h), 0);
  }

  ASSERT_EQ(ret > 0, true);
  font_destroy(ttf_font);
  font_destroy(bmp_font);
  MEM_FREE(bmp_buff);
  MEM_FREE(ttf_buff);
}
