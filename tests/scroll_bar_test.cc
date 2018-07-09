#include "base/scroll_bar.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(ScrollBar, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = scroll_bar_create(NULL, 10, 20, 30, 400);

  value_set_int(&v1, 2000);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 20);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ROW, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ROW, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 50);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_VALUE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_VALUE, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ANIMATABLE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ANIMATABLE, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  widget_destroy(w);
}
