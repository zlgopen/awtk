#include "gtest/gtest.h"
#include "widgets/button.h"
#include "base/window.h"
#include "base/style_mutable.h"

#include <string>

using std::string;

static ret_t on_style_item(void* ctx, const char* widget_state, const char* id,
                           const value_t* val) {
  string& log = *(string*)ctx;
  char str[128];
  if (val->type == VALUE_TYPE_STRING) {
    snprintf(str, sizeof(str), "%s,%s,\"%s\";", widget_state, id, value_str(val));
  } else {
    snprintf(str, sizeof(str), "%s,%s,%d;", widget_state, id, value_int(val));
  }
  log += str;

  return RET_OK;
}

TEST(StyleMutable, basic) {
  value_t v;
  value_t v1;
  int32_t i = 0;
  int32_t k = 0;
  int32_t n = 100;
  color_t fg = color_init(0, 0, 0, 0);
  color_t trans = color_init(0, 0, 0, 0);
  widget_t* w = window_create(NULL, 10, 20, 30, 40);
  widget_t* b = button_create(w, 0, 0, 100, 100);
  style_mutable_t* s = (style_mutable_t*)style_mutable_create(b, NULL);
  style_t* style = (style_t*)s;
  const char* state_names[] = {WIDGET_STATE_NORMAL, WIDGET_STATE_PRESSED, WIDGET_STATE_OVER,
                               WIDGET_STATE_DISABLE, NULL};
  for (k = 0; k < 3; k++) {
    char name[32];
    snprintf(name, sizeof(name), "name%d", k);
    ASSERT_EQ(style_mutable_set_name(style, name), RET_OK);
    ASSERT_EQ(string(STYLE_MUTABLE(style)->name), string(name));
    widget_set_state(b, (const char*)state_names[k]);
    style_notify_widget_state_changed(style, b);
    const char* state = (const char*)widget_get_prop_str(b, WIDGET_PROP_STATE_FOR_STYLE, 0);
    for (i = 0; i < n; i++) {
      char font_name[32];
      ASSERT_EQ(style_mutable_set_int(style, state, STYLE_ID_FONT_SIZE, i + 1), RET_OK);
      ASSERT_EQ(style_get_int(style, STYLE_ID_FONT_SIZE, 0), i + 1);

      fg.color = 0xffff + 1;
      ASSERT_EQ(style_mutable_set_color(style, state, STYLE_ID_FG_COLOR, fg), RET_OK);
      ASSERT_EQ(style_get_color(style, STYLE_ID_FG_COLOR, trans).color, fg.color);

      snprintf(font_name, sizeof(font_name), "font%d", i);
      ASSERT_EQ(style_mutable_set_str(style, state, STYLE_ID_FONT_NAME, font_name), RET_OK);
      ASSERT_EQ(string(style_get_str(style, STYLE_ID_FONT_NAME, "")), string(font_name));

      snprintf(font_name, sizeof(font_name), "fonta%d", i);
      value_set_str(&v, font_name);
      ASSERT_EQ(style_mutable_set_value(style, state, STYLE_ID_FONT_NAME, &v), RET_OK);

      ASSERT_EQ(string(style_get_str(style, STYLE_ID_FONT_NAME, "")), string(font_name));
      ASSERT_EQ(style_mutable_get_value(style, state, STYLE_ID_FONT_NAME, &v1), RET_OK);
      ASSERT_STREQ(value_str(&v1), font_name);

      snprintf(font_name, sizeof(font_name), "font%d", i);
      ASSERT_EQ(style_mutable_set_str(style, state, STYLE_ID_FONT_NAME, font_name), RET_OK);
      ASSERT_EQ(string(style_get_str(style, STYLE_ID_FONT_NAME, "")), string(font_name));
    }
  }

  string str = string(STYLE_MUTABLE(style)->name) + string(":");
  style_mutable_foreach(style, on_style_item, &str);
  ASSERT_EQ(str,
            "name2:normal,font_size,100;normal,fg_color,65536;normal,font_name,\"font99\";pressed,"
            "font_size,100;pressed,fg_color,65536;pressed,font_name,\"font99\";over,font_size,100;"
            "over,fg_color,65536;over,font_name,\"font99\";");

  style_destroy(style);
  widget_destroy(w);
}

TEST(StyleMutable, cast) {
  widget_t* w = window_create(NULL, 10, 20, 30, 40);
  widget_t* b = button_create(w, 0, 0, 100, 100);
  style_t* style_mutable = style_mutable_create(b, NULL);

  ASSERT_EQ(style_is_mutable(style_mutable), TRUE);
  ASSERT_EQ(style_mutable, style_mutable_cast(style_mutable));

  style_destroy(style_mutable);
  widget_destroy(w);
}

TEST(StyleMutable, copy) {
  color_t c1 = color_init(1, 2, 3, 4);
  color_t c2 = color_init(2, 2, 3, 4);
  style_t* m1 = style_mutable_create(NULL, NULL);
  style_t* m2 = style_mutable_create(NULL, NULL);

  style_mutable_set_int(m1, "normal", "font_size", 123);
  style_mutable_set_str(m1, "normal", "font_name", "foo");
  style_mutable_set_color(m1, "normal", "text_color", c1);

  style_mutable_set_int(m1, "focused", "font_size", 321);
  style_mutable_set_str(m1, "focused", "font_name", "foo2");
  style_mutable_set_color(m1, "focused", "text_color", c2);
  style_mutable_copy(m2, m1);

  string str1;
  style_mutable_foreach(m1, on_style_item, &str1);
  string str2;
  style_mutable_foreach(m2, on_style_item, &str2);

  ASSERT_EQ(str1, str2);
  ASSERT_EQ(
      str1,
      string("normal,font_size,123;normal,font_name,\"foo\";normal,text_color,67305985;focused,"
             "font_size,321;focused,font_name,\"foo2\";focused,text_color,67305986;"));

  style_destroy(m1);
  style_destroy(m2);
}
