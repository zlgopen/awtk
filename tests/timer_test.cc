#include "base/timer.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

static uint32_t now = 0;
static uint32_t timer_get_time() { return now; }

static string s_log;
static ret_t timer_once(const timer_info_t* timer) {
  s_log += "o:";
  return RET_OK;
}

static ret_t timer_repeat(const timer_info_t* timer) {
  s_log += "r:";
  return RET_REPEAT;
}

TEST(Timer, once) {
  uint32_t i = 0;
  uint32_t nr = 10;
  timer_init(timer_get_time);

  now = 0;
  for (i = 0; i < nr; i++) {
    ASSERT_EQ(timer_add(timer_once, NULL, 100) > 0, true);
    ASSERT_EQ(timer_count(), i + 1);
  }
  ASSERT_EQ(timer_check(), RET_OK);
  ASSERT_EQ(timer_count(), nr);
  ASSERT_EQ(s_log, "");

  now = 100;

  ASSERT_EQ(timer_check(), RET_OK);
  ASSERT_EQ(timer_count(), 0);
  ASSERT_EQ(s_log, "o:o:o:o:o:o:o:o:o:o:");
}

TEST(Timer, repeat) {
  uint32_t i = 0;
  uint32_t nr = 10;
  timer_init(timer_get_time);
  uint32_t ids[10];

  now = 0;
  s_log = "";
  for (i = 0; i < nr; i++) {
    ids[i] = timer_add(timer_repeat, NULL, 100);
    ASSERT_EQ(ids[i] > 0, true);
    ASSERT_EQ(timer_count(), i + 1);
  }
  ASSERT_EQ(timer_check(), RET_OK);
  ASSERT_EQ(timer_count(), nr);
  ASSERT_EQ(s_log, "");

  now = 100;
  s_log = "";
  ASSERT_EQ(timer_check(), RET_OK);
  ASSERT_EQ(timer_count(), nr);
  ASSERT_EQ(s_log, "r:r:r:r:r:r:r:r:r:r:");

  for (i = 0; i < nr; i++) {
    uint32_t id = ids[i];
    ASSERT_EQ(timer_remove(id), RET_OK);
    ASSERT_EQ(timer_count(), nr - i - 1);
  }
  ASSERT_EQ(timer_count(), 0);
}

