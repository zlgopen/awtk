#include "tkc/thread.h"
#include "tkc/cond_var.h"
#include "tkc/platform.h"

#include "gtest/gtest.h"
#include <stdlib.h>

static int32_t s_max = 10;
static int32_t s_producer = 0;
static int32_t s_consumer = 0;

static void* consumer(void* args) {
  tk_cond_var_t* cond = (tk_cond_var_t*)args;

  while (s_consumer < s_max) {
    tk_cond_var_wait(cond, 10000);
    sleep_ms(100);
    s_consumer++;
  }

  return NULL;
}

static void* producer(void* args) {
  tk_cond_var_t* cond = (tk_cond_var_t*)args;

  while (s_producer < s_max) {
    tk_cond_var_awake(cond);
    sleep_ms(100);
    s_producer++;
  }

  return NULL;
}

TEST(CondVar, basic) {
  tk_cond_var_t* cond = tk_cond_var_create();
  tk_thread_t* c = tk_thread_create(consumer, cond);
  tk_thread_t* p = tk_thread_create(producer, cond);
  tk_thread_start(c);
  tk_thread_start(p);

  tk_thread_join(c);
  tk_thread_join(p);
  tk_thread_destroy(c);
  tk_thread_destroy(p);
  tk_cond_var_destroy(cond);
  ASSERT_EQ(s_producer, s_max);
  ASSERT_EQ(s_consumer, s_max);
}
