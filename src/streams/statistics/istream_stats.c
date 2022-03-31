/**
 * File:   p_this.c
 * Author: AWTK Develop Team
 * Brief:  input stream base on memory
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 20220308 vih created
 *
 */

#include "tkc/mem.h"
#include "istream_stats.h"
#include "iostream_stats.h"

static int32_t tk_istream_stats_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  int32_t size = max_size;
  tk_istream_stats_t* p_this = TK_ISTREAM_STATS(stream);

  size = tk_istream_read(p_this->impl_istream, buff, max_size);
  if (size > 0) {
    p_this->count += size;
  }
  return size;
}

static ret_t tk_istream_stats_wait_for_data(tk_istream_t* stream, uint32_t timeout_ms) {
  tk_istream_stats_t* p_this = TK_ISTREAM_STATS(stream);
  return tk_istream_wait_for_data(p_this->impl_istream, timeout_ms);
}

static ret_t tk_istream_stats_seek(tk_istream_t* stream, uint32_t offset) {
  tk_istream_stats_t* p_this = TK_ISTREAM_STATS(stream);
  return tk_istream_seek(p_this->impl_istream, offset);
}

static int32_t tk_istream_stats_tell(tk_istream_t* stream) {
  tk_istream_stats_t* p_this = TK_ISTREAM_STATS(stream);
  return tk_istream_tell(p_this->impl_istream);
}

static bool_t tk_istream_stats_eos(tk_istream_t* stream) {
  tk_istream_stats_t* p_this = TK_ISTREAM_STATS(stream);
  return tk_istream_eos(p_this->impl_istream);
}

static ret_t tk_istream_stats_flush(tk_istream_t* stream) {
  tk_istream_stats_t* p_this = TK_ISTREAM_STATS(stream);
  return tk_istream_flush(p_this->impl_istream);
}

static ret_t tk_istream_stats_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  tk_istream_stats_t* p_this = TK_ISTREAM_STATS(obj);
  return tk_object_set_prop(TK_OBJECT(p_this->impl_istream), name, v);
}

static ret_t tk_istream_stats_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  tk_istream_stats_t* p_this = TK_ISTREAM_STATS(obj);
  if (tk_str_eq(name, TK_ISTREAM_STATS_PROP_READ)) {
    value_set_int(v, p_this->count);
    return RET_OK;
  }

  return tk_object_get_prop(TK_OBJECT(p_this->impl_istream), name, v);
}

static ret_t tk_istream_stats_exec(tk_object_t* obj, const char* name, const char* args) {
  tk_istream_stats_t* p_this = TK_ISTREAM_STATS(obj);
  if (tk_str_eq(name, TK_ISTREAM_STATS_CMD_RESET)) {
    p_this->count = 0;
    return RET_OK;
  }
  return tk_object_exec(TK_OBJECT(p_this->impl_istream), name, args);
}

static ret_t tk_istream_stats_on_destroy(tk_object_t* obj) {
  return RET_OK;
}

static const object_vtable_t s_tk_istream_stats_vtable = {
    .type = "tk_istream_stats",
    .desc = "tk_istream_stats",
    .size = sizeof(tk_istream_stats_t),
    .on_destroy = tk_istream_stats_on_destroy,
    .get_prop = tk_istream_stats_get_prop,
    .set_prop = tk_istream_stats_set_prop,
    .exec = tk_istream_stats_exec,
};

tk_istream_t* tk_istream_stats_create(tk_istream_t* impl) {
  tk_object_t* obj = NULL;
  tk_istream_stats_t* p_this = NULL;
  return_value_if_fail(impl != NULL, NULL);

  obj = tk_object_create(&s_tk_istream_stats_vtable);
  p_this = TK_ISTREAM_STATS(obj);
  return_value_if_fail(p_this != NULL, NULL);

  p_this->count = 0;
  p_this->impl_istream = impl;
  TK_ISTREAM(obj)->read = tk_istream_stats_read;
  TK_ISTREAM(obj)->seek = tk_istream_stats_seek;
  TK_ISTREAM(obj)->tell = tk_istream_stats_tell;
  TK_ISTREAM(obj)->eos = tk_istream_stats_eos;
  TK_ISTREAM(obj)->flush = tk_istream_stats_flush;
  TK_ISTREAM(obj)->wait_for_data = tk_istream_stats_wait_for_data;

  return TK_ISTREAM(obj);
}
