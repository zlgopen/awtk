#include "tkc/utils.h"
#include "base/idle.h"
#include "gtest/gtest.h"

#include <string>

using std::string;
static string s_log;

static ret_t on_idle(const idle_info_t* idle) {
  s_log += "o:";
  return RET_OK;
}

static ret_t on_idle_repeat(const idle_info_t* idle) {
  s_log += "r:";
  return RET_REPEAT;
}

static ret_t on_idle_remove(const idle_info_t* idle) {
  uint32_t id = (char*)(idle->ctx) - (char*)NULL;

  idle_remove(id);
  s_log += "remove:";

  return RET_OK;
}

TEST(Idle, basic) {
  uint32_t i = 0;
  uint32_t nr = 10;

  idle_manager_remove_all(idle_manager());

  for (i = 0; i < nr; i++) {
    ASSERT_EQ(idle_add(on_idle, tk_pointer_from_int(i)) > 0, true);
    ASSERT_EQ(idle_exist(on_idle, tk_pointer_from_int(i)) > 0, true);
    ASSERT_EQ(idle_count(), i + 1);
  }

  s_log = "";
  ASSERT_EQ(idle_dispatch(), RET_OK);
  ASSERT_EQ(idle_count(), 0);
  ASSERT_EQ(s_log, "o:o:o:o:o:o:o:o:o:o:");

  for (i = 0; i < nr; i++) {
    uint32_t id = idle_add(on_idle, tk_pointer_from_int(i));
    ASSERT_EQ(idle_exist(on_idle, tk_pointer_from_int(i)) > 0, true);
    ASSERT_EQ(id > 0, true);
    ASSERT_EQ(idle_remove(id), RET_OK);
    ASSERT_EQ(idle_count(), 0);
  }
}

TEST(Idle, repeat) {
  uint32_t i = 0;
  uint32_t nr = 10;

  idle_manager_remove_all(idle_manager());

  for (i = 0; i < nr; i++) {
    ASSERT_EQ(idle_add(on_idle_repeat, NULL) > 0, true);
    ASSERT_EQ(idle_count(), i + 1);
  }

  s_log = "";
  ASSERT_EQ(idle_dispatch(), RET_OK);
  ASSERT_EQ(idle_count(), nr);
  ASSERT_EQ(s_log, "r:r:r:r:r:r:r:r:r:r:");

  s_log = "";
  ASSERT_EQ(idle_dispatch(), RET_OK);
  ASSERT_EQ(idle_count(), nr);
  ASSERT_EQ(s_log, "r:r:r:r:r:r:r:r:r:r:");

  idle_manager_remove_all(idle_manager());
}

TEST(Idle, idle_remove) {
  uint32_t id = 0;
  idle_manager_remove_all(idle_manager());

  id = idle_add(on_idle_repeat, NULL);
  idle_add(on_idle_remove, ((char*)NULL) + id);
  ASSERT_EQ(idle_count(), 2);

  s_log = "";
  ASSERT_EQ(idle_dispatch(), RET_OK);
  ASSERT_EQ(idle_count(), 0);
  ASSERT_EQ(s_log, "r:remove:");

  idle_manager_remove_all(idle_manager());
}
