#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/action_queue.h"
#include "tkc/waitable_ring_buffer.h"

#define NR 100000

static uint32_t exec_times = 0;
static waitable_ring_buffer_t* q;

static ret_t qaction_exec_dummy(qaction_t* req) {
  exec_times++;
  log_debug("exec_times:%u\n", exec_times);
  return RET_OK;
}

static void* consumer(void* args) {
  uint32_t n = 0;
  qaction_t* action = NULL;

  log_debug("consumer start\n");
  while (waitable_ring_buffer_read(q, &action, sizeof(action), 3000) == RET_OK) {
    n++;
    qaction_exec(action);
    qaction_destroy(action);
  }
  log_debug("consumer done\n");

  return NULL;
}

static void* producer(void* args) {
  uint32_t i = 0;
  uint32_t id = tk_pointer_to_int(args);
  log_debug("p=%u start\n", id);
  for (i = 0; i < NR; i++) {
    qaction_t* a = qaction_create(qaction_exec_dummy, NULL, 0);
    if (waitable_ring_buffer_write(q, &a, sizeof(a), 3000) != RET_OK) {
      log_debug("write timeout\n");
      break;
    }
  }
  log_debug("p=%u done\n", id);

  return NULL;
}

void test() {
  tk_thread_t* c = tk_thread_create(consumer, NULL);
  tk_thread_t* p1 = tk_thread_create(producer, tk_pointer_from_int(1));
  tk_thread_t* p2 = tk_thread_create(producer, tk_pointer_from_int(2));
  tk_thread_t* p3 = tk_thread_create(producer, tk_pointer_from_int(3));
  tk_thread_t* p4 = tk_thread_create(producer, tk_pointer_from_int(4));

  q = waitable_ring_buffer_create(9, sizeof(void*));

  tk_thread_start(c);
  tk_thread_start(p1);
  tk_thread_start(p2);
  tk_thread_start(p3);
  tk_thread_start(p4);

  tk_thread_join(c);
  tk_thread_join(p1);
  tk_thread_join(p2);
  tk_thread_join(p3);
  tk_thread_join(p4);

  tk_thread_destroy(c);
  tk_thread_destroy(p1);
  tk_thread_destroy(p2);
  tk_thread_destroy(p3);
  tk_thread_destroy(p4);
  waitable_ring_buffer_destroy(q);

  log_debug("exec_times=%u \n", exec_times);
}

#include "tkc/platform.h"

int main(int argc, char* argv[]) {
  platform_prepare();

  test();

  return 0;
}
