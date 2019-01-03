#include <string>
#include "base/timer.h"
#include "gtest/gtest.h"
#include "widgets/image.h"
#include "widget_animators/widget_animator_rotation.h"

TEST(AnimatorRotation, once) {
  image_t* image = IMAGE(image_create(NULL, 0, 0, 100, 30));
  image_base_t* image_base = IMAGE_BASE(image);
  widget_animator_t* wa = widget_animator_rotation_create(WIDGET(image), 1000, 0, EASING_LINEAR);
  widget_animator_rotation_set_params(wa, 0, 100);

  widget_animator_start(wa);
  widget_animator_time_elapse(wa, 0);
  ASSERT_EQ(image_base->rotation, 0);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(image_base->rotation, 50);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(image_base->rotation, 100);

  widget_destroy(WIDGET(image));
}
