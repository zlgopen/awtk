#include "tkc/color.h"
#include "tkc/color_parser.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(Color, basic) {
  char hex[TK_COLOR_HEX_LEN + 1];
  const char* str = "#AABBCC";
  color_t c = color_parse(str);
  color_hex_str(c, hex);
  ASSERT_EQ(string(hex), string(str));

  str = "#AABBCCDD";
  c = color_parse(str);
  ASSERT_EQ(c.color, 0xddccbbaa);
}

TEST(Color, dynamic) {
  color_t* c = color_create(1, 2, 3, 4);

  ASSERT_EQ(color_r(c), c->rgba.r);
  ASSERT_EQ(color_g(c), c->rgba.g);
  ASSERT_EQ(color_b(c), c->rgba.b);
  ASSERT_EQ(color_a(c), c->rgba.a);

  ASSERT_EQ(color_r(c), 1);
  ASSERT_EQ(color_g(c), 2);
  ASSERT_EQ(color_b(c), 3);
  ASSERT_EQ(color_a(c), 4);

  color_destroy(c);
}

TEST(Color, dynamic1) {
  const char* str = "#01020304";
  color_t* c = color_create(0, 0, 0, 0);

  color_from_str(c, str);

  ASSERT_EQ(color_r(c), c->rgba.r);
  ASSERT_EQ(color_g(c), c->rgba.g);
  ASSERT_EQ(color_b(c), c->rgba.b);
  ASSERT_EQ(color_a(c), c->rgba.a);

  ASSERT_EQ(color_r(c), 1);
  ASSERT_EQ(color_g(c), 2);
  ASSERT_EQ(color_b(c), 3);
  ASSERT_EQ(color_a(c), 4);

  color_destroy(c);
}

TEST(Color, rgba) {
  char str[TK_COLOR_RGBA_LEN + 1];
  color_t c = color_init(1, 2, 3, 0xff);

  ASSERT_STREQ(color_rgba_str(c, str), "rgba(1,2,3,1.0)");
}
