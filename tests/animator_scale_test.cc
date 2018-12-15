#include <string>
#include "gtest/gtest.h"
#include "widgets/image.h"
#include "widget_animators/widget_animator_scale.h"

TEST(AnimatorScale, once) {
  image_t* image = IMAGE(image_create(NULL, 0, 0, 100, 30));
  image_base_t* image_base = IMAGE_BASE(image);
  widget_animator_t* wa = widget_animator_scale_create(WIDGET(image), 1000, 0, EASING_LINEAR);
  widget_animator_scale_set_params(wa, 0, 100, 100, 200);

  widget_animator_start(wa);
  widget_animator_time_elapse(wa, 0);
  ASSERT_EQ(image_base->scale_x, 0);
  ASSERT_EQ(image_base->scale_y, 100);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(image_base->scale_x, 50);
  ASSERT_EQ(image_base->scale_y, 150);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(image_base->scale_x, 100);
  ASSERT_EQ(image_base->scale_y, 200);

  widget_destroy(WIDGET(image));
}
