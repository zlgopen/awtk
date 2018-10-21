#include "gtest/gtest.h"
#include "base/button.h"
#include "base/widget_animator_factory.h"

#include "widget_animators/widget_animator_move.h"
#include "widget_animators/widget_animator_scale.h"
#include "widget_animators/widget_animator_value.h"
#include "widget_animators/widget_animator_opacity.h"
#include "widget_animators/widget_animator_rotation.h"

TEST(WidgetAnimatorFactory, opacity) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "opacity(from=1, to=200, yoyo_times=1000, duration=2000, delay=1234)");
  widget_animator_opacity_t* opacity = (widget_animator_opacity_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->yoyo_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(opacity->from, 1);
  ASSERT_EQ(opacity->to, 200);

  widget_animator_destroy(wa);
  widget_destroy(b);
}

TEST(WidgetAnimatorFactory, rotation) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "rotation(from=1, to=200, yoyo_times=1000, duration=2000, delay=1234)");
  widget_animator_rotation_t* rotation = (widget_animator_rotation_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->yoyo_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(rotation->from, 1);
  ASSERT_EQ(rotation->to, 200);

  widget_animator_destroy(wa);
  widget_destroy(b);
}

TEST(WidgetAnimatorFactory, value) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "value(from=1, to=200, yoyo_times=1000, duration=2000, delay=1234)");
  widget_animator_value_t* value = (widget_animator_value_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->yoyo_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(value->from, 1);
  ASSERT_EQ(value->to, 200);

  widget_animator_destroy(wa);
  widget_destroy(b);
}

TEST(WidgetAnimatorFactory, move) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "move(x_from=1, x_to=200, yoyo_times=1000, duration=2000, delay=1234)");
  widget_animator_move_t* move = (widget_animator_move_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->yoyo_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(move->x_from, 1);
  ASSERT_EQ(move->x_to, 200);
  ASSERT_EQ(move->y_from, b->y);
  ASSERT_EQ(move->y_to, b->y);

  widget_animator_destroy(wa);
  widget_destroy(b);
}

TEST(WidgetAnimatorFactory, scale) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "scale(x_from=1, x_to=200, repeat_times=1000, duration=2000, delay=1234)");
  widget_animator_scale_t* scale = (widget_animator_scale_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->repeat_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(scale->x_from, 1);
  ASSERT_EQ(scale->x_to, 200);
  ASSERT_EQ(scale->y_from, 1);
  ASSERT_EQ(scale->y_to, 1);

  widget_animator_destroy(wa);
  widget_destroy(b);
}
