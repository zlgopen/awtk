#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/platform.h"
#include "tkc/action_thread_pool.h"

#define NR 10

static uint32_t exec_times = 0;

static ret_t qaction_exec_dummy(qaction_t* req) {
  exec_times++;
  log_debug("exec: exec_times=%u\n", exec_times);
  return RET_OK;
}

void test() {
  uint32_t i = 0;
  qaction_t action;
  qaction_t* a = qaction_init(&action, qaction_exec_dummy, NULL, 0);
  action_thread_pool_t* pool = action_thread_pool_create(100, 10);

  for (i = 0; i < NR; i++) {
    action_thread_pool_exec(pool, a);
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
