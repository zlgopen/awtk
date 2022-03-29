/**
 * File:   platform.c
 * Author: AWTK Develop Team
 * Brief:  platform dependent function of aworks
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2021-12-16 Wang LinFu <wanglinfu@zlg.cn> created
 *
 */

#include "aworks.h"
#include "aw_system.h"
#include "aw_delay.h"
#include "aw_mem.h"
#include "aw_time.h"

#include "tkc/mem.h"
#include "tkc/date_time.h"
#include "tkc/fscript.h"
#include "tkc/fs.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_reader_file.h"
#include "tkc/data_reader_mem.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_file.h"
#include "tkc/path.h"

#ifndef tk_log_info_lf
#define tk_log_info_lf(fmt, args...) log_info(fmt "\n", ##args)
#define tk_log_info_htlf(header, fmt, args...)
#define tk_log_warn_htlf(header, fmt, args...)
#endif

extern ret_t fs_os_register(const char* mnt_name, fs_t* (*create)(const char*));
extern fs_t* fs_aworks_create(const char* mnt);
static void platform_test(void);

static ret_t date_time_get_now_impl(date_time_t* dt) {
  aw_err_t ret;
  aw_tm_t t;
  ret = aw_local_tm_get(&t);

  if (ret == AW_OK) {
    dt->second = t.tm_sec;
    dt->minute = t.tm_min;
    dt->hour = t.tm_hour;
    dt->day = t.tm_mday;
    dt->month = t.tm_mon + 1;
    dt->year = t.tm_year + 1900;
    dt->wday = t.tm_wday;
  } else {
    dt->second = 0;
    dt->minute = 0;
    dt->hour = 0;
    dt->day = 11;
    dt->month = 11;
    dt->year = 2018;
    dt->wday = 0;
  }

  return RET_OK;
}

#ifndef HAS_STD_MALLOC
#define TK_MEM_SIZE 1 * 1024 * 1024

static ret_t mem_init(void) {
  uint32_t* mem = (uint32_t*)aw_mem_alloc(TK_MEM_SIZE);
  assert(mem != NULL);

  tk_mem_init(mem, TK_MEM_SIZE);
}
#else
#define mem_init()
#endif

uint64_t get_time_ms64() {
  return aw_sys_tick_get();
}

void sleep_ms(uint32_t ms) {
  aw_mdelay(ms);
}

void perror(const char* a) {
}

static ret_t fs_func_memdump(tk_object_t* obj, fscript_args_t* args, value_t* result) {
  tk_mem_dump();
  value_set_bool(result, 1);
  return RET_OK;
}

static ret_t writer_reader_init(void) {
  data_reader_factory_t* reader_factory = data_reader_factory_create();
  data_writer_factory_t* writer_factory = data_writer_factory_create();

  data_reader_factory_set(reader_factory);
  //  data_reader_factory_register(reader_factory, "asset", data_reader_asset_create);
  data_reader_factory_register(reader_factory, "mem", data_reader_mem_create);
  data_reader_factory_register(reader_factory, "file", data_reader_file_create);
  data_writer_factory_set(writer_factory);
  data_writer_factory_register(writer_factory, "file", data_writer_file_create);

  return RET_OK;
}

ret_t platform_prepare(void) {
  static bool_t inited = FALSE;
  char path[MAX_PATH + 1];

  if (inited) {
    return RET_OK;
  }
  inited = TRUE;

  mem_init();
  date_time_set_impl(date_time_get_now_impl);
  writer_reader_init();
  fs_aworks_init();
  fscript_register_func("memdump", fs_func_memdump);

  fs_get_exe(os_fs(), path);
  tk_log_info_lf("fs_get_exe('%s').", path);

  fs_get_temp_path(os_fs(), path);
  tk_log_info_lf("fs_get_temp_path('%s').", path);

  fs_get_user_storage_path(os_fs(), path);
  tk_log_info_lf("fs_get_user_storage_path('%s').", path);

  path_app_root(path);
  tk_log_info_lf("path_app_root('%s').", path);

  tk_log_info_lf("done.");

  platform_test();
  return RET_OK;
}

/*****************************************************************************/
#if defined(UTEST_ENABLE) && 1

#include "tkc/thread.h"
#include "tkc/semaphore.h"
#include "tkc/mutex.h"
#include "tkc/fs.h"
#include "tkc/str.h"

typedef struct {
  void* args;
  int run;
} thread_ctx_t;

