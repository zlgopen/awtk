#include "gtest/gtest.h"
#include "base/idle.h"
#include "tkc/event_source_idle.h"

static uint32_t s_idle_times = 0;
static ret_t idle_once(const idle_info_t* idle) {
  s_idle_times++;

  return RET_OK;
}

TEST(EventSourceIdle, basic) {
  idle_manager_t* tm = idle_manager_create();
  event_source_t* event_source = event_source_idle_create(tm);

  ASSERT_EQ(event_source_get_fd(event_source), -1);
  ASSERT_EQ(event_source_check(event_source), RET_OK);
  ASSERT_EQ(event_source_get_wakeup_time(event_source), 0xffff);
  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_idle_times, 0);

  idle_manager_add(tm, idle_once, NULL);
  ASSERT_EQ(event_source_get_wakeup_time(event_source), 0);

  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_idle_times, 1);

  object_unref(OBJECT(event_source));
  idle_manager_destroy(tm);
}
