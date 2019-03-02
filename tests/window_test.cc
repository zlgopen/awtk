#include "widgets/window.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(Window, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = window_create(NULL, 10, 20, 30, 40);

  value_set_wstr(&v1, L"window");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  value_set_str(&v1, "bottom_to_top");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ANIM_HINT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v2), value_str(&v1)), 0);

  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_OPEN_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v2), value_str(&v1)), 0);

  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_CLOSE_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v2), value_str(&v1)), 0);

  value_set_str(&v1, "center_scale");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_OPEN_ANIM_HINT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_OPEN_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v2), value_str(&v1)), 0);

  value_set_str(&v1, "center_scale");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_CLOSE_ANIM_HINT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_CLOSE_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v2), value_str(&v1)), 0);

  value_set_str(&v1, "main");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_THEME, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_THEME, &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v2), value_str(&v1)), 0);

  widget_destroy(w);
}

TEST(Window, cast) {
  widget_t* w = window_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(w, window_cast(w));
  ASSERT_EQ(w, window_base_cast(w));
  widget_destroy(w);
}
