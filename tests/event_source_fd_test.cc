#include "gtest/gtest.h"
#include "tkc/socket_pair.h"
#include "tkc/event_source_fd.h"

static uint32_t s_fd_times = 0;
static ret_t event_source_on_data(event_source_t* source) {
  s_fd_times++;

  return RET_OK;
}

TEST(EventSourceFd, basic) {
  event_source_t* event_source = event_source_fd_create(1, event_source_on_data, NULL);

  ASSERT_EQ(event_source_get_fd(event_source), 1);
  ASSERT_EQ(event_source_check(event_source), RET_OK);
  ASSERT_EQ(event_source_get_wakeup_time(event_source), 0xffff);
  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_fd_times, 1);

  object_unref(OBJECT(event_source));
}
