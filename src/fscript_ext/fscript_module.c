/**
 * File:   fscript_module.c
 * Author: AWTK Develop Team
 * Brief:  fscript module
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2022-03-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/data_reader.h"
#include "fscript_ext/fscript_module.h"

static ret_t fscript_module_on_destroy(tk_object_t* obj) {
  fscript_module_t* o = FSCRIPT_MODULE(obj);
  return_value_if_fail(o != NULL && o->fscript != NULL, RET_BAD_PARAMS);

  fscript_destroy(o->fscript);
  o->fscript = NULL;

  return RET_OK;
}

static ret_t fscript_module_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  fscript_module_t* o = FSCRIPT_MODULE(obj);
  return_value_if_fail(o != NULL && o->fscript != NULL, RET_BAD_PARAMS);

  return tk_object_get_prop(o->fscript->funcs_def, name, v);
}

static ret_t fscript_module_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  fscript_module_t* o = FSCRIPT_MODULE(obj);
  return_value_if_fail(o != NULL && o->fscript != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(o->fscript->funcs_def, on_prop, ctx);
}

static const object_vtable_t s_fscript_module_vtable = {
    .type = "fscript_module",
    .desc = "fscript_module",
    .size = sizeof(fscript_module_t),
    .is_collection = FALSE,
    .on_destroy = fscript_module_on_destroy,
    .get_prop = fscript_module_get_prop,
    .foreach_prop = fscript_module_foreach_prop};

tk_object_t* fscript_module_create_with_data(const char* data) {
  value_t v;
  fscript_t* fscript = NULL;
  tk_object_t* o = tk_object_create(&s_fscript_module_vtable);
  fscript_module_t* module = FSCRIPT_MODULE(o);
  return_value_if_fail(module != NULL, NULL);
  fscript = fscript_create(o, data);
  goto_error_if_fail(fscript != NULL);

  module->fscript = fscript;

  value_set_int(&v, 0);
  fscript_exec(fscript, &v);
  value_reset(&v);

  return o;
error:
  TK_OBJECT_UNREF(o);

  return NULL;
}

tk_object_t* fscript_module_create(const char* url) {
  char* data = NULL;
  uint32_t size = 0;
  tk_object_t* o = NULL;
  return_value_if_fail(url != NULL, NULL);
  if (strstr(url, "://")) {
    data = (char*)data_reader_read_all(url, &size);
  } else {
    data = file_read(url, &size);
  }
  return_value_if_fail(data != NULL, NULL);

  o = fscript_module_create_with_data(data);
  TKMEM_FREE(data);

  return o;
}

fscript_module_t* fscript_module_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_fscript_module_vtable, NULL);

  return (fscript_module_t*)(obj);
}

static ret_t func_require_str(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* code = NULL;
  tk_object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  code = value_str(args->args);
  FSCRIPT_FUNC_CHECK(code != NULL, RET_BAD_PARAMS);

  obj = fscript_module_create_with_data(code);
  FSCRIPT_FUNC_CHECK(obj != NULL, RET_OOM);

  value_set_object(result, obj);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_require(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* url = NULL;
  tk_object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  url = value_str(args->args);
  FSCRIPT_FUNC_CHECK(url != NULL, RET_BAD_PARAMS);

  obj = fscript_module_create(url);
  FSCRIPT_FUNC_CHECK(obj != NULL, RET_OOM);

  value_set_object(result, obj);
  result->free_handle = TRUE;

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_object)
FACTORY_TABLE_ENTRY("require_str", func_require_str)
FACTORY_TABLE_ENTRY("require", func_require)
FACTORY_TABLE_END()

ret_t fscript_module_register(void) {
  return fscript_register_funcs(s_ext_object);
}
