#include "base/slider.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(slider, basic) {
  value_t v1;
  value_t v2;
  widget_t* s = slider_create(NULL, 10, 20, 30, 40);

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(s, "value", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, "value", &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 5);
  ASSERT_EQ(widget_set_prop(s, "min", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, "min", &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 50);
  ASSERT_EQ(widget_set_prop(s, "max", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, "max", &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 5);
  ASSERT_EQ(widget_set_prop(s, "step", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, "step", &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(s, "vertical", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, "vertical", &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  widget_destroy(s);
}

static ret_t on_event(void* ctx, event_t* e) {
  if (e->type == EVT_VALUE_CHANGED) {
    *(int*)ctx = *(int*)ctx + 1;
  }

  return RET_OK;
}

TEST(slider, event) {
  int count = 0;
  widget_t* s = slider_create(NULL, 10, 20, 30, 40);

  widget_on(s, EVT_VALUE_CHANGED, on_event, (void*)&count);

  ASSERT_EQ(count, 0);
  ASSERT_EQ(slider_set_value(s, 1), RET_OK);
  ASSERT_EQ(count, 1);

  ASSERT_EQ(slider_set_value(s, 2), RET_OK);
  ASSERT_EQ(count, 2);

  widget_destroy(s);
}
