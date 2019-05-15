#include <string>
#include "base/timer.h"
#include "gtest/gtest.h"
#include "widgets/image.h"
#include "base/widget_animator_manager.h"
#include "widget_animators/widget_animator_rotation.h"

TEST(AnimatorManager, basic) {
  image_t* image = IMAGE(image_create(NULL, 0, 0, 100, 30));
  widget_animator_t* wa = widget_animator_rotation_create(WIDGET(image), 1000, 0, EASING_LINEAR);
  widget_animator_rotation_set_params(wa, 0, 100);
  widget_animator_manager_t* am = widget_animator_manager();
  widget_t* widget = WIDGET(image);

  ASSERT_EQ(widget_animator_set_name(wa, "foo"), RET_OK);
  ;
  ASSERT_EQ(widget_animator_manager_count(am), 1);

  widget_animator_destroy(wa);
  ASSERT_EQ(widget_animator_manager_count(am), 0);

  wa = widget_animator_rotation_create(WIDGET(image), 1000, 0, EASING_LINEAR);
  ASSERT_EQ(widget_animator_set_name(wa, "foo"), RET_OK);
  ;

  ASSERT_EQ(wa->state, ANIMATOR_CREATED);
  ASSERT_EQ(widget_animator_manager_start(am, NULL, NULL), RET_OK);
  ASSERT_EQ(wa->state, ANIMATOR_RUNNING);

  ASSERT_EQ(widget_animator_manager_find(am, widget, "foo") != NULL, TRUE);
  ASSERT_EQ(widget_animator_manager_find(am, widget, "notexist") == NULL, TRUE);

  ASSERT_EQ(widget_animator_manager_pause(am, widget, NULL), RET_OK);
  ASSERT_EQ(wa->state, ANIMATOR_PAUSED);

  ASSERT_EQ(widget_animator_manager_start(am, NULL, NULL), RET_OK);
  ASSERT_EQ(wa->state, ANIMATOR_RUNNING);

  ASSERT_EQ(widget_animator_manager_stop(am, NULL, "foo"), RET_OK);
  ASSERT_EQ(wa->state, ANIMATOR_STOPPED);

  ASSERT_EQ(widget_animator_manager_count(am), 1);
  widget_destroy(WIDGET(image));
  ASSERT_EQ(widget_animator_manager_count(am), 0);
}
