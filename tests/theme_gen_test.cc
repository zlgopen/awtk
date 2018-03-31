
#include "base/array.h"
#include "base/theme.h"
#include "base/widget.h"
#include "tools/theme_gen/xml_theme_gen.h"
#include "gtest/gtest.h"
#include <stdlib.h>

#include <string>
using std::string;

TEST(ThemeGen, basic) {
  uint8_t buff[1024];
  theme_t theme;
  style_t style;
  const char* str =
      "<widget><style><normal bg-color=\"yellow\" fg-color=\"#fafbfc\" font-name=\"sans\" font-size=\"12\" /></style></widget>\
       <progress_bar><style><normal bg-color=\"rgb(255,255,0)\" fg-color=\"rgba(255,255,0,0.5)\" border-color=\"#ff00ff\" /></style></progress_bar>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style.data = theme_find_style(&theme, WIDGET_NONE, 0, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style.data != NULL, true);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_FONT_SIZE, 0), 12);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_BG_COLOR, 0), 0xff00ffff);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_FG_COLOR, 0), 0xfffcfbfa);
  ASSERT_EQ(style_get_str(&style, STYLE_ID_FONT_NAME, ""), string("sans"));

  style.data = theme_find_style(&theme, WIDGET_PROGRESS_BAR, 0, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style.data != NULL, true);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_BG_COLOR, 0), 0xff00ffff);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_FG_COLOR, 0), 0x7f00ffff);
}

TEST(ThemeGen, state) {
  uint8_t buff[1024];
  theme_t theme;
  style_t style;
  const char* str =
      "<button><style><over bg-color=\"yellow\" fg-color=\"#fafbfc\" font-name=\"sans\" font-size=\"12\" /></style></button>\
       <button><style><pressed bg-color=\"rgb(255,255,0)\" fg-color=\"rgba(255,255,0,0.5)\" border-color=\"#ff00ff\"/></style></button>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style.data = theme_find_style(&theme, WIDGET_BUTTON, 0, WIDGET_STATE_OVER);
  ASSERT_EQ(style.data != NULL, true);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_FONT_SIZE, 0), 12);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_BG_COLOR, 0), 0xff00ffff);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_FG_COLOR, 0), 0xfffcfbfa);
  ASSERT_EQ(style_get_str(&style, STYLE_ID_FONT_NAME, ""), string("sans"));

  style.data = theme_find_style(&theme, WIDGET_BUTTON, 0, WIDGET_STATE_PRESSED);
  ASSERT_EQ(style.data != NULL, true);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_BG_COLOR, 0), 0xff00ffff);
  ASSERT_EQ(style_get_int(&style, STYLE_ID_FG_COLOR, 0), 0x7f00ffff);
}

TEST(ThemeGen, style_type) {
  uint8_t buff[1024];
  theme_t theme;
  style_t style;
  const char* str =
      "<button><style name=\"1:yellow\"><over bg-color=\"yellow\" fg-color=\"#fafbfc\" font-name=\"sans\" font-size=\"12\" /></style></button>\
       <button><style name=\"1:yellow\"><pressed bg-color=\"rgb(255,255,0)\" fg-color=\"rgba(255,255,0,0.5)\" border-color=\"#ff00ff\" /></style></button>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style.data = theme_find_style(&theme, WIDGET_BUTTON, 1, WIDGET_STATE_OVER);
  ASSERT_EQ(style.data != NULL, true);

  style.data = theme_find_style(&theme, WIDGET_BUTTON, 1, WIDGET_STATE_PRESSED);
  ASSERT_EQ(style.data != NULL, true);
}
