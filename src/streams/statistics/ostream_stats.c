/**
 * File:   p_this.c
 * Author: AWTK Develop Team
 * Brief:  output stream base on memory
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
#include "ostream_stats.h"
#include "iostream_stats.h"

static int32_t tk_ostream_stats_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t max_size) {
  int32_t size = 0;
  tk_ostream_stats_t* p_this = TK_OSTREAM_STATS(stream);

  size = tk_ostream_write(p_this->impl_ostream, buff, max_size);
  if (size > 0) {
    p_this->count += size;
  }
  return size;
}

static ret_t tk_ostream_stats_seek(tk_ostream_t* stream, uint32_t offset) {
  tk_ostream_stats_t* p_this = TK_OSTREAM_STATS(stream);
  return tk_ostream_seek(p_this->impl_ostream, offset);
}

static int32_t tk_ostream_stats_tell(tk_ostream_t* stream) {
  tk_ostream_stats_t* p_this = TK_OSTREAM_STATS(stream);
  return tk_ostream_tell(p_this->impl_ostream);
}

static ret_t tk_ostream_stats_flush(tk_ostream_t* stream) {
  tk_ostream_stats_t* p_this = TK_OSTREAM_STATS(stream);
  return tk_ostream_flush(p_this->impl_ostream);
}

static ret_t tk_ostream_stats_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  tk_ostream_stats_t* p_this = TK_OSTREAM_STATS(obj);
  return tk_object_set_prop(TK_OBJECT(p_this->impl_ostream), name, v);
}

static ret_t tk_ostream_stats_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  tk_ostream_stats_t* p_this = TK_OSTREAM_STATS(obj);
  if (tk_str_eq(name, TK_OSTREAM_STATS_PROP_WRITE)) {
    value_set_int(v, p_this->count);
    return RET_OK;
  }

  return tk_object_get_prop(TK_OBJECT(p_this->impl_ostream), name, v);
}

static ret_t tk_ostream_stats_exec(tk_object_t* obj, const char* name, const char* args) {
  tk_ostream_stats_t* p_this = TK_OSTREAM_STATS(obj);
  if (tk_str_eq(name, TK_OSTREAM_STATS_CMD_RESET)) {
    p_this->count = 0;
    return RET_OK;
  }
  return tk_object_exec(TK_OBJECT(p_this->impl_ostream), name, args);
}

static ret_t tk_ostream_stats_on_destroy(tk_object_t* obj) {
  return RET_OK;
}

static const object_vtable_t s_tk_ostream_stats_vtable = {
    .type = "tk_ostream_stats",
    .desc = "tk_ostream_stats",
    .size = sizeof(tk_ostream_stats_t),
    .on_destroy = tk_ostream_stats_on_destroy,
    .get_prop = tk_ostream_stats_get_prop,
    .set_prop = tk_ostream_stats_set_prop,
    .exec = tk_ostream_stats_exec,
};

tk_ostream_t* tk_ostream_stats_create(tk_ostream_t* impl) {
  tk_object_t* obj = NULL;
  tk_ostream_stats_t* p_this = NULL;
  return_value_if_fail(impl, NULL);

  obj = tk_object_create(&s_tk_ostream_stats_vtable);
  p_this = TK_OSTREAM_STATS(obj);
  return_value_if_fail(p_this != NULL, NULL);

  p_this->impl_ostream = impl;
  TK_OSTREAM(obj)->write = tk_ostream_stats_write;
  TK_OSTREAM(obj)->seek = tk_ostream_stats_seek;
  TK_OSTREAM(obj)->tell = tk_ostream_stats_tell;
  TK_OSTREAM(obj)->flush = tk_ostream_stats_flush;

  return TK_OSTREAM(obj);
}