static void* task_entry(void* args) {
  int i = 5;
  static int chk_pri = 1;
  if (chk_pri == 1) {
    ENSURE(tk_str_eq_with_len("high", (const char*)args, 4));
    chk_pri++;
  }
  while (i-- > 0) {
    sleep_ms(1000);
    tk_log_info_htlf("[UTEST]", "task %s execute.", (char*)args);
  }
  return NULL;
}

static void platform_test(void) {
#ifdef HAS_STD_MALLOC
  if (1) {
    void* p = malloc(8);
    tk_log_info_htlf("[UTEST]", "test_str: step1 %p", p);
    ENSURE(p);
    tk_snprintf(p, 8, "abc123");
    p = realloc(p, 16);
    tk_log_info_htlf("[UTEST]", "test_str: step1");
    ENSURE(p);
    ENSURE(tk_str_cmp(p, "abc123") == 0);
    free(p);
  }
#endif

  if (1) {
    uint64_t s = time_now_ms();
    tk_log_info_htlf("[TEST]", "sleep_ms: start %u ms", (unsigned int)s);
    sleep_ms(1050);
    tk_log_info_htlf("[TEST]", "sleep_ms: stop %u ms (%u)", (unsigned int)time_now_ms(),
                     (unsigned int)(time_now_ms() - s));
    ENSURE((time_now_ms() - s) >= 1020 && (time_now_ms() - s) <= 1080);
  }

  if (1) {
    tk_mutex_t* p = tk_mutex_create();
    ENSURE(tk_mutex_lock(p) == RET_OK);
    ENSURE(tk_mutex_try_lock(p) == RET_OK);
    ENSURE(tk_mutex_lock(p) == RET_OK);

    ENSURE(tk_mutex_unlock(p) == RET_OK);
    ENSURE(tk_mutex_unlock(p) == RET_OK);
    ENSURE(tk_mutex_unlock(p) == RET_OK);

    tk_mutex_destroy(p);
  }
  if (1) {
    tk_semaphore_t* p = tk_semaphore_create(3, NULL);

    tk_log_info_htlf("[TEST]", "tk_semaphore_t: start  time %u ms", (unsigned int)time_now_ms());
    ENSURE(tk_semaphore_wait(p, 1000) == RET_OK);
    ENSURE(tk_semaphore_wait(p, 1000) == RET_OK);
    ENSURE(tk_semaphore_wait(p, 1000) == RET_OK);
    ENSURE(tk_semaphore_wait(p, 1000) != RET_OK);

    tk_log_info_htlf("[TEST]", "tk_semaphore_t: locked time %u ms", (unsigned int)time_now_ms());
    ENSURE(tk_semaphore_post(p) == RET_OK);
    ENSURE(tk_semaphore_post(p) == RET_OK);
    ENSURE(tk_semaphore_post(p) == RET_OK);
    tk_log_info_htlf("[TEST]", "tk_semaphore_t: stop   time %u ms", (unsigned int)time_now_ms());

    tk_semaphore_destroy(p);
  }
  if (1) {
    tk_thread_t* p = tk_thread_create(task_entry, "low_t1");
    tk_thread_t* p1 = tk_thread_create(task_entry, "high_t2");
    ENSURE(p && p1);

    tk_log_info_htlf("[TEST]", "tk_thread: start %u ms ", (unsigned int)time_now_ms());
    ENSURE(tk_thread_set_priority(p1, tk_thread_get_priority_from_platform(
                                          TK_THREAD_PRIORITY_BELOW_NORAML)) == RET_OK);
    ENSURE(tk_thread_start(p) == RET_OK);

    tk_log_info_htlf("[TEST]", "tk_thread: start %u ms ", (unsigned int)time_now_ms());
    ENSURE(tk_thread_set_priority(p1, tk_thread_get_priority_from_platform(
                                          TK_THREAD_PRIORITY_ABOVE_NORAML)) == RET_OK);
    ENSURE(tk_thread_start(p1) == RET_OK);

    tk_log_info_htlf("[TEST]", "task startup:");
    sleep_ms(3050);

    tk_thread_join(p);
    tk_thread_destroy(p);
    tk_log_info_htlf("[TEST]", "tk_thread: stop  %u ms ", (unsigned int)time_now_ms());

    tk_thread_join(p1);
    tk_thread_destroy(p1);
    tk_log_info_htlf("[TEST]", "tk_thread: stop  %u ms ", (unsigned int)time_now_ms());
  }

  tk_log_info_htlf("[UTEST]", "pass.");
}
#else
static void platform_test(void) {
  tk_log_info_htlf("[UTEST]", "nothing.");
}
#endif
