/**
 * File:   ostream_process.c
 * Author: AWTK Develop Team
 * Brief:  output stream base on process
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "ostream_process.h"

static int32_t tk_ostream_process_write(tk_ostream_t* stream, const uint8_t* buff,
                                        uint32_t max_size) {
  tk_ostream_process_t* ostream_process = TK_OSTREAM_PROCESS(stream);
  return_value_if_fail(ostream_process != NULL && buff != NULL, RET_BAD_PARAMS);

  return process_write(ostream_process->handle, buff, max_size);
}

static ret_t tk_ostream_process_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  tk_ostream_process_t* ostream_process = TK_OSTREAM_PROCESS(obj);
  return_value_if_fail(ostream_process != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, process_handle_get_fd(ostream_process->handle));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_tk_ostream_process_vtable = {
    .type = "tk_ostream_process",
    .desc = "tk_ostream_process",
    .size = sizeof(tk_ostream_process_t),
    .get_prop = tk_ostream_process_get_prop};

tk_ostream_t* tk_ostream_process_create(void) {
  tk_object_t* obj = NULL;
  tk_ostream_process_t* ostream_process = NULL;

  obj = tk_object_create(&s_tk_ostream_process_vtable);
  ostream_process = TK_OSTREAM_PROCESS(obj);
  return_value_if_fail(ostream_process != NULL, NULL);

  TK_OSTREAM(obj)->write = tk_ostream_process_write;
  return TK_OSTREAM(obj);
}

ret_t tk_ostream_process_set_handle(tk_ostream_t* ostream, process_handle_t handle) {
  tk_ostream_process_t* ostream_process = TK_OSTREAM_PROCESS(ostream);
  return_value_if_fail(ostream_process != NULL && handle != NULL, RET_BAD_PARAMS);
  ostream_process->handle = handle;
  return RET_OK;
}
