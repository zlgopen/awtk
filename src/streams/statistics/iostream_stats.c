/**
 * File:   p_this.c
 * Author: AWTK Develop Team
 * Brief:  iostream base on mem
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
#include "ostream_stats.h"
#include "iostream_stats.h"

static ret_t tk_iostream_stats_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  tk_iostream_stats_t* p_this = TK_IOSTREAM_STATS(obj);

  if (tk_str_eq(name, TK_ISTREAM_STATS_PROP_READ)) {
    return tk_object_get_prop(TK_OBJECT(p_this->istream), name, v);
  } else if (tk_str_eq(name, TK_OSTREAM_STATS_PROP_WRITE)) {
    return tk_object_get_prop(TK_OBJECT(p_this->ostream), name, v);
  } else if (tk_str_eq(name, "iostream")) {
    value_set_pointer(v, p_this->impl);
    return RET_OK;
  }

  return tk_object_get_prop(TK_OBJECT(p_this->impl), name, v);
}

static ret_t tk_iostream_stats_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  tk_iostream_stats_t* p_this = TK_IOSTREAM_STATS(obj);
  return tk_object_set_prop(TK_OBJECT(p_this->impl), name, v);
}

static ret_t tk_iostream_stats_exec(tk_object_t* obj, const char* name, const char* args) {
  tk_iostream_stats_t* p_this = TK_IOSTREAM_STATS(obj);
  if (tk_str_eq(name, TK_ISTREAM_STATS_CMD_RESET)) {
    return tk_object_exec(TK_OBJECT(p_this->istream), name, args);
  } else if (tk_str_eq(name, TK_OSTREAM_STATS_CMD_RESET)) {
    return tk_object_exec(TK_OBJECT(p_this->ostream), name, args);
  }

  return tk_object_exec(TK_OBJECT(p_this->impl), name, args);
}

static ret_t tk_iostream_stats_on_destroy(tk_object_t* obj) {
  tk_iostream_stats_t* p_this = TK_IOSTREAM_STATS(obj);

  tk_object_unref(TK_OBJECT(p_this->impl));
  tk_object_unref(TK_OBJECT(p_this->istream));
  tk_object_unref(TK_OBJECT(p_this->ostream));

  return RET_OK;
}

static const object_vtable_t s_tk_iostream_stats_vtable = {
    .type = "tk_iostream_stats",
    .desc = "tk_iostream_stats",
    .size = sizeof(tk_iostream_stats_t),
    .get_prop = tk_iostream_stats_get_prop,
    .set_prop = tk_iostream_stats_set_prop,
    .exec = tk_iostream_stats_exec,
    .on_destroy = tk_iostream_stats_on_destroy,
};

static tk_istream_t* tk_iostream_stats_get_istream(tk_iostream_t* stream) {
  tk_iostream_stats_t* p_this = TK_IOSTREAM_STATS(stream);

  return p_this->istream;
}

static tk_ostream_t* tk_iostream_stats_get_ostream(tk_iostream_t* stream) {
  tk_iostream_stats_t* p_this = TK_IOSTREAM_STATS(stream);

  return p_this->ostream;
}

tk_iostream_t* tk_iostream_stats_create(tk_iostream_t* impl) {
  tk_object_t* obj = NULL;
  tk_iostream_stats_t* p_this = NULL;
  return_value_if_fail(impl != NULL, NULL);

  obj = tk_object_create(&s_tk_iostream_stats_vtable);
  p_this = TK_IOSTREAM_STATS(obj);
  if (p_this == NULL) {
    return_value_if_fail(p_this != NULL, NULL);
  }

  tk_object_ref(TK_OBJECT(impl));
  p_this->impl = impl;
  p_this->istream = tk_istream_stats_create(tk_iostream_get_istream(impl));
  p_this->ostream = tk_ostream_stats_create(tk_iostream_get_ostream(impl));

  TK_IOSTREAM(obj)->get_istream = tk_iostream_stats_get_istream;
  TK_IOSTREAM(obj)->get_ostream = tk_iostream_stats_get_ostream;

  return TK_IOSTREAM(obj);
}
