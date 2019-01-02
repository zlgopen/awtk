#include "tkc/color.h"
#include "tkc/color_parser.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(Color, basic) {
  char hex[8];
  const char* str = "#AABBCC";
  color_t c = color_parse(str);
  color_hex_str(c, hex);
  ASSERT_EQ(string(hex), string(str));

  str = "#AABBCCDD";
  c = color_parse(str);
  ASSERT_EQ(c.color, 0xddccbbaa);
}
