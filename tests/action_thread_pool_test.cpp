#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/platform.h"
#include "tkc/action_thread_pool.h"

#define NR 10

static uint32_t exec_times = 0;

static ret_t qaction_dummy_on_event(qaction_t* action, event_t* e) {
  if(e->type == EVT_DONE) {
    log_debug("done\n");
  }

  return RET_OK;
}

static ret_t qaction_dummy_exec(qaction_t* action) {
  exec_times++;
  log_debug("exec: exec_times=%u\n", exec_times);
  return RET_OK;
}

void test() {
  uint32_t i = 0;
  qaction_t action;
  action_thread_t* thread1 = NULL;
  action_thread_t* thread2 = NULL;
  action_thread_t* thread3 = NULL;

  qaction_t* a = qaction_init(&action, qaction_dummy_exec, NULL, 0);
  action_thread_pool_t* pool = action_thread_pool_create(100, 10);
  qaction_set_on_event(a, qaction_dummy_on_event, NULL);

  thread1 = action_thread_pool_get(pool);
  thread2 = action_thread_pool_get(pool);
  thread3 = action_thread_pool_get(pool);

  for (i = 0; i < NR; i++) {
    action_thread_exec(thread1, a);
  }

  for (i = 0; i < NR; i++) {
    action_thread_exec(thread2, a);
  }
  
  for (i = 0; i < NR; i++) {
    action_thread_exec(thread3, a);
  }
  sleep_ms(200);
  action_thread_pool_destroy(pool);
  log_debug("exec_times=%u\n", exec_times);
}

#include "tkc/platform.h"

int main(int argc, char* argv[]) {
  platform_prepare();

  test();

  return 0;
}
