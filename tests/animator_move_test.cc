#include <string>
#include "base/timer.h"
#include "gtest/gtest.h"
#include "base/button.h"
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
}

#include "get_time.inc"

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
  timer_manager_t* tm = timer_manager();
  timer_manager_set(timer_manager_create(timer_get_time));

  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_move_create(button, 1000, 0, EASING_LINEAR);
  widget_animator_move_set_params(wa, 0, 0, 100, 200);
  widget_animator_on(wa, EVT_ANIM_START, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_ONCE, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_STOP, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_END, on_animator_event, NULL);

  s_log = "";
  timer_set_time(0);
  widget_animator_start(wa);
  timer_dispatch();

  ASSERT_EQ(s_log, "start:");
  ASSERT_EQ(button->x, 0);
  ASSERT_EQ(button->y, 0);

  timer_set_time(500);
  timer_dispatch();
  ASSERT_EQ(button->x, 50);
  ASSERT_EQ(button->y, 100);

  timer_set_time(1000);
  timer_dispatch();
  ASSERT_EQ(button->x, 100);
  ASSERT_EQ(button->y, 200);
  ASSERT_EQ(s_log, "start:end:");

  widget_destroy(button);
  timer_manager_destroy(timer_manager());
  timer_manager_set(tm);
}

TEST(AnimatorMove, reversed) {
  timer_manager_t* tm = timer_manager();
  timer_manager_set(timer_manager_create(timer_get_time));

  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_move_create(button, 1000, 0, EASING_LINEAR);
  widget_animator_move_set_params(wa, 0, 0, 100, 200);
  widget_animator_on(wa, EVT_ANIM_START, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_ONCE, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_STOP, on_animator_event, NULL);
  widget_animator_on(wa, EVT_ANIM_END, on_animator_event, NULL);
  widget_animator_set_reversed(wa, TRUE);

  s_log = "";
  timer_set_time(0);
  widget_animator_start(wa);
  timer_dispatch();

  ASSERT_EQ(s_log, "start:");
  ASSERT_EQ(button->x, 100);
  ASSERT_EQ(button->y, 200);

  timer_set_time(500);
  timer_dispatch();
  ASSERT_EQ(button->x, 50);
  ASSERT_EQ(button->y, 100);

  timer_set_time(1000);
  timer_dispatch();
  ASSERT_EQ(button->x, 0);
  ASSERT_EQ(button->y, 0);
  ASSERT_EQ(s_log, "start:end:");

  widget_destroy(button);
  timer_manager_destroy(timer_manager());
  timer_manager_set(tm);
}

TEST(AnimatorMove, repeat) {
  uint32_t i = 0;
  uint32_t nr = 3;
  uint32_t now = 0;
  timer_manager_t* tm = timer_manager();
  timer_manager_set(timer_manager_create(timer_get_time));

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

  timer_set_time(now);
  widget_animator_start(wa);

  for (i = 0; i < nr; i++) {
    timer_dispatch();
    ASSERT_EQ(button->x, 0);
    ASSERT_EQ(button->y, 0);

    now += 500;
    timer_set_time(now);
    timer_dispatch();
    ASSERT_EQ(button->x, 50);
    ASSERT_EQ(button->y, 100);

    now += 500;
    timer_set_time(now);
    timer_dispatch();
    ASSERT_EQ(button->x, 100);
    ASSERT_EQ(button->y, 200);

    log += "once:";
    ASSERT_EQ(s_log, log);
  }

  widget_destroy(button);
  timer_manager_destroy(timer_manager());
  timer_manager_set(tm);
}
