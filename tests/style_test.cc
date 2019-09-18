#include "gtest/gtest.h"
#include "widgets/button.h"
#include "tkc/value.h"
#include "base/window.h"
#include "base/style_mutable.h"

#include <string>

using std::string;

TEST(Style, normalize_int) {
  value_t v;
  ASSERT_EQ(style_normalize_value("font_size", "16", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);
  ASSERT_EQ(value_int(&v), 16);
}

TEST(Style, normalize_str) {
  value_t v;
  ASSERT_EQ(style_normalize_value("font_name", "sans", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_STRING);
  ASSERT_STREQ(value_str(&v), "sans");
}

TEST(Style, normalize_border_left_right) {
  value_t v;
  uint32_t v32 = 0;
  ASSERT_EQ(style_normalize_value("border", "left|right", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);
  v32 = value_uint32(&v);

  ASSERT_EQ((v32 & BORDER_LEFT) != 0, true);
  ASSERT_EQ((v32 & BORDER_RIGHT) != 0, true);
  ASSERT_EQ((v32 & BORDER_TOP) == 0, true);
  ASSERT_EQ((v32 & BORDER_BOTTOM) == 0, true);
}

TEST(Style, normalize_border_top_bottom) {
  value_t v;
  uint32_t v32 = 0;
  ASSERT_EQ(style_normalize_value("border", "top|bottom", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);
  v32 = value_uint32(&v);

  ASSERT_EQ((v32 & BORDER_LEFT) == 0, true);
  ASSERT_EQ((v32 & BORDER_RIGHT) == 0, true);
  ASSERT_EQ((v32 & BORDER_TOP) != 0, true);
  ASSERT_EQ((v32 & BORDER_BOTTOM) != 0, true);
}

TEST(Style, normalize_border_all) {
  value_t v;
  uint32_t v32 = 0;
  ASSERT_EQ(style_normalize_value("border", "all", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);
  v32 = value_uint32(&v);

  ASSERT_EQ((v32 & BORDER_LEFT) != 0, true);
  ASSERT_EQ((v32 & BORDER_RIGHT) != 0, true);
  ASSERT_EQ((v32 & BORDER_TOP) != 0, true);
  ASSERT_EQ((v32 & BORDER_BOTTOM) != 0, true);
}

TEST(Style, normalize_icon_at_left) {
  value_t v;
  uint32_t v32 = 0;
  ASSERT_EQ(style_normalize_value("icon_at", "left", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);

  v32 = value_uint32(&v);
  ASSERT_EQ((v32 & ICON_AT_LEFT) != 0, true);
}

TEST(Style, normalize_icon_at_right) {
  value_t v;
  uint32_t v32 = 0;
  ASSERT_EQ(style_normalize_value("icon_at", "right", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);

  v32 = value_uint32(&v);
  ASSERT_EQ((v32 & ICON_AT_RIGHT) != 0, true);
}

TEST(Style, normalize_icon_at_top) {
  value_t v;
  uint32_t v32 = 0;
  ASSERT_EQ(style_normalize_value("icon_at", "top", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);

  v32 = value_uint32(&v);
  ASSERT_EQ((v32 & ICON_AT_TOP) != 0, true);
}

TEST(Style, normalize_icon_at_bottom) {
  value_t v;
  uint32_t v32 = 0;
  ASSERT_EQ(style_normalize_value("icon_at", "bottom", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);

  v32 = value_uint32(&v);
  ASSERT_EQ((v32 & ICON_AT_BOTTOM) != 0, true);
}

TEST(Style, normalize_draw_type) {
  value_t v;
  ASSERT_EQ(style_normalize_value("fg_image_draw_type", "center", &v), RET_OK);
  ASSERT_EQ(IMAGE_DRAW_CENTER, value_int(&v));

  ASSERT_EQ(style_normalize_value("fg_image_draw_type", "scale_w", &v), RET_OK);
  ASSERT_EQ(IMAGE_DRAW_SCALE_W, value_int(&v));
}

TEST(Style, normalize_text_align_h) {
  value_t v;
  ASSERT_EQ(style_normalize_value("text_align_h", "center", &v), RET_OK);
  ASSERT_EQ(ALIGN_H_CENTER, value_int(&v));

  ASSERT_EQ(style_normalize_value("text_align_h", "left", &v), RET_OK);
  ASSERT_EQ(ALIGN_H_LEFT, value_int(&v));

  ASSERT_EQ(style_normalize_value("text_align_h", "right", &v), RET_OK);
  ASSERT_EQ(ALIGN_H_RIGHT, value_int(&v));
}

TEST(Style, normalize_text_align_v) {
  value_t v;
  ASSERT_EQ(style_normalize_value("text_align_v", "top", &v), RET_OK);
  ASSERT_EQ(ALIGN_V_TOP, value_int(&v));

  ASSERT_EQ(style_normalize_value("text_align_v", "middle", &v), RET_OK);
  ASSERT_EQ(ALIGN_V_MIDDLE, value_int(&v));

  ASSERT_EQ(style_normalize_value("text_align_v", "bottom", &v), RET_OK);
  ASSERT_EQ(ALIGN_V_BOTTOM, value_int(&v));
}

TEST(Style, normalize_color) {
  value_t v;
  ASSERT_EQ(style_normalize_value("bg_color", "#112233", &v), RET_OK);
  ASSERT_EQ(0xff332211, value_uint32(&v));

  ASSERT_EQ(style_normalize_value("bg_color", "white", &v), RET_OK);
  ASSERT_EQ(0xffffffff, value_uint32(&v));
}
