#include "widgets/slider.h"
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

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(s, "bar_size", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, "bar_size", &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));
  ASSERT_EQ(SLIDER(s)->bar_size, value_int(&v2));

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(s, "vertical", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(s, "vertical", &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  widget_destroy(s);
}

#include "log_change_events.inc"

TEST(Slider, event) {
  widget_t* w = slider_create(NULL, 0, 0, 100, 100);
  slider_t* slider = SLIDER(w);
  slider_set_value(w, 10);

  s_log = "";
  widget_on(w, EVT_VALUE_WILL_CHANGE, on_change_events, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_change_events, NULL);

  slider_set_value(w, 10);
  ASSERT_EQ(s_log, "");

  slider->dragging = TRUE;
  slider_set_value(w, 20);
  ASSERT_EQ(s_log, "");
  slider->dragging = FALSE;

  widget_destroy(w);
}

TEST(Slider, inputable) {
  widget_t* b = slider_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(b->vt->inputable, TRUE);
  widget_destroy(b);
}

TEST(Slider, cast) {
  widget_t* w = slider_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(w, slider_cast(w));

  widget_destroy(w);
}

TEST(Slider, inc) {
  widget_t* w = slider_create(NULL, 0, 0, 100, 100);
  slider_t* slider = SLIDER(w);

  slider_set_value(w, 0);
  ASSERT_EQ(slider_inc(w), RET_OK);
  ASSERT_EQ(slider_inc(w), RET_OK);
  ASSERT_EQ(slider_inc(w), RET_OK);
  ASSERT_EQ(slider->value, 3);

  slider_set_value(w, 100);
  ASSERT_EQ(slider_inc(w), RET_OK);
  ASSERT_EQ(slider_inc(w), RET_OK);
  ASSERT_EQ(slider_inc(w), RET_OK);
  ASSERT_EQ(slider->value, 100);

  widget_destroy(w);
}

TEST(Slider, inc_step) {
  widget_t* w = slider_create(NULL, 0, 0, 100, 100);
  slider_t* slider = SLIDER(w);

  slider_set_value(w, 0);
  slider_set_step(w, 10);
  ASSERT_EQ(slider_inc(w), RET_OK);
  ASSERT_EQ(slider_inc(w), RET_OK);
  ASSERT_EQ(slider_inc(w), RET_OK);
  ASSERT_EQ(slider->value, 30);

  widget_destroy(w);
}

TEST(Slider, dec) {
  widget_t* w = slider_create(NULL, 0, 0, 100, 100);
  slider_t* slider = SLIDER(w);

  slider_set_value(w, 100);
  ASSERT_EQ(slider_dec(w), RET_OK);
  ASSERT_EQ(slider_dec(w), RET_OK);
  ASSERT_EQ(slider_dec(w), RET_OK);
  ASSERT_EQ(slider->value, 97);

  slider_set_value(w, 0);
  ASSERT_EQ(slider_dec(w), RET_OK);
  ASSERT_EQ(slider_dec(w), RET_OK);
  ASSERT_EQ(slider_dec(w), RET_OK);
  ASSERT_EQ(slider->value, 0);

  widget_destroy(w);
}

TEST(Slider, dec_step) {
  widget_t* w = slider_create(NULL, 0, 0, 100, 100);
  slider_t* slider = SLIDER(w);

  slider_set_value(w, 100);
  slider_set_step(w, 10);
  ASSERT_EQ(slider_dec(w), RET_OK);
  ASSERT_EQ(slider_dec(w), RET_OK);
  ASSERT_EQ(slider_dec(w), RET_OK);
  ASSERT_EQ(slider->value, 70);

  widget_destroy(w);
}

TEST(Slider, inputing) {
  widget_t* w = slider_create(NULL, 0, 0, 100, 100);
  slider_t* slider = SLIDER(w);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_INPUTING, TRUE), FALSE);
  slider->dragging = TRUE;
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_INPUTING, TRUE), TRUE);

  widget_destroy(w);
}
