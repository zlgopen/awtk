#define WIN32_LEAN_AND_MEAN 1
#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/platform.h"
#include "tkc/action_thread.h"
#include "tkc/object_default.h"

#define NR 10000

static uint32_t exec_times = 0;

static ret_t qaction_dummy_on_event(qaction_t* action, event_t* e) {
  if (e->type == EVT_DONE) {
    tk_object_t* args = TK_OBJECT(*action->args);
    log_debug("done\n");
    TK_OBJECT_UNREF(args);
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

void test(action_thread_t* thread) {
  uint32_t i = 0;

  for (i = 0; i < NR; i++) {
    tk_object_t* args = object_default_create();
    qaction_t* a = qaction_create(qaction_dummy_exec, &args, sizeof(tk_object_t*));
    qaction_set_on_event(a, qaction_dummy_on_event);
    action_thread_exec(thread, a);
  }

  sleep_ms(2000);

  action_thread_destroy(thread);
  log_debug("exec_times=%u\n", exec_times);
}

#include "tkc/platform.h"

static action_thread_t* action_thread_create_demo1(void) {
  action_thread_t* thread = action_thread_create();
  action_thread_set_on_idle(thread, on_idle, NULL);
  action_thread_set_on_quit(thread, on_quit, NULL);

  return thread;
}

static action_thread_t* action_thread_create_demo2(void) {
  action_thread_t* thread =
      action_thread_create_ex("action_thread2", 10 * 1024, TK_THREAD_PRIORITY_NORMAL);
  action_thread_set_on_idle(thread, on_idle, NULL);
  action_thread_set_on_quit(thread, on_quit, NULL);

  return thread;
}

static action_thread_t* action_thread_create_demo3(void) {
  action_thread_t* thread =
      action_thread_create_ex2("action_thread3", 10 * 1024, TK_THREAD_PRIORITY_NORMAL, 500);
  action_thread_set_on_idle(thread, on_idle, NULL);
  action_thread_set_on_quit(thread, on_quit, NULL);

  return thread;
}

int main(int argc, char* argv[]) {
  platform_prepare();

  test(action_thread_create_demo1());
  test(action_thread_create_demo2());
  test(action_thread_create_demo3());

  return 0;
}
