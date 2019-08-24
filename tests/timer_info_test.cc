#include "tkc/timer_info.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

static ret_t on_timer(const timer_info_t* timer) {
  return RET_REPEAT;
}

TEST(TimerInfo, basic) {
  timer_info_t dummy;
  timer_info_t* timer = timer_info_create(NULL, on_timer, NULL, 1000);

  ASSERT_EQ(timer->duration, 1000);
  ASSERT_EQ(timer_info_compare(timer, timer), 0);
  ASSERT_EQ(timer_info_compare(timer, timer_info_init_dummy(&dummy, timer->id)), 0);

  object_unref((object_t*)timer);
}
