#include "scroll_view/scroll_bar.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include <string>
#include "ui_loader/ui_serializer.h"

using std::string;

TEST(ScrollBar, basic) {
  value_t v1;
  value_t v2;
  widget_t* w1 = NULL;
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

  w1 = widget_clone(w, NULL);
  ASSERT_EQ(widget_equal(w, w1), TRUE);

  widget_destroy(w);
  widget_destroy(w1);
}

TEST(ScrollBar, to_xml) {
  str_t str;
  widget_t* w = scroll_bar_create(NULL, 10, 20, 30, 400);
  str_init(&str, 1024);

  ASSERT_EQ(widget_to_xml(w, &str), RET_OK);
  ASSERT_EQ(string(str.str), string("<?xml version='1.0' encoding='UTF-8' standalone='yes' "
                                    "?>\r\n<scroll_bar_d x=\"10\" y=\"20\" w=\"30\" h=\"400\" "
                                    "animatable=\"true\">\n</scroll_bar_d>\n"));

  str_reset(&str);
  widget_destroy(w);
}

TEST(ScrollBar, cast) {
  widget_t* w = scroll_bar_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, scroll_bar_cast(w));

  widget_destroy(w);
}

TEST(ScrollBar, auto_hide) {
  widget_t* w = scroll_bar_create_mobile(NULL, 10, 20, 30, 40);

  ASSERT_EQ(SCROLL_BAR(w)->auto_hide, TRUE);
  scroll_bar_set_auto_hide(w, FALSE);
  ASSERT_EQ(SCROLL_BAR(w)->auto_hide, FALSE);

  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_AUTO_HIDE, TRUE), RET_OK);
  ASSERT_EQ(SCROLL_BAR(w)->auto_hide, TRUE);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_HIDE, FALSE), TRUE);

  widget_destroy(w);
}
