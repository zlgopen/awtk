#include "base/canvas.h"
#include "base/widget.h"
#include "base/layout.h"
#include "switch/switch.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(Switch, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = switch_create(NULL, 10, 20, 30, 40);

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_VALUE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_VALUE, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_VALUE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_VALUE, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_float(&v1, 0.4);
  ASSERT_EQ(widget_set_prop(w, SWITCH_PROP_MAX_XOFFSET_RATIO, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, SWITCH_PROP_MAX_XOFFSET_RATIO, &v2), RET_OK);
  ASSERT_EQ(value_float(&v1), value_float(&v2));

  widget_destroy(w);
}

TEST(Switch, clone) {
  value_t v1;

  widget_t* w1 = switch_create(NULL, 10, 20, 30, 40);

  value_set_int(&v1, 200);
  ASSERT_EQ(widget_set_prop(w1, WIDGET_PROP_REPEAT, &v1), RET_OK);
  widget_set_self_layout_params(w1, "1", "2", "3", "4");
  widget_set_children_layout(w1, "default(r=0, c=0, x=10, y=10, s=10)");
  ASSERT_EQ(switch_cast(w1), w1);

  widget_t* w2 = widget_clone(w1, NULL);
  ASSERT_EQ(widget_equal(w1, w2), TRUE);
  widget_destroy(w1);
  widget_destroy(w2);
}

#include "log_change_events.inc"

TEST(Switch, event) {
  widget_t* w = switch_create(NULL, 0, 0, 100, 100);

  switch_set_value(w, TRUE);

  s_log = "";
  widget_on(w, EVT_VALUE_WILL_CHANGE, on_change_events, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_change_events, NULL);

  switch_set_value(w, TRUE);
  ASSERT_EQ(s_log, "");

  switch_set_value(w, FALSE);
  ASSERT_EQ(s_log, "will_change;change;");

  widget_destroy(w);
}

TEST(Switch, cast) {
  widget_t* w = switch_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(w, switch_cast(w));

  widget_destroy(w);
}
