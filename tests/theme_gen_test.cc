
#include "base/theme.h"
#include "base/widget.h"
#include "tools/theme_gen/xml_theme_gen.h"
#include "gtest/gtest.h"
#include <stdlib.h>

#include <string>
using std::string;

TEST(ThemeGen, basic0) {
  uint8_t buff[1024];
  theme_t theme;
  const uint8_t* style_data = NULL;
  const char* str = "<widget><style><normal bg_color=\"#fafbfc\"/></style></widget>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_NONE, TK_DEFAULT_STYLE, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BG_COLOR, 0), 0xfffcfbfa);
}

TEST(ThemeGen, basic1) {
  uint8_t buff[1024];
  theme_t theme;
  const uint8_t* style_data = NULL;
  const char* str =
      "<widget><style><normal bg_color=\"#fafbfc\" fg_color=\"#223344\" font_name=\"sans\" "
      "font_size=\"12\"/></style></widget>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_NONE, TK_DEFAULT_STYLE, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BG_COLOR, 0), 0xfffcfbfa);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FG_COLOR, 0), 0xff443322);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FONT_SIZE, 0), 12);
  ASSERT_EQ(style_data_get_str(style_data, STYLE_ID_FONT_NAME, ""), string("sans"));
}

TEST(ThemeGen, basic2) {
  uint8_t buff[1024];
  theme_t theme;
  const uint8_t* style_data = NULL;
  const char* str =
      "<widget>\
      <style><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
      <style><over bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
      <style><focus bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
      </widget>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_NONE, TK_DEFAULT_STYLE, WIDGET_STATE_OVER);
  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BG_COLOR, 0), 0xff332211);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FG_COLOR, 0), 0xff443322);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FONT_SIZE, 0), 12);
  ASSERT_EQ(style_data_get_str(style_data, STYLE_ID_FONT_NAME, ""), string("sans"));
}

TEST(ThemeGen, basic3) {
  uint8_t buff[10240];
  theme_t theme;
  const uint8_t* style_data = NULL;
  const char* str =
      "<widget>\
        <style name=\"default\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"1\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"2\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
      </widget>\
      <button>\
        <style name=\"default\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"1\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"2\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
      </button>\
      <label>\
        <style name=\"default\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"1\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"2\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
      </label>\
      ";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, TK_DEFAULT_STYLE, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BG_COLOR, 0), 0xff332211);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FG_COLOR, 0), 0xff443322);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FONT_SIZE, 0), 12);
  ASSERT_EQ(style_data_get_str(style_data, STYLE_ID_FONT_NAME, ""), string("sans"));
}

TEST(ThemeGen, basic4) {
  uint8_t buff[10240];
  theme_t theme;
  const uint8_t* style_data = NULL;
  const char* str =
      "<widget>\
        <style name=\"default\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"1\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"2\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
      </widget>\
      <button>\
        <style name=\"default\"> \
          <normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/> \
          <over bg_color=\"#112244\" fg_color=\"#223355\" font_name=\"sans\" font_size=\"12\"/> \
          <focus bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/> \
        </style>\
        <style name=\"1\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"2\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
      </button>\
      <label>\
        <style name=\"default\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"1\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
        <style name=\"2\"><normal bg_color=\"#112233\" fg_color=\"#223344\" font_name=\"sans\" font_size=\"12\"/></style>\
      </label>\
      ";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, TK_DEFAULT_STYLE, WIDGET_STATE_OVER);
  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BG_COLOR, 0), 0xff442211);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FG_COLOR, 0), 0xff553322);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FONT_SIZE, 0), 12);
  ASSERT_EQ(style_data_get_str(style_data, STYLE_ID_FONT_NAME, ""), string("sans"));
}

TEST(ThemeGen, state) {
  uint8_t buff[1024];
  theme_t theme;
  const uint8_t* style_data = NULL;
  const char* str =
      "<button><style><over bg_color=\"#f1f2f3\" fg_color=\"#fafbfc\" font_name=\"sans\" font_size=\"12\" /></style></button>\
       <button><style><pressed bg_color=\"rgb(255,255,0)\" fg_color=\"rgba(255,255,0,0.5)\" border_color=\"#ff00ff\"/></style></button>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, TK_DEFAULT_STYLE, WIDGET_STATE_OVER);
  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FONT_SIZE, 0), 12);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BG_COLOR, 0), 0xfff3f2f1);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FG_COLOR, 0), 0xfffcfbfa);
  ASSERT_EQ(style_data_get_str(style_data, STYLE_ID_FONT_NAME, ""), string("sans"));

  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, TK_DEFAULT_STYLE, WIDGET_STATE_PRESSED);
  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BG_COLOR, 0), 0xff00ffff);
}

TEST(ThemeGen, style_type) {
  uint8_t buff[1024];
  theme_t theme;
  const uint8_t* style_data = NULL;
  const char* str =
      "<button><style name=\"yellow\"><over bg_color=\"yellow\" fg_color=\"#fafbfc\" font_name=\"sans\" font_size=\"12\" /></style></button>\
       <button><style name=\"yellow\"><pressed bg_color=\"rgb(255,255,0)\" fg_color=\"rgba(255,255,0,0.5)\" border_color=\"#ff00ff\" /></style></button>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, "yellow", WIDGET_STATE_OVER);
  ASSERT_EQ(style_data != NULL, true);

  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, "yellow", WIDGET_STATE_PRESSED);
  ASSERT_EQ(style_data != NULL, true);
}

