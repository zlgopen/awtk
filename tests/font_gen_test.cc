
#include "base/font.h"
#include "tkc/mem.h"
#include "base/widget.h"
#include "tools/common/utils.h"
#include "tools/font_gen/font_gen.h"
#include "font_loader/font_loader_truetype.h"
#include "font_loader/font_loader_bitmap.h"
#include "gtest/gtest.h"
#include <stdlib.h>

#include <string>
using std::string;

#define TTF_FILE TK_ROOT "/tests/testdata/assets/default/raw/fonts/starthere.ttf"
#define BUFF_SIZE 1024 * 1024

#if defined(WITH_FT_FONT) || defined(WITH_STB_FONT)
TEST(FontGen, basic) {
  uint32_t size = 0;
  uint16_t font_size = 50;
  uint8_t* bmp_buff = (uint8_t*)TKMEM_ALLOC(BUFF_SIZE);
  uint8_t* ttf_buff = (uint8_t*)read_file(TTF_FILE, &size);
  font_t* ttf_font = font_truetype_create("default", ttf_buff, size);
  const char* str = "helloworld HELLOWORLD 1243541 helloworld HELLOWORLD 1243541";
  wbuffer_t wbuffer;
  wbuffer_init(&wbuffer, bmp_buff, BUFF_SIZE);
  uint32_t ret = font_gen_buff(ttf_font, font_size, GLYPH_FMT_ALPHA, str, &wbuffer);
  font_t* bmp_font = font_bitmap_create("default", bmp_buff, ret);

  for (uint32_t i = 0; str[i]; i++) {
    glyph_t g1;
    glyph_t g2;
    char c = str[i];
    ASSERT_EQ(font_get_glyph(ttf_font, c, font_size, &g1), RET_OK);
    ASSERT_EQ(font_get_glyph(bmp_font, c, font_size, &g2), RET_OK);

    ASSERT_EQ(g1.x, g2.x);
    ASSERT_EQ(g1.y, g2.y);
    ASSERT_EQ(g1.w, g2.w);
    ASSERT_EQ(g1.h, g2.h);

    ASSERT_EQ(memcmp(g1.data, g2.data, g1.pitch * g1.h), 0);
  }

  ASSERT_EQ(ret > 0, true);
  font_destroy(ttf_font);
  font_destroy(bmp_font);
  wbuffer_deinit(&wbuffer);
  TKMEM_FREE(bmp_buff);
  TKMEM_FREE(ttf_buff);
}

TEST(FontGen, expand) {
  str_t str;
  str_init(&str, 100);

  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("[[0-9]][[a-z]][[A-Z]][]!@#$", &str),
               "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]!@#$");

#ifndef WIN32
  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("[[0x4e2d-0x4e2f]]", &str), "中丮丯");

  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("[[0X4e2d-0X4e2f]]", &str), "中丮丯");

  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("[[0X4E2D-0X4E2F]]", &str), "中丮丯");

  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("[[20013-20015]]", &str), "中丮丯");

  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("123[[20013-20015]]abc", &str), "123中丮丯abc");

  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("[[0-9]][[20013-20015]][[a-c]]", &str), "0123456789中丮丯abc");
#endif

  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("[[48-51]]", &str), "0123");

  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("abc[[d-d]]", &str), "abcd");
  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("abc[[d-e]]", &str), "abcde");
  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("abc[[", &str), "abc");
  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("abc[[]]", &str), "abc");
  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("abc[[48-51]]", &str), "abc0123");

  str_clear(&str);
  ASSERT_STREQ(font_gen_expand_text("abc[[48-51]][[d-e]]", &str), "abc0123de");

  str_reset(&str);
}

#endif /**/
