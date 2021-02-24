#include "tkc/timer_info.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

static ret_t on_timer(const timer_info_t* timer) {
  return RET_REPEAT;
}

TEST(TimerInfo, basic) {
  timer_info_t dummy;
  timer_info_t* timer = timer_info_create(NULL, on_timer, NULL, 1000, TIMER_INFO_NORMAL);

  ASSERT_EQ(timer->duration, 1000);
  ASSERT_EQ(timer_info_compare_by_id(timer, timer), 0);
  ASSERT_EQ(timer_info_compare_by_id(timer, timer_info_init_dummy(&dummy, timer->id)), 0);

  ASSERT_EQ(timer_info_compare_by_ctx(timer, NULL), 0);

  ASSERT_EQ(timer_info_compare_by_ctx_and_type(timer, timer), 0);
  ASSERT_EQ(
      timer_info_compare_by_ctx_and_type(timer, timer_info_init_dummy_with_ctx_and_type(
                                                    &dummy, timer->timer_info_type, timer->ctx)),
      0);

  object_unref((object_t*)timer);
}
