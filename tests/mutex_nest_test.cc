#include "gtest/gtest.h"
#include "tkc/mutex_nest.h"
#include "tkc/thread.h"
#include "tkc/platform.h"

TEST(MutexNest, basic) {
  uint32_t i = 0;
  tk_mutex_nest_t* mutex_nest = tk_mutex_nest_create();

  for (i = 0; i < 100; i++) {
    ASSERT_EQ(mutex_nest->ref, i);
    ASSERT_EQ(tk_mutex_nest_lock(mutex_nest), RET_OK);
    ASSERT_EQ(mutex_nest->owner, tk_thread_self());
  }

  for (i = 0; i < 100; i++) {
    ASSERT_EQ(mutex_nest->ref, 100 - i);
    ASSERT_EQ(mutex_nest->owner, tk_thread_self());
    ASSERT_EQ(tk_mutex_nest_unlock(mutex_nest), RET_OK);
  }
  ASSERT_EQ(mutex_nest->ref, 0);
  ASSERT_EQ(mutex_nest->owner, 0);

  tk_mutex_nest_destroy(mutex_nest);
}
