#include "gtest/gtest.h"
#include "tkc/socket_pair.h"
#include "tkc/event_source_fd.h"

static uint32_t s_fd_times = 0;
static ret_t event_source_on_data(event_source_t* source) {
  s_fd_times++;

  return RET_OK;
}

TEST(EventSourceFd, basic) {
  s_fd_times = 0;
  event_source_t* event_source = event_source_fd_create(1, event_source_on_data, NULL);

  ASSERT_EQ(event_source_get_fd(event_source), 1);
  ASSERT_EQ(event_source_check(event_source), RET_OK);
  ASSERT_EQ(event_source_get_wakeup_time(event_source), 0xffffu);
  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_fd_times, 1u);

  tk_object_unref(TK_OBJECT(event_source));
}

TEST(EventSourceFd, none_fd) {
  int fd = 2;
  s_fd_times = 0;
  event_source_t* event_source = event_source_fd_create(-1, event_source_on_data, NULL);
  ASSERT_EQ(event_source != NULL, true);
  ASSERT_EQ(event_source_fd_set_fd(event_source, fd), RET_OK);

  ASSERT_EQ(event_source_get_fd(event_source), fd);
  ASSERT_EQ(event_source_check(event_source), RET_OK);
  ASSERT_EQ(event_source_get_wakeup_time(event_source), 0xffffu);
  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_fd_times, 1u);

  ASSERT_EQ(event_source_fd_set_fd(event_source, -1), RET_OK);
  ASSERT_EQ(event_source_get_fd(event_source), -1);
  ASSERT_EQ(event_source_check(event_source), RET_FAIL);
  ASSERT_EQ(s_fd_times, 1u);

  ASSERT_EQ(event_source_fd_set_fd(event_source, fd), RET_OK);
  ASSERT_EQ(event_source_get_fd(event_source), fd);
  ASSERT_EQ(event_source_check(event_source), RET_OK);
  ASSERT_EQ(event_source_get_wakeup_time(event_source), 0xffffu);
  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_fd_times, 2u);

  tk_object_unref(TK_OBJECT(event_source));
}

