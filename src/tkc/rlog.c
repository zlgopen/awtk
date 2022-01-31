/**
 * File:   rlog.c
 * Author: AWTK Develop Team
 * Brief:  rotate log api
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
ret_t rlog_clear(rlog_t* log) {
  char filename[MAX_PATH + 1];
  ret_t ret;
  my_return_value_if_fail(log, RET_BAD_PARAMS);

  ret = tk_mutex_nest_lock(log->mutex);
  my_return_value_if_fail(ret == RET_OK, RET_FAIL);

  fs_file_close(log->fp);
  tk_snprintf(filename, MAX_PATH, log->filename_pattern, (int)0);
  fs_remove_file(os_fs(), filename);
  log->fp = fs_open_file(os_fs(), filename, "wb+");

  if (log->index == 1) {
    tk_snprintf(filename, MAX_PATH, log->filename_pattern, (int)1);
    fs_remove_file(os_fs(), filename);
    log->index = 0;
  }
  tk_mutex_nest_unlock(log->mutex);

  my_return_value_if_fail(log->fp != NULL, RET_FAIL);
  return RET_OK;
}

ret_t rlog_size(rlog_t* log, uint32_t* size) {
  char filename[MAX_PATH + 1];
  ret_t ret;
  int32_t sz = 0;
  my_return_value_if_fail(log && size, RET_BAD_PARAMS);

  ret = tk_mutex_nest_lock(log->mutex);
  my_return_value_if_fail(ret == RET_OK, RET_FAIL);

  tk_snprintf(filename, MAX_PATH, log->filename_pattern, (int)0);
  sz = file_get_size(filename);
  goto_error_if_fail(sz >= 0);

  *size = sz;
  if (log->index == 1) {
    tk_snprintf(filename, MAX_PATH, log->filename_pattern, (int)1);
    sz = file_get_size(filename);
    goto_error_if_fail(sz >= 0);
    *size += sz;
  }
  tk_mutex_nest_unlock(log->mutex);
  return RET_OK;

error:
  tk_mutex_nest_unlock(log->mutex);
  return RET_FAIL;
}

ret_t rlog_read(rlog_t* log, uint32_t offs, char* buff, uint32_t buffsz, uint32_t* readsz) {
  char filename[MAX_PATH + 1];
  int32_t ret;
  int32_t sz = 0;
  my_return_value_if_fail(log && buff && buffsz && readsz, RET_BAD_PARAMS);

  ret = tk_mutex_nest_lock(log->mutex);
  my_return_value_if_fail(ret == RET_OK, RET_FAIL);

  tk_snprintf(filename, MAX_PATH, log->filename_pattern, (int)0);
  sz = file_get_size(filename);
  goto_error_if_fail(sz >= 0);
  if (offs < sz) {
    ret = file_read_part(filename, buff, buffsz, offs);
    goto_error_if_fail(ret >= 0);
    offs = 0;
    buff += ret;
    buffsz -= ret;
    *readsz = ret;
  } else {
    offs -= sz;
  }
  if (buffsz == 0 || log->index == 0) {
    tk_mutex_nest_unlock(log->mutex);
    return RET_OK;
  }

  tk_snprintf(filename, MAX_PATH, log->filename_pattern, (int)1);
  ret = file_get_size(filename);
  goto_error_if_fail(ret >= 0);
  goto_error_if_fail(offs < sz);

  ret = file_read_part(filename, buff, buffsz, offs);
  goto_error_if_fail(ret >= 0);

  *readsz += ret;
  tk_mutex_nest_unlock(log->mutex);
  return RET_OK;

error:
  tk_mutex_nest_unlock(log->mutex);
  return RET_FAIL;
}
