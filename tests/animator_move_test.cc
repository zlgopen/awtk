#include <string>
#include "base/timer.h"
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "widget_animators/widget_animator_move.h"

using std::string;

TEST(AnimatorMove, basic) {
  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_move_create(button, 1000, 0, EASING_LINEAR);

  ASSERT_EQ(wa->yoyo_times, 0);
  ASSERT_EQ(widget_animator_set_yoyo(wa, 12), RET_OK);
  ASSERT_EQ(wa->yoyo_times, 12);

  ASSERT_EQ(wa->repeat_times, 0);
  ASSERT_EQ(widget_animator_set_repeat(wa, 10), RET_OK);
  ASSERT_EQ(wa->repeat_times, 10);

  ASSERT_EQ(wa->reversed, FALSE);
  ASSERT_EQ(widget_animator_set_reversed(wa, TRUE), RET_OK);
  ASSERT_EQ(wa->reversed, TRUE);

  widget_animator_destroy(wa);
  widget_destroy(button);
}

TEST(AnimatorMove, state) {
  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_move_create(button, 1000, 0, EASING_LINEAR);

  ASSERT_EQ(wa->repeat_times, 0);
  ASSERT_EQ(widget_animator_set_repeat(wa, 10), RET_OK);
  ASSERT_EQ(wa->repeat_times, 10);

  ASSERT_EQ(widget_animator_start(wa), RET_OK);
  ASSERT_EQ(wa->state, ANIMATOR_RUNNING);

  ASSERT_EQ(widget_animator_pause(wa), RET_OK);
  ASSERT_EQ(wa->state, ANIMATOR_PAUSED);

  ASSERT_EQ(widget_animator_start(wa), RET_OK);
  ASSERT_EQ(wa->state, ANIMATOR_RUNNING);

  ASSERT_EQ(widget_animator_stop(wa), RET_OK);
  ASSERT_EQ(wa->state, ANIMATOR_STOPPED);

  widget_animator_destroy(wa);
  widget_destroy(button);
}

static string s_log;
static ret_t on_animator_event(void* ctx, event_t* e) {
  switch (e->type) {
    case EVT_ANIM_START: {
      s_log += "start:";
      break;
    }
    case EVT_ANIM_STOP: {
      s_log += "stop:";
      break;
    }
    case EVT_ANIM_ONCE: {
      s_log += "once:";
      break;
    }
    case EVT_ANIM_END: {
      s_log += "end:";
      break;
    }
  }

  return RET_OK;
}

TEST(AnimatorMove, once) {
  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_move_create(button, 1000, 0, EASING_LINEAR);
  widget_animator_move_set_params(wa, 0, 0, 100, 200);
  widget_animator_on(wa, EVT_ANIM_START, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_ONCE, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_STOP, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_END, on_animator_event, NULL);

  s_log = "";
  widget_animator_start(wa);
  widget_animator_time_elapse(wa, 0);

  ASSERT_EQ(s_log, "start:");
  ASSERT_EQ(button->x, 0);
  ASSERT_EQ(button->y, 0);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(button->x, 50);
  ASSERT_EQ(button->y, 100);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(button->x, 100);
  ASSERT_EQ(button->y, 200);
  ASSERT_EQ(s_log, "start:end:");

  widget_destroy(button);
}

TEST(AnimatorMove, time_scale) {
  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_move_create(button, 1000, 0, EASING_LINEAR);
  widget_animator_move_set_params(wa, 0, 0, 100, 200);
  widget_animator_on(wa, EVT_ANIM_START, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_ONCE, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_STOP, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_END, on_animator_event, NULL);

  s_log = "";
  widget_animator_start(wa);
  widget_animator_set_time_scale(wa, 0.5);
  widget_animator_time_elapse(wa, 0);

  ASSERT_EQ(s_log, "start:");
  ASSERT_EQ(button->x, 0);
  ASSERT_EQ(button->y, 0);

  widget_animator_time_elapse(wa, 1000);
  ASSERT_EQ(button->x, 50);
  ASSERT_EQ(button->y, 100);

  widget_animator_time_elapse(wa, 1000);
  ASSERT_EQ(button->x, 100);
  ASSERT_EQ(button->y, 200);
  ASSERT_EQ(s_log, "start:end:");

  widget_destroy(button);
}

TEST(AnimatorMove, reversed) {
  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_move_create(button, 1000, 0, EASING_LINEAR);
  widget_animator_move_set_params(wa, 0, 0, 100, 200);
  widget_animator_on(wa, EVT_ANIM_START, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_ONCE, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_STOP, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_END, on_animator_event, NULL);
  widget_animator_set_reversed(wa, TRUE);

  s_log = "";
  widget_animator_start(wa);
  widget_animator_time_elapse(wa, 0);

  ASSERT_EQ(s_log, "start:");
  ASSERT_EQ(button->x, 100);
  ASSERT_EQ(button->y, 200);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(button->x, 50);
  ASSERT_EQ(button->y, 100);

  widget_animator_time_elapse(wa, 500);
  ASSERT_EQ(button->x, 0);
  ASSERT_EQ(button->y, 0);
  ASSERT_EQ(s_log, "start:end:");

  widget_destroy(button);
}

TEST(AnimatorMove, repeat) {
  uint32_t i = 0;
  uint32_t nr = 3;

  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_move_create(button, 1000, 0, EASING_LINEAR);
  widget_animator_move_set_params(wa, 0, 0, 100, 200);
  widget_animator_set_repeat(wa, 3);
  widget_animator_on(wa, EVT_ANIM_START, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_ONCE, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_STOP, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_END, on_animator_event, NULL);

  s_log = "";
  string log = "start:";

  widget_animator_start(wa);
  widget_animator_time_elapse(wa, 0);

  for (i = 0; i < nr; i++) {
    widget_animator_time_elapse(wa, 0);
    ASSERT_EQ(button->x, 0);
    ASSERT_EQ(button->y, 0);

    widget_animator_time_elapse(wa, 500);
    ASSERT_EQ(button->x, 50);
    ASSERT_EQ(button->y, 100);

    widget_animator_time_elapse(wa, 500);
    ASSERT_EQ(button->x, 100);
    ASSERT_EQ(button->y, 200);

    log += "once:";
    if (i == 2) {
      log += "end:";
    }
    ASSERT_EQ(s_log, log);
  }

  widget_destroy(button);
}

TEST(AnimatorMove, repeat_forever) {
  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_move_create(button, 1000, 0, EASING_LINEAR);
  widget_animator_move_set_params(wa, 0, 0, 100, 200);
  widget_animator_set_repeat(wa, 0);

  widget_animator_start(wa);
  widget_animator_time_elapse(wa, 0);

  ASSERT_EQ(wa->forever, TRUE);
  ASSERT_EQ(wa->repeat_times, TK_UINT32_MAX);

  widget_animator_time_elapse(wa, 1000);
  ASSERT_EQ(wa->repeat_times, TK_UINT32_MAX - 1);

  wa->repeat_times = 1;
  widget_animator_time_elapse(wa, 1000);
  ASSERT_EQ(wa->repeat_times, TK_UINT32_MAX);

  widget_animator_destroy(wa);
  widget_destroy(button);
}
