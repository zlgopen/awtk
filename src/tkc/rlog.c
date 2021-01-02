/**
 * File:   rlog.c
 * Author: AWTK Develop Team
 * Brief:  rotate log api
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-11-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/rlog.h"
#include "tkc/utils.h"
#include "tkc/mutex_nest.h"

#ifdef HAS_STDIO
#define my_return_value_if_fail(p, value)                                \
  if (!(p)) {                                                            \
    printf("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    fflush(stdout);                                                      \
    return (value);                                                      \
  }
#else
#define my_return_value_if_fail(p, value) \
  if (!(p)) {                             \
    return (value);                       \
  }
#endif

rlog_t* rlog_create(const char* filename_pattern, uint32_t max_size, uint32_t buff_size) {
  rlog_t* log = NULL;
  fs_stat_info_t fst;
  fs_file_t* fp = NULL;
  uint32_t index = 0;
  char filename[MAX_PATH + 1];
  my_return_value_if_fail(filename_pattern != NULL && max_size > 1000 && buff_size > 32, NULL);

  memset(filename, 0x00, sizeof(filename));
  tk_snprintf(filename, MAX_PATH, filename_pattern, (int)index);
  if (fs_stat(os_fs(), filename, &fst) == RET_OK) {
    if (fst.size < (max_size - buff_size)) {
      fp = fs_open_file(os_fs(), filename, "ab+");
    } else {
      index = 1;
      tk_snprintf(filename, MAX_PATH, filename_pattern, (int)(index));
      fp = fs_open_file(os_fs(), filename, "ab+");
    }
  } else {
    fp = fs_open_file(os_fs(), filename, "wb+");
  }
  my_return_value_if_fail(fp != NULL, NULL);

  log = TKMEM_ZALLOC(rlog_t);
  if (log != NULL) {
    log->index = index;
    log->max_size = max_size;
    log->buff_size = buff_size;
    log->mutex = tk_mutex_nest_create();
    log->filename_pattern = tk_strdup(filename_pattern);

    if (log->filename_pattern != NULL) {
      log->fp = fp;
    } else {
      fs_file_close(fp);
      TKMEM_FREE(log);
    }
  } else {
    fs_file_close(fp);
  }

  return log;
}

ret_t rlog_write(rlog_t* log, const char* str) {
  uint32_t len = 0;
  fs_stat_info_t fst;
  my_return_value_if_fail(log != NULL && log->fp != NULL && str != NULL, RET_BAD_PARAMS);
  my_return_value_if_fail(fs_file_stat(log->fp, &fst) == RET_OK, RET_BAD_PARAMS);

  if (tk_mutex_nest_lock(log->mutex) == RET_OK) {
    len = strlen(str);
    if ((fst.size + len) > log->max_size) {
      char filename0[MAX_PATH + 1];
      char filename1[MAX_PATH + 1];

      fs_file_close(log->fp);
      memset(filename1, 0x00, sizeof(filename1));
      tk_snprintf(filename1, MAX_PATH, log->filename_pattern, (int)1);
      if (log->index == 1) {
        memset(filename0, 0x00, sizeof(filename0));
        tk_snprintf(filename0, MAX_PATH, log->filename_pattern, (int)0);
        fs_remove_file(os_fs(), filename0);
        fs_file_rename(os_fs(), filename1, filename0);
      }
      log->index = 1;
      log->fp = fs_open_file(os_fs(), filename1, "wb+");
    }
    if (log->fp != NULL) {
      fs_file_write(log->fp, str, len);
      fs_file_sync(log->fp);
    }
    tk_mutex_nest_unlock(log->mutex);
  }

  return RET_OK;
}

ret_t rlog_print(rlog_t* log, const char* format, ...) {
  va_list va;
  ret_t ret = RET_OK;
  my_return_value_if_fail(log != NULL && log->fp != NULL && format != NULL, RET_BAD_PARAMS);
  my_return_value_if_fail(log->buff_size > 0, RET_BAD_PARAMS);

  if (tk_mutex_nest_lock(log->mutex) == RET_OK) {
    if (log->buff == NULL) {
      log->buff = TKMEM_ALLOC(log->buff_size + 1);
      if (log->buff == NULL) {
        tk_mutex_nest_unlock(log->mutex);
        return RET_OOM;
      }
      memset(log->buff, 0x00, log->buff_size + 1);
    }

    va_start(va, format);
    tk_vsnprintf(log->buff, log->buff_size, format, va);
    va_end(va);

    ret = rlog_write(log, log->buff);
    tk_mutex_nest_unlock(log->mutex);
  }

  return ret;
}

ret_t rlog_destroy(rlog_t* log) {
  my_return_value_if_fail(log != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(log->buff);
  fs_file_close(log->fp);
  TKMEM_FREE(log->filename_pattern);
  tk_mutex_nest_destroy(log->mutex);
  memset(log, 0x00, sizeof(rlog_t));
  TKMEM_FREE(log);

  return RET_OK;
}
