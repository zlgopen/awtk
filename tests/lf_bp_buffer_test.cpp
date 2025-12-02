#include "tkc/thread.h"
#include "tkc/lf_bp_buffer.h"

#include "tkc/str.h"

#include "tkc/time_now.h"
#include "base/timer.h"

#define TEST_BUF_SIZE (20 * 1024 * 1024)
#define BUF_WRITE_MAX_SIZE (16 * 1024)
#define CACHE_SIZE (64 * 1024)

static char* generate_random_text(size_t size) {
  char* text = (char*)TKMEM_ALLOC(size + 1);  // +1 保存字符串结束符
  return_value_if_fail(text != NULL, NULL);

  static bool_t s_initialized = FALSE;
  if (!s_initialized) {
    srand((unsigned int)time(NULL));
    s_initialized = TRUE;
  }

  // 生成可打印ASCII字符 (32-126)
  for (size_t i = 0; i < size; i++) {
    text[i] = (char)(rand() % 95 + 32);  // ASCII 32-126
  }
  text[size] = '\0';

  return text;
}

static tk_lf_bp_buffer_t s_buffer;
static char* s_text = NULL;

static void* thread_entry(void* args) {
  tk_atomic_t* stop = (tk_atomic_t*)args;
  uint64_t offset = 0;
  value_t v;
  uint64_t thread_id = tk_thread_self();
  bool_t failed_times = 0;

  while (offset < TEST_BUF_SIZE) {
    uint64_t size = tk_min(TEST_BUF_SIZE - offset, BUF_WRITE_MAX_SIZE);
    if (RET_OK == tk_lf_bp_buffer_write_simple(&s_buffer, (uint8_t*)s_text + offset, &size, TRUE)) {
      failed_times = 0;
      offset += size;
    } else {
      failed_times++;
      if (failed_times >= 3) {
        sleep_us(tk_min(failed_times * 20, 10000));
      } else {
        sleep_us(10);
        //printf("[%" PRIu64 "] write total %" PRIu64 " bytes.\n", thread_id, offset);
      }
    }
  }

  tk_atomic_store_explicit(stop, value_set_bool(&v, TRUE), TK_ATOMIC_MEMORY_ORDER_RELEASE);
  return NULL;
}

int main(int argc, char* argv[]) {
  uint8_t cache[CACHE_SIZE];
  tk_atomic_t stop;
  str_t s;
  value_t v;

  platform_prepare();
  timer_prepare(time_now_ms);

  tk_atomic_init(&stop, value_set_bool(&v, FALSE));

  log_debug("generate random text start.\n");
  s_text = generate_random_text(TEST_BUF_SIZE);
  log_debug("generate random text end.\n");

  tk_lf_bp_buffer_init(&s_buffer, cache, sizeof(cache));

  str_init(&s, TEST_BUF_SIZE);

  tk_thread_t* thread = tk_thread_create(thread_entry, &stop);

  uint64_t time = timer_manager()->get_time();

  tk_thread_start(thread);

  bool_t failed_times = 0;
  uint64_t thread_id = tk_thread_self();
  do {
    uint64_t available = 0;
    uint8_t* read_ptr = tk_lf_bp_buffer_read_acquire(&s_buffer, &available);
    if (NULL != read_ptr) {
      failed_times = 0;
      str_append_with_len(&s, (const char*)read_ptr, available);
      tk_lf_bp_buffer_read_release(&s_buffer, available);
    } else if (RET_OK == tk_atomic_load_explicit(&stop, &v, TK_ATOMIC_MEMORY_ORDER_ACQUIRE) &&
               value_bool(&v)) {
      break;
    } else {
      failed_times++;
      if (failed_times >= 3) {
        sleep_us(tk_min(failed_times * 20, 10000));
      } else {
        sleep_us(10);
        //printf("[%" PRIu64 "] read total %" PRIu32 " bytes.\n", thread_id, s.size);
      }
    }
  } while (TRUE);

  time = timer_manager()->get_time() - time;
  printf("time used: %" PRIu64 " ms\n", time);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  ENSURE(tk_str_eq(s.str, s_text));

  str_reset(&s);

  tk_atomic_deinit(&stop);

  tk_lf_bp_buffer_deinit(&s_buffer);
  TKMEM_FREE(s_text);
  return 0;
}
