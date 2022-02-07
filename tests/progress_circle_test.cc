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

  value_set_str(&v1, "%02.2f");
  ASSERT_EQ(widget_set_prop(s, WIDGET_PROP_FORMAT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, WIDGET_PROP_FORMAT, &v2), RET_OK);
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

TEST(ProgressCircle, change_value) {
  widget_t* w = progress_circle_create(NULL, 10, 20, 30, 40);
  value_change_event_t evt;
  memset(&evt, 0x00, sizeof(evt));

  widget_on(w, EVT_VALUE_WILL_CHANGE, on_value_will_changed_accept, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_value_changed, &evt);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 3);

  ASSERT_EQ(value_int(&(evt.old_value)), 0);
  ASSERT_EQ(value_int(&(evt.new_value)), 3);

  widget_destroy(w);
}

TEST(ProgressCircle, change_value_abort) {
  widget_t* w = progress_circle_create(NULL, 10, 20, 30, 40);
  value_change_event_t evt;
  memset(&evt, 0x00, sizeof(evt));

  widget_on(w, EVT_VALUE_WILL_CHANGE, on_value_will_changed_abort, NULL);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 3), 0);

  widget_destroy(w);
}

static void ASSERT_RECT(rect_t r, int32_t x, int32_t y, int32_t w, int32_t h) {
  log_debug("r(%d,%d,%d,%d)(%d,%d,%d,%d)\n", r.x, r.y, r.w, r.h, x, y, w, h);
  ASSERT_EQ(r.x, x);
  ASSERT_EQ(r.y, y);
  ASSERT_EQ(r.w, w);
  ASSERT_EQ(r.h, h);
}

TEST(ProgressCircle, dirty_rect) {
  rect_t r;
  widget_t* w = progress_circle_create(NULL, 10, 20, 300, 400);

  r = progress_circle_calc_line_dirty_rect(w, 0, 1);
  ASSERT_RECT(r, 145, 49, 19, 10);

  r = progress_circle_calc_line_dirty_rect(w, 0, 10);
  ASSERT_RECT(r, 145, 49, 96, 38);

  r = progress_circle_calc_line_dirty_rect(w, 10, 11);
  ASSERT_RECT(r, 231, 77, 17, 16);

  r = progress_circle_calc_line_dirty_rect(w, 25, 26);
  ASSERT_RECT(r, 291, 195, 10, 19);

  r = progress_circle_calc_line_dirty_rect(w, 15, 35);
  ASSERT_RECT(r, 263, 109, 33, 182);

  r = progress_circle_calc_line_dirty_rect(w, 35, 36);
  ASSERT_RECT(r, 257, 281, 16, 17);

  r = progress_circle_calc_line_dirty_rect(w, 35, 55);
  ASSERT_RECT(r, 100, 281, 173, 65);

  r = progress_circle_calc_line_dirty_rect(w, 55, 56);
  ASSERT_RECT(r, 91, 331, 19, 13);

  r = progress_circle_calc_line_dirty_rect(w, 65, 85);
  ASSERT_RECT(r, 4, 109, 33, 182);

  r = progress_circle_calc_line_dirty_rect(w, 85, 86);
  ASSERT_RECT(r, 27, 102, 16, 17);

  r = progress_circle_calc_line_dirty_rect(w, 85, 100);
  ASSERT_RECT(r, 27, 49, 128, 70);

  widget_destroy(w);
}
