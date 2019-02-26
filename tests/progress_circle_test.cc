#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include "progress_circle/progress_circle.h"

TEST(ProgressCircle, basic) {
  value_t v1;
  value_t v2;
  widget_t* s = progress_circle_create(NULL, 10, 20, 30, 40);

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(s, WIDGET_PROP_VALUE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, WIDGET_PROP_VALUE, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 1000);
  ASSERT_EQ(widget_set_prop(s, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 90);
  ASSERT_EQ(widget_set_prop(s, PROGRESS_CIRCLE_PROP_START_ANGLE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, PROGRESS_CIRCLE_PROP_START_ANGLE, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 9);
  ASSERT_EQ(widget_set_prop(s, PROGRESS_CIRCLE_PROP_LINE_WIDTH, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, PROGRESS_CIRCLE_PROP_LINE_WIDTH, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(s, WIDGET_PROP_SHOW_TEXT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, WIDGET_PROP_SHOW_TEXT, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(s, PROGRESS_CIRCLE_PROP_COUNTER_CLOCK_WISE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, PROGRESS_CIRCLE_PROP_COUNTER_CLOCK_WISE, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_str(&v1, "C");
  ASSERT_EQ(widget_set_prop(s, PROGRESS_CIRCLE_PROP_UNIT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, PROGRESS_CIRCLE_PROP_UNIT, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));

  widget_destroy(s);
}

#include "log_change_events.inc"

TEST(ProgressCircle, event) {
  widget_t* w = progress_circle_create(NULL, 0, 0, 100, 100);

  progress_circle_set_value(w, 10);

  s_log = "";
  widget_on(w, EVT_VALUE_WILL_CHANGE, on_change_events, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_change_events, NULL);

  progress_circle_set_value(w, 10);
  ASSERT_EQ(s_log, "");

  progress_circle_set_value(w, 20);
  ASSERT_EQ(s_log, "will_change;change;");

  widget_destroy(w);
}

TEST(ProgressCircle, cast) {
  widget_t* w = progress_circle_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, progress_circle_cast(w));

  widget_destroy(w);
}
