#include "gtest/gtest.h"
#include "tkc/mutex.h"
#include "tkc/thread.h"
#include "tkc/platform.h"

TEST(Mutex, basic) {
  tk_mutex_t* mutex = tk_mutex_create();

  ASSERT_EQ(tk_mutex_lock(mutex), RET_OK);
  ASSERT_EQ(tk_mutex_unlock(mutex), RET_OK);

  tk_mutex_destroy(mutex);
}
