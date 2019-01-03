#include <string>
#include "base/timer.h"
#include "gtest/gtest.h"
#include "widgets/progress_bar.h"
#include "widget_animators/widget_animator_value.h"

TEST(AnimatorValue, once) {
  widget_t* progress_bar = progress_bar_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_value_create(progress_bar, 1000, 0, EASING_LINEAR);
  widget_animator_value_set_params(wa, 0, 100);

  widget_animator_start(wa);
  widget_animator_time_elapse(wa, 0);

  ASSERT_EQ(widget_get_value(progress_bar), 0);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(widget_get_value(progress_bar), 50);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(widget_get_value(progress_bar), 100);

  widget_destroy(progress_bar);
}
