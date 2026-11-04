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

TEST(Color, channels) {
  color_t c = color_init(0x11, 0x22, 0x33, 0x44);

  ASSERT_EQ(sizeof(c.channels), (size_t)COLOR_CHANNEL_COUNT);
  ASSERT_EQ(COLOR_CHANNEL_R, 0);
  ASSERT_EQ(COLOR_CHANNEL_G, 1);
  ASSERT_EQ(COLOR_CHANNEL_B, 2);
  ASSERT_EQ(COLOR_CHANNEL_A, 3);
  ASSERT_EQ(COLOR_CHANNEL_COUNT, 4);

  ASSERT_EQ(c.channels[COLOR_CHANNEL_R], 0x11);
  ASSERT_EQ(c.channels[COLOR_CHANNEL_G], 0x22);
  ASSERT_EQ(c.channels[COLOR_CHANNEL_B], 0x33);
  ASSERT_EQ(c.channels[COLOR_CHANNEL_A], 0x44);

  ASSERT_EQ(c.rgba.r, c.channels[COLOR_CHANNEL_R]);
  ASSERT_EQ(c.rgba.g, c.channels[COLOR_CHANNEL_G]);
  ASSERT_EQ(c.rgba.b, c.channels[COLOR_CHANNEL_B]);
  ASSERT_EQ(c.rgba.a, c.channels[COLOR_CHANNEL_A]);
}

TEST(Color, channels_write_through) {
  color_t c;
  c.color = 0;

  c.channels[COLOR_CHANNEL_R] = 0x12;
  c.channels[COLOR_CHANNEL_G] = 0x34;
  c.channels[COLOR_CHANNEL_B] = 0x56;
  c.channels[COLOR_CHANNEL_A] = 0x78;

  ASSERT_EQ(c.rgba.r, 0x12);
  ASSERT_EQ(c.rgba.g, 0x34);
  ASSERT_EQ(c.rgba.b, 0x56);
  ASSERT_EQ(c.rgba.a, 0x78);

  c.rgba.r = 0x9a;
  c.rgba.g = 0xbc;
  c.rgba.b = 0xde;
  c.rgba.a = 0xf0;

  ASSERT_EQ(c.channels[COLOR_CHANNEL_R], 0x9a);
  ASSERT_EQ(c.channels[COLOR_CHANNEL_G], 0xbc);
  ASSERT_EQ(c.channels[COLOR_CHANNEL_B], 0xde);
  ASSERT_EQ(c.channels[COLOR_CHANNEL_A], 0xf0);
}

TEST(Color, accessors_null) {
  color_t c = color_init(1, 2, 3, 4);

  ASSERT_EQ(color_r(&c), 1);
  ASSERT_EQ(color_g(&c), 2);
  ASSERT_EQ(color_b(&c), 3);
  ASSERT_EQ(color_a(&c), 4);
  ASSERT_EQ(color_get_color(&c), c.color);
  ASSERT_EQ(color_cast(&c), &c);

  ASSERT_EQ(color_r(NULL), 0);
  ASSERT_EQ(color_g(NULL), 0);
  ASSERT_EQ(color_b(NULL), 0);
  ASSERT_EQ(color_a(NULL), 0);
  ASSERT_EQ(color_get_color(NULL), 0);
  ASSERT_EQ(color_cast(NULL), (color_t*)NULL);
  ASSERT_EQ(color_destroy(NULL), RET_BAD_PARAMS);
}

TEST(Color, hex_str) {
  char str[TK_COLOR_HEX_LEN + 1];

  ASSERT_STREQ(color_hex_str(color_init(0xAA, 0xBB, 0xCC, 0xFF), str), "#AABBCC");
  ASSERT_STREQ(color_hex_str(color_init(0xAA, 0xBB, 0xCC, 0xDD), str), "#aabbccdd");
}
