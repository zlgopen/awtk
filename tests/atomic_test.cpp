#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/atomic.h"

#include "tkc/time_now.h"
#include "base/timer.h"

#define THREAD_NUM 10
#define NR 20000
#define STEP 1

static tk_atomic_t s_atomic;

static void* inc_thread(void* args) {
  uint64_t i = 0;
  value_t v;

  value_set_int(&v, STEP);

  for (i = 0; i < NR; i++) {
    tk_atomic_fetch_add(&s_atomic, &v);
  }

  return NULL;
}

static void* dec_thread(void* args) {
  uint64_t i = 0;
  value_t v;

  value_set_int(&v, STEP);

  for (i = 0; i < NR; i++) {
    tk_atomic_fetch_sub(&s_atomic, &v);
  }

  return NULL;
}

static void* cas_weak_thread(void* args) {
  uint64_t last_value = 0;
  value_t v;

  tk_atomic_load(&s_atomic, &v);
  last_value = value_uint64(&v);

  while (last_value < NR * STEP) {
    value_t tmp;
    if (!tk_atomic_compare_exchange_weak(&s_atomic, value_set_uint64(&tmp, last_value),
                                         value_set_uint64(&v, last_value + STEP))) {
      last_value = value_uint64(&tmp);
    } else {
      last_value = value_uint64(&v);
    }
  }

  return NULL;
}

static void* cas_strong_thread(void* args) {
  uint64_t last_value = 0;
  value_t v;

  tk_atomic_load(&s_atomic, &v);
  last_value = value_uint64(&v);

  while (last_value < NR * STEP) {
    value_t tmp;
    if (!tk_atomic_compare_exchange_strong(&s_atomic, value_set_uint64(&tmp, last_value),
                                           value_set_uint64(&v, last_value + STEP))) {
      last_value = value_uint64(&tmp);
    } else {
      last_value = value_uint64(&v);
    }
  }

  return NULL;
}

int main(int argc, char* argv[]) {
  uint32_t i = 0;
  value_t v;
  tk_thread_t* threads[THREAD_NUM];
  tk_thread_entry_t entrys[] = {
      inc_thread,
      dec_thread,
      cas_weak_thread,
      cas_strong_thread,
  };
  uint64_t entrys_init_value[ARRAY_SIZE(entrys)] = {
      0,
      THREAD_NUM * NR * STEP,
      0,
      0,
  };
  uint64_t entrys_correct[ARRAY_SIZE(entrys)] = {
      THREAD_NUM * NR * STEP,
      0,
      NR * STEP,
      NR * STEP,
  };
  uint64_t entrys_result[ARRAY_SIZE(entrys)] = {0};
  uint64_t entrys_time[ARRAY_SIZE(entrys)] = {0};
  uint64_t total_time = 0;

  platform_prepare();
  timer_prepare(time_now_ms);

  tk_atomic_init(&s_atomic, value_set_uint64(&v, 0));

  for (i = 0; i < ARRAY_SIZE(entrys); i++) {
    uint32_t j = 0;
    value_t tmp;

    log_debug("entrys[%" PRIu32 "] start\n", i);

    tk_atomic_store(&s_atomic, value_set_uint64(&v, entrys_init_value[i]));

    entrys_time[i] = timer_manager()->get_time();

    for (j = 0; j < ARRAY_SIZE(threads); j++) {
      threads[j] = tk_thread_create(entrys[i], tk_pointer_from_int(j));
      tk_thread_start(threads[j]);
    }

    for (j = 0; j < ARRAY_SIZE(threads); j++) {
      tk_thread_join(threads[j]);
      tk_thread_destroy(threads[j]);
      log_debug("%" PRIu32 " stop\n", j);
    }

    entrys_time[i] = timer_manager()->get_time() - entrys_time[i];

    tk_atomic_load(&s_atomic, &tmp);

    entrys_result[i] = value_uint64(&tmp);
    log_debug("entrys[%" PRIu32 "] result %" PRIu64 "\n", i, entrys_result[i]);
  }

  tk_atomic_deinit(&s_atomic);

  log_debug("\n");

  for (i = 0; i < ARRAY_SIZE(entrys); i++) {
    log_debug("entrys[%" PRIu32 "] : time = %" PRIu64 " ms, result = %" PRIu64 ", correct = %s\n",
              i, entrys_time[i], entrys_result[i],
              entrys_result[i] == entrys_correct[i] ? "True" : "False");
    total_time += entrys_time[i];
  }

  log_debug("\ntotal time = %" PRIu64 " ms\n", total_time);

  return 0;
}
