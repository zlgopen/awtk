#include "gtest/gtest.h"
#include "base/timer.h"
#include "tkc/event_source_timer.h"

static uint64_t s_now = 0;
static uint32_t s_timer_times = 0;
static uint64_t timer_get_time() {
  return s_now;
}

static void timer_set_time(uint32_t now) {
  s_now = now;
}

static ret_t timer_once(const timer_info_t* timer) {
  s_timer_times++;

  return RET_OK;
}

TEST(EventSourceTimer, basic) {
  timer_manager_t* tm = timer_manager_create(timer_get_time);
  timer_manager_add(tm, timer_once, NULL, 100);
  event_source_t* event_source = event_source_timer_create(tm);
  ASSERT_EQ(event_source_get_fd(event_source), -1);
  ASSERT_EQ(event_source_check(event_source), RET_OK);
  ASSERT_EQ(event_source_get_wakeup_time(event_source), 100);
  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_timer_times, 0);

  timer_set_time(100);
  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_timer_times, 1);

  object_unref(OBJECT(event_source));
  timer_manager_destroy(tm);
}
