#include "gtest/gtest.h"
#include "tkc/semaphore.h"
#include "tkc/thread.h"
#include "tkc/platform.h"

static void* thread_entry(void* args) {
  tk_semaphore_t* sem = (tk_semaphore_t*)args;

  sleep_ms(100);
  tk_semaphore_post(sem);

  return NULL;
}

TEST(Semaphore, basic) {
  tk_semaphore_t* sem = tk_semaphore_create(0, "hello");
  tk_thread_t* thread = tk_thread_create(thread_entry, sem);

  tk_thread_start(thread);
  ASSERT_EQ(tk_semaphore_wait(sem, 2000), RET_OK);
  tk_thread_join(thread);

  tk_thread_destroy(thread);
  tk_semaphore_destroy(sem);
}
