#include "gtest/gtest.h"
#include "widgets/image.h"
#include "widgets/button.h"
#include "widgets/progress_bar.h"
#include "base/widget_animator_factory.h"

#include "widget_animators/widget_animator_move.h"
#include "widget_animators/widget_animator_scale.h"
#include "widget_animators/widget_animator_value.h"
#include "widget_animators/widget_animator_opacity.h"
#include "widget_animators/widget_animator_rotation.h"

#include <string>

using std::string;

TEST(WidgetAnimatorFactory, opacity) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "opacity(from=1, to=200, yoyo_times=1000, duration=2000, delay=1234)");
  widget_animator_prop_t* opacity = (widget_animator_prop_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->yoyo_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(opacity->from, 1);
  ASSERT_EQ(opacity->to, 200);

  widget_animator_destroy(wa);
  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, opacity_default) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);
  b->opacity = 200;
  widget_animator_t* wa = widget_animator_create(b, "opacity(from=12)");
  widget_animator_prop_t* opacity = (widget_animator_prop_t*)wa;
  ASSERT_EQ(opacity->from, 12);
  ASSERT_EQ(opacity->to, 200);
  widget_animator_destroy(wa);

  b->opacity = 200;
  wa = widget_animator_create(b, "opacity(to=12)");
  opacity = (widget_animator_prop_t*)wa;
  ASSERT_EQ(opacity->from, 200);
  ASSERT_EQ(opacity->to, 12);
  widget_animator_destroy(wa);

  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, rotation) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "rotation(from=1, to=200, yoyo_times=1000, duration=2000, delay=1234)");
  widget_animator_prop_t* rotation = (widget_animator_prop_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->yoyo_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(rotation->from, 1);
  ASSERT_EQ(rotation->to, 200);

  widget_animator_destroy(wa);
  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, rotation_default) {
  value_t v;
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  value_set_float(&v, 200);
  widget_set_prop(b, WIDGET_PROP_ROTATION, &v);

  widget_animator_t* wa = widget_animator_create(b, "rotation(from=1)");
  widget_animator_prop_t* rotation = (widget_animator_prop_t*)wa;
  ASSERT_EQ(rotation->from, 1);
  ASSERT_EQ(rotation->to, 200);
  widget_animator_destroy(wa);

  widget_set_prop(b, WIDGET_PROP_ROTATION, &v);
  wa = widget_animator_create(b, "rotation(to=1)");
  rotation = (widget_animator_prop_t*)wa;
  ASSERT_EQ(rotation->to, 1);
  ASSERT_EQ(rotation->from, 200);
  widget_animator_destroy(wa);

  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, value) {
  widget_t* b = progress_bar_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "value(from=1, to=200, yoyo_times=1000, duration=2000, delay=1234)");
  widget_animator_prop_t* value = (widget_animator_prop_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->yoyo_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(value->from, 1);
  ASSERT_EQ(value->to, 200);

  widget_animator_destroy(wa);
  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, value_default) {
  widget_t* b = progress_bar_create(NULL, 10, 20, 100, 30);

  widget_set_value(b, 20);
  widget_animator_t* wa = widget_animator_create(b, "value(from=10)");
  widget_animator_prop_t* value = (widget_animator_prop_t*)wa;
  ASSERT_EQ(value->from, 10);
  ASSERT_EQ(value->to, 20);
  widget_animator_destroy(wa);

  widget_set_value(b, 20);
  wa = widget_animator_create(b, "value(to=10)");
  value = (widget_animator_prop_t*)wa;
  ASSERT_EQ(value->from, 20);
  ASSERT_EQ(value->to, 10);
  widget_animator_destroy(wa);

  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, move) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "move(x_from=1, x_to=200, yoyo_times=1000, duration=2000, delay=1234)");
  widget_animator_prop2_t* move = (widget_animator_prop2_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->yoyo_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(move->from1, 1);
  ASSERT_EQ(move->to1, 200);
  ASSERT_EQ(move->from2, b->y);
  ASSERT_EQ(move->to2, b->y);

  widget_animator_destroy(wa);
  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, move_default) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(b, "move(x_from=1, x_to=200)");
  widget_animator_prop2_t* move = (widget_animator_prop2_t*)wa;
  ASSERT_EQ(move->from1, 1);
  ASSERT_EQ(move->to1, 200);
  ASSERT_EQ(move->from2, b->y);
  ASSERT_EQ(move->to2, b->y);
  widget_animator_destroy(wa);

  wa = widget_animator_create(b, "move(y_from=1, y_to=200)");
  move = (widget_animator_prop2_t*)wa;
  ASSERT_EQ(move->from2, 1);
  ASSERT_EQ(move->to2, 200);
  ASSERT_EQ(move->from1, b->x);
  ASSERT_EQ(move->to1, b->x);
  widget_animator_destroy(wa);

  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, scale) {
  widget_t* b = image_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "scale(x_from=1, x_to=-1, repeat_times=1000, duration=2000, delay=1234)");
  widget_animator_prop2_t* scale = (widget_animator_prop2_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->repeat_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(scale->from1, 1);
  ASSERT_EQ(scale->to1, -1);
  ASSERT_EQ(scale->from2, 1);
  ASSERT_EQ(scale->to2, 1);
  ASSERT_EQ(wa->forever, FALSE);

  widget_animator_destroy(wa);
  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, scale_default) {
  widget_t* b = image_create(NULL, 10, 20, 100, 30);

  image_set_scale(b, 2, 4);
  widget_animator_t* wa = widget_animator_create(b, "scale(x_from=1, x_to=200)");
  widget_animator_prop2_t* scale = (widget_animator_prop2_t*)wa;
  ASSERT_EQ(scale->from1, 1);
  ASSERT_EQ(scale->to1, 200);
  ASSERT_EQ(scale->from2, 4);
  ASSERT_EQ(scale->to2, 4);
  widget_animator_destroy(wa);

  image_set_scale(b, 2, 4);
  wa = widget_animator_create(b, "scale(y_from=1, y_to=200)");
  scale = (widget_animator_prop2_t*)wa;
  ASSERT_EQ(scale->from2, 1);
  ASSERT_EQ(scale->to2, 200);
  ASSERT_EQ(scale->from1, 2);
  ASSERT_EQ(scale->to1, 2);
  widget_animator_destroy(wa);

  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, yoyo_forever) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "scale(x_from=1, x_to=200, yoyo_times=0, duration=2000, delay=1234)");

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->yoyo_times, TK_UINT32_MAX);
  ASSERT_EQ(wa->forever, TRUE);

  widget_animator_destroy(wa);
  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, repeat_forever) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "scale(x_from=1, x_to=200, repeat_times=0, duration=2000, delay=1234)");

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->repeat_times, TK_UINT32_MAX);
  ASSERT_EQ(wa->forever, TRUE);

  widget_animator_destroy(wa);
  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, auto_start) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa =
      widget_animator_create(b, "scale(x_from=1, x_to=200, repeat_times=0, duration=2000)");
  ASSERT_EQ(wa->state, ANIMATOR_RUNNING);
  widget_animator_destroy(wa);

  wa = widget_animator_create(
      b, "scale(x_from=1, x_to=200, repeat_times=0, duration=2000, auto_start=false)");
  ASSERT_EQ(wa->state, ANIMATOR_CREATED);
  widget_animator_destroy(wa);

  wa = widget_animator_create(
      b, "scale(x_from=1, x_to=200, repeat_times=0, duration=2000, auto_start=true)");
  ASSERT_EQ(wa->state, ANIMATOR_RUNNING);
  widget_animator_destroy(wa);

  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, auto_destroy) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa =
      widget_animator_create(b, "scale(x_from=1, x_to=200, repeat_times=0, duration=2000)");
  ASSERT_EQ(wa->destroy_when_done, TRUE);
  widget_animator_destroy(wa);

  wa = widget_animator_create(
      b, "scale(x_from=1, x_to=200, repeat_times=0, duration=2000, auto_destroy=false)");
  ASSERT_EQ(wa->destroy_when_done, FALSE);
  widget_animator_destroy(wa);

  wa = widget_animator_create(
      b, "scale(x_from=1, x_to=200, repeat_times=0, duration=2000, auto_destroy=true)");
  ASSERT_EQ(wa->destroy_when_done, TRUE);
  widget_animator_destroy(wa);

  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, name) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa =
      widget_animator_create(b, "scale(x_from=1, x_to=200, repeat_times=0, duration=2000)");
  ASSERT_EQ(string(wa->name), string("scale"));
  widget_animator_destroy(wa);

  wa = widget_animator_create(
      b, "scale(name=scale_1_200 x_from=1, x_to=200, repeat_times=0, duration=2000)");
  ASSERT_EQ(string(wa->name), string("scale_1_200"));

  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, time_scale) {
  widget_t* b = button_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa =
      widget_animator_create(b, "scale(x_from=1, x_to=200, repeat_times=0, duration=2000)");
  ASSERT_EQ(wa->time_scale, 1);
  widget_animator_destroy(wa);

  wa = widget_animator_create(
      b, "scale(time_scale=1.5 x_from=1, x_to=200, repeat_times=0, duration=2000)");
  ASSERT_EQ(wa->time_scale, 1.5);

  widget_destroy(b);
  idle_dispatch();
}

TEST(WidgetAnimatorFactory, any_prop) {
  widget_t* b = progress_bar_create(NULL, 10, 20, 100, 30);

  widget_animator_t* wa = widget_animator_create(
      b, "any_prop(from=1, to=200, yoyo_times=1000, duration=2000, delay=1234)");
  widget_animator_prop_t* prop = (widget_animator_prop_t*)wa;

  ASSERT_EQ(wa != NULL, true);
  ASSERT_EQ(wa->delay, 1234);
  ASSERT_EQ(wa->yoyo_times, 1000);
  ASSERT_EQ(wa->duration, 2000);
  ASSERT_EQ(prop->from, 1);
  ASSERT_EQ(prop->to, 200);
  ASSERT_EQ(prop->prop_name, string("any_prop"));

  widget_animator_destroy(wa);
  widget_destroy(b);
  idle_dispatch();
}
