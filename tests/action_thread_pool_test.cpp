#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/platform.h"
#include "tkc/action_thread_pool.h"

#include <atomic>

#define NR 10000 * 100

static std::atomic<int> exec_times;

static ret_t qaction_dummy_on_event(qaction_t* action, event_t* e) {
  if (e->type == EVT_DONE) {
    log_debug("done\n");
    qaction_destroy(action);
  }

  return RET_OK;
}

static ret_t qaction_dummy_exec(qaction_t* action) {
  exec_times++;
  log_debug("exec: exec_times=%d\n", exec_times.load());
  return RET_OK;
}

void test() {
  uint32_t i = 0;
  action_thread_pool_t* pool = action_thread_pool_create(10, 2);

  for (i = 0; i < NR; i++) {
    qaction_t* a = qaction_create(qaction_dummy_exec, NULL, 0);
    qaction_set_on_event(a, qaction_dummy_on_event);
    action_thread_pool_exec(pool, a);
  }

  sleep_ms(2000);

  action_thread_pool_destroy(pool);
  log_debug("exec_times=%d\n", exec_times.load());
}

#include "tkc/platform.h"

int main(int argc, char* argv[]) {
  platform_prepare();

  test();

  return 0;
}
