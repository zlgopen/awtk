#include "tkc/color.h"
#include "tkc/color_parser.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(ColorParser, hex) {
  color_t c = color_parse("#AABBCC");
  ASSERT_EQ(c.rgba.r, 0xAA);
  ASSERT_EQ(c.rgba.g, 0xBB);
  ASSERT_EQ(c.rgba.b, 0xCC);
  ASSERT_EQ(c.rgba.a, 0xFF);
}

TEST(ColorParser, name) {
  color_t c = color_parse("white");
  ASSERT_EQ(c.rgba.r, 0xFF);
  ASSERT_EQ(c.rgba.g, 0xFF);
  ASSERT_EQ(c.rgba.b, 0xFF);
  ASSERT_EQ(c.rgba.a, 0xFF);

  c = color_parse("green");
  ASSERT_EQ(c.rgba.r, 0x0);
  ASSERT_EQ(c.rgba.g, 0x80);
  ASSERT_EQ(c.rgba.b, 0x0);
  ASSERT_EQ(c.rgba.a, 0xFF);
}

TEST(ColorParser, rgb) {
  color_t c = color_parse("rgb(11,22,33)");
  ASSERT_EQ(c.rgba.r, 11);
  ASSERT_EQ(c.rgba.g, 22);
  ASSERT_EQ(c.rgba.b, 33);
  ASSERT_EQ(c.rgba.a, 255);
}

TEST(ColorParser, rgba) {
  color_t c = color_parse("rgba(11,22,33,0.6)");
  ASSERT_EQ(c.rgba.r, 11);
  ASSERT_EQ(c.rgba.g, 22);
  ASSERT_EQ(c.rgba.b, 33);
  ASSERT_EQ(c.rgba.a, 255 * 0.6);

  c = color_parse("rgba(11,22,33,1)");
  ASSERT_EQ(c.rgba.r, 11);
  ASSERT_EQ(c.rgba.g, 22);
  ASSERT_EQ(c.rgba.b, 33);
  ASSERT_EQ(c.rgba.a, 255);
}
