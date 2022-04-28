#include "base/window.h"
#include "widgets/system_bar.h"
#include "gtest/gtest.h"

TEST(SystemBar, cast) {
  widget_t* w = system_bar_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_is_system_bar(w), TRUE);

  ASSERT_EQ(w, system_bar_cast(w));
  ASSERT_EQ(w, window_base_cast(w));

  window_close_force(w);
}

TEST(SystemBar, copy) {
  widget_t* w = system_bar_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_STRONGLY_FOCUS, TRUE), RET_OK);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_W, 400), RET_OK);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_H, 300), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_THEME, "hello"), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_ANIM_HINT, "popup"), RET_OK);
  widget_t* w1 = widget_clone(w, NULL);

  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_STRONGLY_FOCUS, FALSE), TRUE);
  ASSERT_EQ(widget_get_prop_int(w1, WIDGET_PROP_DESIGN_W, 0), 400);
  ASSERT_EQ(widget_get_prop_int(w1, WIDGET_PROP_DESIGN_H, 0), 300);
  ASSERT_STREQ(widget_get_prop_str(w1, WIDGET_PROP_THEME, NULL), "hello");
  ASSERT_STREQ(widget_get_prop_str(w1, WIDGET_PROP_ANIM_HINT, NULL), "popup");

  widget_destroy(w);
  widget_destroy(w1);
}