TEST(ThemeGen, inher) {
  uint8_t buff[1024];
  theme_t theme;
  const uint8_t* style_data = NULL;
  const char* str =
      "<button font_size=\"12\"><style name=\"yellow\" font_name=\"sans\"><over bg_color=\"yellow\" fg_color=\"#fafbfc\" /></style>\
       <style name=\"yellow\"><pressed bg_color=\"rgb(255,255,0)\" font_name=\"serif\" font_size=\"14\" /></style></button>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, "yellow", WIDGET_STATE_OVER);
  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FONT_SIZE, 0), 12);
  ASSERT_EQ(style_data_get_str(style_data, STYLE_ID_FONT_NAME, ""), string("sans"));

  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, "yellow", WIDGET_STATE_PRESSED);
  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_FONT_SIZE, 0), 14);
  ASSERT_EQ(style_data_get_str(style_data, STYLE_ID_FONT_NAME, ""), string("serif"));
}

TEST(ThemeGen, border) {
  uint8_t buff[1024];
  theme_t theme;
  const uint8_t* style_data = NULL;
  const char* str = "<button><style><normal border=\"left\" /></style></button>";
  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, TK_DEFAULT_STYLE, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BORDER, 0), BORDER_LEFT);

  str = "<button><style><normal border=\"right\" /></style></button>";
  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;
  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, TK_DEFAULT_STYLE, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BORDER, 0), BORDER_RIGHT);

  str = "<button><style><normal border=\"top\" /></style></button>";
  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;
  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, TK_DEFAULT_STYLE, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BORDER, 0), BORDER_TOP);

  str = "<button><style><normal border=\"bottom\" /></style></button>";
  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;
  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, TK_DEFAULT_STYLE, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BORDER, 0), BORDER_BOTTOM);

  str = "<button><style><normal border=\"all\" /></style></button>";
  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;
  style_data = theme_find_style(&theme, WIDGET_TYPE_BUTTON, TK_DEFAULT_STYLE, WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data_get_int(style_data, STYLE_ID_BORDER, 0), BORDER_ALL);
}

TEST(ThemeGen, active_state) {
  uint8_t buff[10240];
  theme_t theme;
  color_t def = color_init(0, 0, 0, 0);
  const uint8_t* style_data = NULL;
  const char* str =
      "<tab_button> \
  <style name=\"default\" text_align_h=\"left\" margin=\"4\" border_color=\"#cccccc\" bg_color=\"#eeeeee\"> \
    <normal     text_color=\"#111111\"/> \
    <pressed    text_color=\"#222222\"/> \
    <over       text_color=\"#333333\"/> \
    <normal_of_active     text_color=\"#444444\"/> \
    <pressed_of_active    text_color=\"#555555\"/> \
    <over_of_active       text_color=\"#666666\"/> \
  </style> \
</tab_button>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_TAB_BUTTON, "default", WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x11);

  style_data = theme_find_style(&theme, WIDGET_TYPE_TAB_BUTTON, "default", WIDGET_STATE_PRESSED);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x22);

  style_data = theme_find_style(&theme, WIDGET_TYPE_TAB_BUTTON, "default", WIDGET_STATE_OVER);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x33);

  style_data =
      theme_find_style(&theme, WIDGET_TYPE_TAB_BUTTON, "default", WIDGET_STATE_NORMAL_OF_ACTIVE);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x44);

  style_data =
      theme_find_style(&theme, WIDGET_TYPE_TAB_BUTTON, "default", WIDGET_STATE_PRESSED_OF_ACTIVE);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x55);

  style_data =
      theme_find_style(&theme, WIDGET_TYPE_TAB_BUTTON, "default", WIDGET_STATE_OVER_OF_ACTIVE);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x66);
}

TEST(ThemeGen, selected_state) {
  uint8_t buff[10240];
  theme_t theme;
  color_t def = color_init(0, 0, 0, 0);
  const uint8_t* style_data = NULL;
  const char* str =
      "<combo_box_item> \
  <style name=\"default\" text_align_h=\"left\" margin=\"4\" border_color=\"#cccccc\" bg_color=\"#eeeeee\"> \
    <normal     text_color=\"#111111\"/> \
    <pressed    text_color=\"#222222\"/> \
    <over       text_color=\"#333333\"/> \
    <normal_of_checked     text_color=\"#444444\"/> \
    <pressed_of_checked    text_color=\"#555555\"/> \
    <over_of_checked       text_color=\"#666666\"/> \
  </style> \
</combo_box_item>";

  xml_gen_buff(str, buff, sizeof(buff));
  theme.data = buff;

  style_data = theme_find_style(&theme, WIDGET_TYPE_COMBO_BOX_ITEM, "default", WIDGET_STATE_NORMAL);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x11);

  style_data =
      theme_find_style(&theme, WIDGET_TYPE_COMBO_BOX_ITEM, "default", WIDGET_STATE_PRESSED);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x22);

  style_data = theme_find_style(&theme, WIDGET_TYPE_COMBO_BOX_ITEM, "default", WIDGET_STATE_OVER);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x33);

  style_data = theme_find_style(&theme, WIDGET_TYPE_COMBO_BOX_ITEM, "default",
                                WIDGET_STATE_NORMAL_OF_CHECKED);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x44);

  style_data = theme_find_style(&theme, WIDGET_TYPE_COMBO_BOX_ITEM, "default",
                                WIDGET_STATE_PRESSED_OF_CHECKED);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x55);

  style_data =
      theme_find_style(&theme, WIDGET_TYPE_COMBO_BOX_ITEM, "default", WIDGET_STATE_OVER_OF_CHECKED);
  ASSERT_EQ(style_data_get_color(style_data, STYLE_ID_TEXT_COLOR, def).rgba.r, 0x66);
}
