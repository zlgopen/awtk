#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/idle.h"
#include "base/main_loop.h"
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
    ASSERT_EQ(idle_count(), i + 1u);
  }

  s_log = "";
  ASSERT_EQ(idle_dispatch(), RET_OK);
  ASSERT_EQ(idle_count(), 0u);
  ASSERT_EQ(s_log, "o:o:o:o:o:o:o:o:o:o:");

  for (i = 0; i < nr; i++) {
    uint32_t id = idle_add(on_idle, tk_pointer_from_int(i));
    ASSERT_EQ(idle_exist(on_idle, tk_pointer_from_int(i)) > 0, true);
    ASSERT_EQ(id > 0, true);
    ASSERT_EQ(idle_remove(id), RET_OK);
    ASSERT_EQ(idle_count(), 0u);
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
  ASSERT_EQ(idle_count(), 2u);

  s_log = "";
  ASSERT_EQ(idle_dispatch(), RET_OK);
  ASSERT_EQ(idle_count(), 0u);
  ASSERT_EQ(s_log, "r:remove:");

  idle_manager_remove_all(idle_manager());
}

TEST(Idle, id) {
  uint32_t id1 = 0;
  uint32_t id2 = 0;
  idle_manager_remove_all(idle_manager());

  id1 = idle_add(on_idle_repeat, NULL);
  idle_manager()->next_idle_id = id1;
  id2 = idle_add(on_idle_repeat, NULL);
  ASSERT_NE(id1, id2);

  idle_manager_remove_all(idle_manager());
}

static ret_t idle_repeat_times(const idle_info_t* info) {
  int32_t* repeat_times = (int32_t*)(info->ctx);
  if (*repeat_times > 0) {
    (*repeat_times)--;
    return RET_REPEAT;
  } else {
    return RET_OK;
  }
}

static ret_t idle_on_destroy(const idle_info_t* info) {
  int32_t** repeat_times = (int32_t**)(info->on_destroy_ctx);
  ENSURE(**repeat_times == 0);
  TKMEM_FREE(*repeat_times);
  return RET_OK;
}

TEST(Idle, queue) {
  int32_t* repeat_times = TKMEM_ZALLOC(int32_t);
  *repeat_times = 3;
  idle_queue_ex(idle_repeat_times, repeat_times, (tk_destroy_t)idle_on_destroy, &repeat_times);
  while (repeat_times != NULL) {
    main_loop_step(main_loop());
    sleep_ms(20);
  }
}
