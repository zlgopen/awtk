/**
 * File:   csv_file_object.h
 * Author: AWTK Develop Team
 * Brief:  csv file object
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-07-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object.h"
#include "csv_file.h"

typedef struct _csv_file_object_t {
  object_t object;

  /*private*/
  csv_file_t* csv;
  str_t str;
} csv_file_object_t;

static csv_file_object_t* csv_file_object_cast(object_t* obj);
#define CSV_FILE_OBJECT(obj) csv_file_object_cast((object_t*)obj)

typedef struct _csv_path_t {
  int32_t row;
  int32_t col;
  const char* col_name;
} csv_path_t;

static ret_t csv_path_parse(csv_path_t* path, csv_file_t* csv, const char* name) {
  const char* p = name;
  memset(path, 0x00, sizeof(*path));

  while (*p && *p != '[') p++;
  return_value_if_fail(*p == '[', RET_FAIL);

  p++;
  path->row = tk_atoi(p);
  p = strchr(p, '.');
  if (p == NULL) {
    return RET_OK;
  } else {
    p++;
  }

  if (tk_str_eq(p, OBJECT_PROP_CHECKED)) {
    path->col_name = p;

    return RET_OK;
  }

  if (*p == '[') {
    path->col = tk_atoi(p + 1);
  } else {
    path->col = csv_file_get_col_of_name(csv, p);
    if (path->col < 0) {
      path->col = tk_atoi(p);
    }
  }
  return_value_if_fail((path->col >= 0) && (path->col < csv_file_get_cols(csv)), RET_BAD_PARAMS);
  return_value_if_fail((path->row >= 0) && (path->row < csv_file_get_rows(csv)), RET_BAD_PARAMS);

  return RET_OK;
}

static ret_t csv_file_object_remove_prop(object_t* obj, const char* name) {
  csv_path_t p;
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  return_value_if_fail(csv_path_parse(&p, o->csv, name) == RET_OK, RET_FAIL);

  return csv_file_remove_row(o->csv, p.row);
}

static ret_t csv_file_object_set_prop(object_t* obj, const char* name, const value_t* v) {
  csv_path_t p;
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  return_value_if_fail(csv_path_parse(&p, o->csv, name) == RET_OK, RET_FAIL);

  if (p.col_name != NULL && tk_str_ieq(p.col_name, OBJECT_PROP_CHECKED)) {
    return csv_file_set_row_checked(o->csv, p.row, value_bool(v));
  }

  str_from_value(&(o->str), v);
  return csv_file_set(o->csv, p.row, p.col, o->str.str);
}

static ret_t csv_file_object_get_prop(object_t* obj, const char* name, value_t* v) {
  csv_path_t p;
  const char* str = NULL;
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, OBJECT_PROP_SIZE)) {
    value_set_int(v, csv_file_get_rows(o->csv));
    return RET_OK;
  }

  return_value_if_fail(csv_path_parse(&p, o->csv, name) == RET_OK, RET_FAIL);

  if (p.col_name != NULL && tk_str_ieq(p.col_name, OBJECT_PROP_CHECKED)) {
    return_value_if_fail(p.row < csv_file_get_rows(o->csv), RET_FAIL);

    value_set_bool(v, csv_file_is_row_checked(o->csv, p.row));
    return RET_OK;
  }

  value_set_str(v, "");
  str = csv_file_get(o->csv, p.row, p.col);
  return_value_if_fail(str != NULL, RET_FAIL);
  value_set_str(v, str);

  return RET_OK;
}

static bool_t csv_file_object_can_exec(object_t* obj, const char* name, const char* args) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, OBJECT_CMD_SAVE)) {
    return TRUE;
  } else if (tk_str_ieq(name, OBJECT_CMD_RELOAD)) {
    return TRUE;
  } else if (tk_str_ieq(name, OBJECT_CMD_CLEAR)) {
    return TRUE;
  } else if (tk_str_ieq(name, OBJECT_CMD_REMOVE)) {
    return TRUE;
  } else if (tk_str_ieq(name, OBJECT_CMD_REMOVE_CHECKED)) {
    return csv_file_get_checked_rows(o->csv) > 0;
  } else if (tk_str_ieq(name, OBJECT_CMD_ADD)) {
    return TRUE;
  }

  return FALSE;
}

static ret_t csv_file_object_exec(object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_NOT_IMPL;
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, OBJECT_CMD_SAVE)) {
    ret = csv_file_save(o->csv, NULL);
  } else if (tk_str_ieq(name, OBJECT_CMD_RELOAD)) {
    csv_file_reload(o->csv);
    ret = RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, OBJECT_CMD_CLEAR)) {
    csv_file_clear(o->csv);
    ret = RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, OBJECT_CMD_REMOVE)) {
    const char* index = strrchr(args, '[');
    if (index != NULL) {
      index++;
    } else {
      index = args;
    }
    return_value_if_fail(index != NULL, RET_FAIL);
    ret = csv_file_remove_row(o->csv, tk_atoi(index)) == RET_OK ? RET_ITEMS_CHANGED : RET_FAIL;
  } else if (tk_str_ieq(name, OBJECT_CMD_REMOVE_CHECKED)) {
    ret = csv_file_remove_checked_rows(o->csv) == RET_OK ? RET_ITEMS_CHANGED : RET_FAIL;
  } else if (tk_str_ieq(name, OBJECT_CMD_ADD)) {
    return_value_if_fail(args != NULL, RET_FAIL);
    ret = csv_file_append_row(o->csv, args) == RET_OK ? RET_ITEMS_CHANGED : RET_FAIL;
  } else {
    return RET_NOT_IMPL;
  }

  if (ret == RET_ITEMS_CHANGED) {
    emitter_dispatch_simple_event(EMITTER(obj), EVT_PROPS_CHANGED);
    emitter_dispatch_simple_event(EMITTER(obj), EVT_ITEMS_CHANGED);
  }

  return RET_OK;
}

static ret_t csv_file_object_destroy(object_t* obj) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  csv_file_destroy(o->csv);
  o->csv = NULL;
  str_reset(&(o->str));

  return RET_OK;
}

static const object_vtable_t s_csv_file_object_vtable = {.type = "csv_file_object",
                                                         .desc = "csv_file_object",
                                                         .size = sizeof(csv_file_object_t),
                                                         .is_collection = TRUE,
                                                         .exec = csv_file_object_exec,
                                                         .can_exec = csv_file_object_can_exec,
                                                         .remove_prop = csv_file_object_remove_prop,
                                                         .get_prop = csv_file_object_get_prop,
                                                         .set_prop = csv_file_object_set_prop,
                                                         .on_destroy = csv_file_object_destroy};

static csv_file_object_t* csv_file_object_cast(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_csv_file_object_vtable, NULL);

  return (csv_file_object_t*)obj;
}

object_t* csv_file_object_create(csv_file_t* csv) {
  object_t* obj = NULL;
  csv_file_object_t* o = NULL;
  return_value_if_fail(csv != NULL, NULL);

  obj = object_create(&s_csv_file_object_vtable);
  o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, NULL);

  o->csv = csv;
  str_init(&(o->str), 0);

  return obj;
}

csv_file_t* csv_file_object_get_csv(object_t* obj) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, NULL);

  return o->csv;
}
