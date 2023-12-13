/**
 * File:   csv_row_object.h
 * Author: AWTK Develop Team
 * Brief:  csv row object
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License row for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2023-12-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "csv_file_object.h"
#include "csv_row_object.h"

static csv_row_object_t* csv_row_object_cast(tk_object_t* obj);
#define CSV_ROW_OBJECT(obj) csv_row_object_cast((tk_object_t*)obj)

static uint32_t get_col_from_name(const char* name) {
  if (name[0] == '[') {
    return tk_atoi(name + 1);
  } else {
    return tk_atoi(name);
  }
}

static ret_t csv_row_object_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  uint32_t col = 0;
  char buff[64] = {0};
  csv_row_object_t* o = CSV_ROW_OBJECT(obj);
  return_value_if_fail(o != NULL && name != NULL, RET_BAD_PARAMS);
  col = get_col_from_name(name);

  return csv_row_set(&(o->row), col, value_str_ex(v, buff, sizeof(buff) - 1));
}

static ret_t csv_row_object_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  uint32_t col = 0;
  csv_row_object_t* o = CSV_ROW_OBJECT(obj);
  return_value_if_fail(o != NULL && name != NULL, RET_BAD_PARAMS);
  col = get_col_from_name(name);

  value_set_str(v, csv_row_get(&(o->row), col));

  return RET_OK;
}

static bool_t csv_row_object_can_exec(tk_object_t* obj, const char* name, const char* args) {
  csv_row_object_t* o = CSV_ROW_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, TK_OBJECT_CMD_SAVE) || tk_str_ieq(name, TK_OBJECT_CMD_ADD)) {
    csv_file_object_t* csv_file_object = CSV_FILE_OBJECT(o->csv);
    if (csv_file_object->check_new_row != NULL) {
      return csv_file_object->check_new_row(csv_file_object, &(o->row));
    }
    return TRUE;
  }

  return FALSE;
}

static ret_t csv_row_object_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_NOT_IMPL;
  csv_row_object_t* o = CSV_ROW_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, TK_OBJECT_CMD_SAVE) || tk_str_ieq(name, TK_OBJECT_CMD_ADD)) {
    str_t str;
    csv_row_t* row = &(o->row);
    csv_file_object_t* csv_file_object = CSV_FILE_OBJECT(o->csv);

    str_init(&str, 128);
    csv_row_to_str(row, &str, csv_file_object->csv->sep);
    tk_object_exec(TK_OBJECT(csv_file_object), TK_OBJECT_CMD_ADD, str.str);
    str_reset(&str);

    ret = RET_OK;
  }

  return ret;
}

static ret_t csv_row_object_destroy(tk_object_t* obj) {
  csv_row_object_t* o = CSV_ROW_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  csv_row_reset(&(o->row));
  o->csv = NULL;

  return RET_OK;
}

static const object_vtable_t s_csv_row_object_vtable = {.type = "csv_row_object",
                                                        .desc = "csv_row_object",
                                                        .size = sizeof(csv_row_object_t),
                                                        .is_collection = TRUE,
                                                        .exec = csv_row_object_exec,
                                                        .can_exec = csv_row_object_can_exec,
                                                        .get_prop = csv_row_object_get_prop,
                                                        .set_prop = csv_row_object_set_prop,
                                                        .on_destroy = csv_row_object_destroy};

static csv_row_object_t* csv_row_object_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_csv_row_object_vtable, NULL);

  return (csv_row_object_t*)obj;
}

tk_object_t* csv_row_object_create(tk_object_t* csv, const char* init) {
  tk_object_t* obj = NULL;
  csv_row_object_t* o = NULL;
  csv_file_object_t* csv_file_object = CSV_FILE_OBJECT(csv);
  return_value_if_fail(csv_file_object != NULL, NULL);

  obj = tk_object_create(&s_csv_row_object_vtable);
  o = CSV_ROW_OBJECT(obj);
  return_value_if_fail(o != NULL, NULL);

  o->csv = csv;
  csv_row_set_data(&(o->row), init, csv_file_object->csv->sep);

  return obj;
}
