#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/async.h"
#include "tkc/platform.h"
#include "tkc/action_thread_pool.h"

static uint32_t fibonacci(uint32_t n) {
  if (n == 0) {
    return 0;
  } else if (n == 1) {
    return 1;
  } else {
    return fibonacci(n - 1) + fibonacci(n - 2);
  }
}

static ret_t fibonacci_async_exec(void* ctx) {
  uint32_t n = (uint32_t)tk_pointer_to_int(ctx);

  fibonacci(n);
  log_debug("thread id=%u n=%u\n", (uint32_t)tk_thread_self(), n);

  return RET_OK;
}

static ret_t fibonacci_async(uint32_t n) {
  return async_call(fibonacci_async_exec, NULL, tk_pointer_from_int(n));
}

#define NR 10000

void test() {
  uint32_t i = 0;

  for (i = 0; i < NR; i++) {
    uint32_t n = i % 30 + 10;
    log_debug("%u %u\n", i, n);
    fibonacci_async(n);
  }
}

#include "tkc/platform.h"

int main(int argc, char* argv[]) {
  platform_prepare();

  async_call_init_ex(10, 2);
  test();
  async_call_deinit();

  return 0;
}
