/**
 * File:   istream_process.h
 * Author: AWTK Develop Team
 * Brief:  input stream on process
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "istream_process.h"

static ret_t tk_istream_process_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  tk_istream_process_t* istream_process = TK_ISTREAM_PROCESS(obj);
  return_value_if_fail(istream_process != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, (int)(process_handle_get_fd(istream_process->handle)));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static int32_t tk_istream_process_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  tk_istream_process_t* s = TK_ISTREAM_PROCESS(stream);
  return_value_if_fail(s != NULL && s->handle && buff != NULL, -1);
  return process_read(s->handle, buff, max_size);
}

ret_t tk_istream_process_wait_for_data(tk_istream_t* stream, uint32_t timeout_ms) {
  tk_istream_process_t* s = TK_ISTREAM_PROCESS(stream);
  return_value_if_fail(s != NULL && s->handle, RET_BAD_PARAMS);
  return process_wait_for_data(s->handle, timeout_ms);
}

static bool_t tk_istream_process_eos(tk_istream_t* stream) {
  tk_istream_process_t* s = TK_ISTREAM_PROCESS(stream);
  return_value_if_fail(s != NULL && s->handle, -1);
  return process_handle_get_fd(s->handle) > 0;
}

static const object_vtable_t s_tk_istream_process_vtable = {
    .type = "tk_istream_process",
    .desc = "tk_istream_process",
    .size = sizeof(tk_istream_process_t),
    .get_prop = tk_istream_process_get_prop,
};

tk_istream_t* tk_istream_process_create(void) {
  tk_object_t* obj = NULL;
  tk_istream_process_t* istream_process = NULL;
  obj = tk_object_create(&s_tk_istream_process_vtable);
  istream_process = TK_ISTREAM_PROCESS(obj);
  return_value_if_fail(istream_process != NULL, NULL);

  TK_ISTREAM(obj)->eos = tk_istream_process_eos;
  TK_ISTREAM(obj)->read = tk_istream_process_read;
  TK_ISTREAM(obj)->wait_for_data = tk_istream_process_wait_for_data;

  return TK_ISTREAM(obj);
}

ret_t tk_istream_process_set_handle(tk_istream_t* istream, process_handle_t handle) {
  tk_istream_process_t* istream_process = TK_ISTREAM_PROCESS(istream);
  return_value_if_fail(istream_process != NULL && handle != NULL, RET_BAD_PARAMS);
  istream_process->handle = handle;
  return RET_OK;
}
