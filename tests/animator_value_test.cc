#include <string>
#include "base/timer.h"
#include "gtest/gtest.h"
#include "base/progress_bar.h"
#include "widget_animators/widget_animator_value.h"

#include "get_time.inc"

TEST(AnimatorValue, once) {
  timer_manager_t* tm = timer_manager();
  timer_manager_set(timer_manager_create(timer_get_time));

  widget_t* progress_bar = progress_bar_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_value_create(progress_bar, 1000, 0, NULL);
  widget_animator_value_set_params(wa, 0, 100);

  timer_set_time(0);
  widget_animator_start(wa);
  timer_dispatch();

  ASSERT_EQ(widget_get_value(progress_bar), 0);

  timer_set_time(500);
  timer_dispatch();
  ASSERT_EQ(widget_get_value(progress_bar), 50);

  timer_set_time(1000);
  timer_dispatch();
  ASSERT_EQ(widget_get_value(progress_bar), 100);

  timer_manager_destroy(timer_manager());
  timer_manager_set(tm);
}

