#include <string>
#include "base/timer.h"
#include "gtest/gtest.h"
#include "base/progress_bar.h"
#include "widget_animators/widget_animator_scale.h"

#include "get_time.inc"

#ifdef WITH_VGCANVAS_LCD
TEST(AnimatorScale, once) {
  timer_manager_t* tm = timer_manager();
  timer_manager_set(timer_manager_create(timer_get_time));

  widget_t* progress_bar = progress_bar_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_scale_create(progress_bar, 1000, 0, EASING_LINEAR);
  widget_animator_scale_set_params(wa, 0, 100, 100, 200);

  timer_set_time(0);
  widget_animator_start(wa);
  timer_dispatch();
  ASSERT_EQ(progress_bar->scale_x, 0);
  ASSERT_EQ(progress_bar->scale_y, 100);

  timer_set_time(500);
  timer_dispatch();
  ASSERT_EQ(progress_bar->scale_x, 50);
  ASSERT_EQ(progress_bar->scale_y, 150);

  timer_set_time(1000);
  timer_dispatch();
  ASSERT_EQ(progress_bar->scale_x, 100);
  ASSERT_EQ(progress_bar->scale_y, 200);

  widget_destroy(progress_bar);
  timer_manager_destroy(timer_manager());
  timer_manager_set(tm);
}
#endif /*WITH_VGCANVAS_LCD*/
