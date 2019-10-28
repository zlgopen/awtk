#include "gtest/gtest.h"
#include "tkc/mutex.h"
#include "tkc/thread.h"
#include "tkc/platform.h"

static void* thread_entry(void* args) {
  tk_mutex_t* mutex = (tk_mutex_t*)args;

  sleep_ms(100);
  tk_mutex_lock(mutex);

  return NULL;
}

TEST(Mutex, basic) {
  tk_mutex_t* mutex = tk_mutex_create();
  tk_thread_t* thread = tk_thread_create(thread_entry, mutex);

  tk_thread_start(thread);
  ASSERT_EQ(tk_mutex_unlock(mutex), RET_OK);
  tk_thread_join(thread);

  tk_thread_destroy(thread);
  tk_mutex_destroy(mutex);
}
