#define WIN32_LEAN_AND_MEAN 1
#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/platform.h"
#include "tkc/action_thread.h"

#define NR 10000

static uint32_t exec_times = 0;

static ret_t qaction_dummy_on_event(qaction_t* action, event_t* e) {
  if (e->type == EVT_DONE) {
    log_debug("done\n");
    qaction_destroy(action);
  }

  return RET_OK;
}

static ret_t qaction_dummy_exec(qaction_t* action) {
  exec_times++;
  log_debug("exec: exec_times=%u\n", exec_times);
  return RET_OK;
}

static ret_t on_idle(void* ctx, action_thread_t* thread) {
  log_debug("on_idle\n");

  return RET_QUIT;
}

static ret_t on_quit(void* ctx, action_thread_t* thread) {
  log_debug("on_quit\n");

  return RET_OK;
}

void test() {
  uint32_t i = 0;
  action_thread_t* thread1 = action_thread_create();
  action_thread_set_on_idle(thread1, on_idle, NULL);
  action_thread_set_on_quit(thread1, on_quit, NULL);

  for (i = 0; i < NR; i++) {
    qaction_t* a = qaction_create(qaction_dummy_exec, NULL, 0);
    qaction_set_on_event(a, qaction_dummy_on_event);
    action_thread_exec(thread1, a);
  }

  sleep_ms(2000);

  action_thread_destroy(thread1);
  log_debug("exec_times=%u\n", exec_times);
}

#include "tkc/platform.h"

int main(int argc, char* argv[]) {
  platform_prepare();

  test();

  return 0;
}
