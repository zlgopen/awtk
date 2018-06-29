#include "base/idle.h"
#include "gtest/gtest.h"

#include <string>

using std::string;
static string s_log;

static ret_t on_idle(const idle_info_t* idle) {
  s_log += "o:";
  return RET_OK;
}

TEST(Idle, basic) {
  uint32_t i = 0;
  uint32_t nr = 10;

  idle_manager_remove_all(idle_manager());

  for (i = 0; i < nr; i++) {
    ASSERT_EQ(idle_add(on_idle, NULL) > 0, true);
    ASSERT_EQ(idle_count(), i + 1);
  }

  ASSERT_EQ(idle_dispatch(), RET_OK);
  ASSERT_EQ(idle_count(), 0);
  ASSERT_EQ(s_log, "o:o:o:o:o:o:o:o:o:o:");

  for (i = 0; i < nr; i++) {
    uint32_t id = idle_add(on_idle, NULL);
    ASSERT_EQ(id > 0, true);
    ASSERT_EQ(idle_remove(id), RET_OK);
    ASSERT_EQ(idle_count(), 0);
  }
}
