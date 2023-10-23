/**
 * File:   iostream_process.h
 * Author: AWTK Develop Team
 * Brief:  output input stream on process
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-09-27 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "istream_process.h"
#include "ostream_process.h"
#include "iostream_process.h"


static ret_t tk_iostream_process_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  tk_iostream_process_t* iostream_process = TK_IOSTREAM_PROCESS(obj);

  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, (int32_t)(process_handle_get_fd(iostream_process->handle)));
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_PROCESS_FILE_PATH)) {
    value_set_str(v, iostream_process->file_path);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok = process_handle_get_fd(iostream_process->handle) > 0 && process_is_broken(iostream_process->handle);
    value_set_bool(v, is_ok);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_iostream_process_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  tk_iostream_process_t* iostream_process = TK_IOSTREAM_PROCESS(obj);

  if (tk_str_eq(name, TK_IOSTREAM_PROCESS_FILE_PATH)) {
    iostream_process->file_path = tk_str_copy(iostream_process->file_path, value_str(v));
    return RET_OK;
  } 

  return RET_NOT_FOUND;
}

static ret_t tk_iostream_process_on_destroy(tk_object_t* obj) {
  uint32_t i = 0;
  tk_iostream_process_t* iostream_process = TK_IOSTREAM_PROCESS(obj);
  return_value_if_fail(iostream_process != NULL, RET_BAD_PARAMS);

  if (iostream_process->file_path != NULL) {
    TKMEM_FREE(iostream_process->file_path);
    iostream_process->file_path = NULL;
  }

  if (iostream_process->args != NULL) {
    for (i = 0; i < iostream_process->argc; i++) {
      TKMEM_FREE(iostream_process->args[i]);
    }
    
    TKMEM_FREE(iostream_process->args);
    iostream_process->args = NULL;
  }

  if (iostream_process->start_info.work_dir != NULL) {
    TKMEM_FREE(iostream_process->start_info.work_dir);
    iostream_process->start_info.work_dir = NULL;
  }

  tk_object_unref(TK_OBJECT(iostream_process->istream));
  tk_object_unref(TK_OBJECT(iostream_process->ostream));

  process_destroy(iostream_process->handle);
  return RET_OK;
}

static ret_t tk_iostream_process_exec(tk_object_t* obj, const char* name, const char* args) {
  tk_iostream_process_t* iostream_process = TK_IOSTREAM_PROCESS(obj);
  return_value_if_fail(iostream_process != NULL && name != NULL && args != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, TK_IOSTREAM_PROCESS_CMD_START)) {
    return tk_iostream_process_start(TK_IOSTREAM(iostream_process));
  }
  return RET_NOT_IMPL;
}

static tk_istream_t* tk_iostream_process_get_istream(tk_iostream_t* stream) {
  tk_iostream_process_t* iostream_process = TK_IOSTREAM_PROCESS(stream);
  return_value_if_fail(iostream_process != NULL, NULL);

  return iostream_process->istream;
}

static tk_ostream_t* tk_iostream_process_get_ostream(tk_iostream_t* stream) {
  tk_iostream_process_t* iostream_process = TK_IOSTREAM_PROCESS(stream);
  return_value_if_fail(iostream_process != NULL, NULL);

  return iostream_process->ostream;
}

static const object_vtable_t s_tk_iostream_process_vtable = {
    .type = "tk_iostream_process",
    .desc = "tk_iostream_process",
    .size = sizeof(tk_iostream_process_t),
    .get_prop = tk_iostream_process_get_prop,
    .set_prop = tk_iostream_process_set_prop,
    .exec = tk_iostream_process_exec,
    .on_destroy = tk_iostream_process_on_destroy
  };

tk_iostream_t* tk_iostream_process_create(const char* file_path, const char* args[], uint32_t argc) {
  uint32_t i = 0;
  tk_object_t* obj = NULL;
  tk_iostream_process_t* iostream_process = NULL;
  return_value_if_fail(file_path != NULL || (args != NULL && argc > 0), NULL);

  obj = tk_object_create(&s_tk_iostream_process_vtable);

  iostream_process = TK_IOSTREAM_PROCESS(obj);
  return_value_if_fail(iostream_process != NULL, NULL);

  iostream_process->argc = argc;
  if (file_path != NULL) {
    iostream_process->file_path = tk_strdup(file_path);
  }
  if (argc > 0) {
    iostream_process->args = TKMEM_ZALLOCN(const char*, argc);
    for (i = 0; i < argc; i++) {
      iostream_process->args[i] = tk_strdup(args[i]);
    }
  }

  iostream_process->istream = tk_istream_process_create();
  iostream_process->ostream = tk_ostream_process_create();
  TK_IOSTREAM(obj)->get_istream = tk_iostream_process_get_istream;
  TK_IOSTREAM(obj)->get_ostream = tk_iostream_process_get_ostream;

  return TK_IOSTREAM(obj);
}

ret_t tk_iostream_process_start(tk_iostream_t* iostream) {
  tk_iostream_process_t* iostream_process = TK_IOSTREAM_PROCESS(iostream);
  return_value_if_fail(iostream_process != NULL, RET_BAD_PARAMS);
  iostream_process->handle = process_create(iostream_process->file_path, iostream_process->args, iostream_process->argc, &iostream_process->start_info);
  return_value_if_fail(iostream_process != NULL, RET_BAD_PARAMS);
  if (iostream_process->handle != NULL) {
    tk_istream_process_set_handle(iostream_process->istream, iostream_process->handle);
    tk_ostream_process_set_handle(iostream_process->ostream, iostream_process->handle);

    return RET_OK;
  }
  return RET_FAIL;
}

ret_t tk_iostream_process_kill(tk_iostream_t* iostream) {
  tk_iostream_process_t* iostream_process = TK_IOSTREAM_PROCESS(iostream);
  return_value_if_fail(iostream_process != NULL, RET_BAD_PARAMS);
  return process_kill(iostream_process->handle);
}

ret_t tk_iostream_process_set_work_dir(tk_iostream_t* iostream, const char* work_dir) {
  tk_iostream_process_t* iostream_process = TK_IOSTREAM_PROCESS(iostream);
  return_value_if_fail(iostream_process != NULL, RET_BAD_PARAMS);
  if (work_dir == NULL && iostream_process->start_info.work_dir != NULL) {
    TKMEM_FREE(iostream_process->start_info.work_dir);
    iostream_process->start_info.work_dir = NULL;
  } else {
    iostream_process->start_info.work_dir = tk_str_copy(iostream_process->start_info.work_dir, work_dir);
  }
  return RET_OK;
}
