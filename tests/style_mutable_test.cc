#include "gtest/gtest.h"
#include "widgets/button.h"
#include "widgets/window.h"
#include "designer_support/style_mutable.h"

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
  int32_t i = 0;
  int32_t k = 0;
  int32_t n = 100;
  color_t fg = color_init(0, 0, 0, 0);
  color_t trans = color_init(0, 0, 0, 0);
  widget_t* w = window_create(NULL, 10, 20, 30, 40);
  widget_t* b = button_create(w, 0, 0, 100, 100);
  style_mutable_t* s = (style_mutable_t*)style_mutable_create(b);
  style_t* style = (style_t*)s;
  const char* state_names[] = {WIDGET_STATE_NORMAL, WIDGET_STATE_PRESSED, WIDGET_STATE_OVER,
                               WIDGET_STATE_DISABLE, NULL};
  for (k = 0; k < 3; k++) {
    char name[32];
    snprintf(name, sizeof(name), "name%d", k);
    ASSERT_EQ(style_mutable_set_name(style, name), RET_OK);
    ASSERT_EQ(string(style_mutable_get_name(style)), string(name));
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
    }
  }

  string str = string(style_mutable_get_name(style)) + string(":");
  style_mutable_foreach(style, on_style_item, &str);
  ASSERT_EQ(str,
            "name2:normal,font_size,100;normal,fg_color,65536;normal,font_name,\"font99\";pressed,"
            "font_size,100;pressed,fg_color,65536;pressed,font_name,\"font99\";over,font_size,100;"
            "over,fg_color,65536;over,font_name,\"font99\";");

  style_destroy(style);
  widget_destroy(w);
}
