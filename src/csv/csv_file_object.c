/**
 * File:   csv_file_object.h
 * Author: AWTK Develop Team
 * Brief:  csv file object
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/object_default.h"
#include "tkc/data_reader.h"

#include "csv_file.h"
#include "csv_file_object.h"

static csv_file_object_t* csv_file_object_cast(tk_object_t* obj);
#define CSV_FILE_OBJECT(obj) csv_file_object_cast((tk_object_t*)obj)

typedef struct _csv_path_t {
  int32_t row;
  int32_t col;
  const char* col_name;
} csv_path_t;

static ret_t csv_path_parse_impl(csv_path_t* path, csv_file_t* csv, const char* name) {
  const char* p = name;
  return_value_if_fail(path != NULL && csv != NULL && name != NULL, RET_BAD_PARAMS);

  memset(path, 0x00, sizeof(*path));

  while (tk_isspace(*p)) p++;

  if (tk_isdigit(*p)) {
    path->row = tk_atoi(p);
  } else if (*p != '[') {
    return RET_BAD_PARAMS;
  } else {
    p++; /* skip '[' */
    if (tk_isdigit(*p)) {
      path->row = tk_atoi(p);
    } else {
      return RET_BAD_PARAMS;
    }
  }

  p = strchr(p, '.');
  if (p == NULL) {
    return RET_OK;
  } else {
    p++;
  }

  if (tk_str_eq(p, TK_OBJECT_PROP_CHECKED)) {
    path->col_name = p;

    return RET_OK;
  }

  if (*p == '[') {
    return_value_if_fail(tk_isdigit(p[1]), RET_BAD_PARAMS);
    path->col = tk_atoi(p + 1);
  } else if (tk_isdigit(*p)) {
    path->col = tk_atoi(p);
  } else {
    path->col = csv_file_get_col_of_name(csv, p);
    if (path->col < 0) {
      return_value_if_fail(tk_isdigit(p[0]), RET_BAD_PARAMS);
      path->col = tk_atoi(p);
    }
  }
  return_value_if_fail((path->col >= 0) && (path->col < csv_file_get_cols(csv)), RET_BAD_PARAMS);
  return_value_if_fail((path->row >= 0) && (path->row < csv_file_get_rows(csv)), RET_BAD_PARAMS);

  return RET_OK;
}

static ret_t csv_file_object_remove_map(csv_file_object_t* o, uint32_t index) {
  uint32_t i = 0;
  uint32_t n = 0;
  uint32_t* rows_map = NULL;
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  rows_map = o->rows_map;
  n = o->rows_map_size;
  for (i = index; i < n - 1; i++) {
    rows_map[i] = rows_map[i + 1];
  }

  o->rows_map_size--;

  return RET_OK;
}

static ret_t csv_path_parse_ex(csv_file_object_t* o, csv_path_t* path, const char* name,
                            bool_t for_remove) {
  ret_t ret = RET_BAD_PARAMS;
  return_value_if_fail(o != NULL && path != NULL && name != NULL, RET_BAD_PARAMS);

  ret = csv_path_parse_impl(path, o->csv, name);
  if (ret == RET_OK) {
    if (o->rows_map != NULL) {
      uint32_t index = path->row;
      path->row = o->rows_map[path->row];
      if (for_remove) {
        csv_file_object_remove_map(o, index);
      }
    }
    return RET_OK;
  } else {
    return ret;
  }
}

static ret_t csv_path_parse(csv_file_object_t* o, csv_path_t* path, const char* name) {
  return csv_path_parse_ex(o, path, name, FALSE);
}

static ret_t csv_file_object_remove_prop(tk_object_t* obj, const char* name) {
  csv_path_t p;
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_start_with(name, CSV_QUERY_PREFIX)) {
    return tk_object_remove_prop(o->query_args, name);
  }

  return_value_if_fail(csv_path_parse_ex(o, &p, name, TRUE) == RET_OK, RET_FAIL);

  return csv_file_remove_row(o->csv, p.row);
}

static ret_t csv_file_object_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  csv_path_t p;
  uint32_t rows = 0;
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_start_with(name, CSV_QUERY_PREFIX)) {
    return tk_object_set_prop(o->query_args, name, v);
  }

  rows = csv_file_get_rows(o->csv);
  if (rows <= 0) {
    return RET_NOT_FOUND;
  }

  return_value_if_fail(csv_path_parse(o, &p, name) == RET_OK, RET_FAIL);
  if (p.col_name != NULL && tk_str_ieq(p.col_name, TK_OBJECT_PROP_CHECKED)) {
    return csv_file_set_row_checked(o->csv, p.row, value_bool(v));
  }

  str_from_value(&(o->str), v);
  return csv_file_set(o->csv, p.row, p.col, o->str.str);
}

static ret_t csv_file_object_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  csv_path_t p;
  uint32_t rows = 0;
  const char* str = NULL;
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_start_with(name, CSV_QUERY_PREFIX)) {
    return tk_object_get_prop(o->query_args, name, v);
  }

  rows = csv_file_get_rows(o->csv);
  if (tk_str_ieq(name, TK_OBJECT_PROP_SIZE)) {
    if (o->rows_map != NULL) {
      value_set_int(v, o->rows_map_size);
    } else {
      value_set_int(v, rows);
    }
    return RET_OK;
  } else if (tk_str_eq(name, TK_OBJECT_PROP_SELECTED_INDEX)) {
    value_set_int(v, csv_file_get_first_checked(o->csv));
    return RET_OK;
  }

  if (rows <= 0) {
    return RET_NOT_FOUND;
  }

  return_value_if_fail(csv_path_parse(o, &p, name) == RET_OK, RET_FAIL);
  if (p.col_name != NULL && tk_str_ieq(p.col_name, TK_OBJECT_PROP_CHECKED)) {
    return_value_if_fail(p.row < rows, RET_FAIL);

    value_set_bool(v, csv_file_is_row_checked(o->csv, p.row));
    return RET_OK;
  }

  value_set_str(v, "");
  str = csv_file_get(o->csv, p.row, p.col);
  return_value_if_fail(str != NULL, RET_FAIL);
  value_set_str(v, str);

  return RET_OK;
}

static bool_t csv_file_object_can_exec(tk_object_t* obj, const char* name, const char* args) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, TK_OBJECT_CMD_SAVE)) {
    return TRUE;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_RELOAD)) {
    return TRUE;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_CLEAR)) {
    return TRUE;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_REMOVE)) {
    return TRUE;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_REMOVE_CHECKED)) {
    return csv_file_get_checked_rows(o->csv) > 0;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_ADD)) {
    return TRUE;
  } else if (tk_str_eq(name, CSV_CMD_QUERY)) {
    return TRUE;
  }

  return FALSE;
}

static ret_t csv_file_object_prepare(csv_file_object_t* o) {
  o->rows_map_size = 0;
  if (o->rows_map != NULL) {
    if (o->rows_map_capacity < csv_file_get_rows(o->csv)) {
      TKMEM_FREE(o->rows_map);
      o->rows_map = NULL;
      o->rows_map_capacity = 0;
    }
  }

  if (o->rows_map == NULL) {
    o->rows_map_capacity = csv_file_get_rows(o->csv);
    o->rows_map = TKMEM_ZALLOCN(uint32_t, o->rows_map_capacity);
  }

  return o->rows_map != NULL ? RET_OK : RET_OOM;
}

static ret_t csv_file_object_clear_query(tk_object_t* obj) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->rows_map_size = 0;
  TKMEM_FREE(o->rows_map);
  o->rows_map_capacity = 0;

  return RET_OK;
}

static ret_t csv_file_object_query(tk_object_t* obj) {
  uint32_t i = 0;
  uint32_t n = 0;
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  return_value_if_fail(o->filter != NULL, RET_BAD_PARAMS);
  return_value_if_fail(csv_file_object_prepare(o) == RET_OK, RET_OOM);

  n = csv_file_get_rows(o->csv);
  for (i = 0; i < n; i++) {
    csv_row_t* row = csv_file_get_row(o->csv, i);
    ret_t ret = o->filter(o->filter_ctx, o->query_args, i, row);

    if (ret == RET_OK) {
      o->rows_map[o->rows_map_size++] = i;
    } else if (ret == RET_STOP) {
      break;
    }
  }

  return RET_OK;
}

ret_t csv_file_object_set_filter(tk_object_t* obj, csv_file_object_filter_t filter, void* ctx) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->filter = filter;
  o->filter_ctx = ctx;

  return RET_OK;
}

static ret_t csv_file_object_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_NOT_IMPL;
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, TK_OBJECT_CMD_SAVE)) {
    ret = csv_file_save(o->csv, NULL);
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_RELOAD)) {
    csv_file_reload(o->csv);
    ret = RET_ITEMS_CHANGED;
    csv_file_object_clear_query(obj);
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_CLEAR)) {
    csv_file_clear(o->csv);
    ret = RET_ITEMS_CHANGED;
    csv_file_object_clear_query(obj);
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_REMOVE)) {
    ret = csv_file_object_remove_prop(obj, args) == RET_OK ? RET_ITEMS_CHANGED : RET_FAIL;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_REMOVE_CHECKED)) {
    ret = csv_file_remove_checked_rows(o->csv) == RET_OK ? RET_ITEMS_CHANGED : RET_FAIL;
    csv_file_object_clear_query(obj);
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_ADD)) {
    return_value_if_fail(args != NULL, RET_FAIL);
    ret = csv_file_append_row(o->csv, args) == RET_OK ? RET_ITEMS_CHANGED : RET_FAIL;
  } else if (tk_str_eq(name, CSV_CMD_QUERY)) {
    if (tk_str_eq(args, CSV_CMD_QUERY_ARG_CLEAR)) {
      ret = csv_file_object_clear_query(obj);
    } else {
      ret = csv_file_object_query(obj);
    }
    ret = RET_ITEMS_CHANGED;
  } else {
    return RET_NOT_IMPL;
  }

  if (ret == RET_ITEMS_CHANGED) {
    emitter_dispatch_simple_event(EMITTER(obj), EVT_PROPS_CHANGED);
    emitter_dispatch_simple_event(EMITTER(obj), EVT_ITEMS_CHANGED);
  }

  return RET_OK;
}

static ret_t csv_file_object_destroy(tk_object_t* obj) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  csv_file_destroy(o->csv);
  o->csv = NULL;
  TK_OBJECT_UNREF(o->query_args);
  TKMEM_FREE(o->rows_map);
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

static csv_file_object_t* csv_file_object_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_csv_file_object_vtable, NULL);

  return (csv_file_object_t*)obj;
}

tk_object_t* csv_file_object_create(csv_file_t* csv) {
  tk_object_t* obj = NULL;
  csv_file_object_t* o = NULL;
  return_value_if_fail(csv != NULL, NULL);

  obj = tk_object_create(&s_csv_file_object_vtable);
  o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, NULL);

  o->csv = csv;
  o->query_args = object_default_create_ex(FALSE);
  str_init(&(o->str), 0);

  return obj;
}

csv_file_t* csv_file_object_get_csv(tk_object_t* obj) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, NULL);

  return o->csv;
}

tk_object_t* csv_file_object_load(const char* filename, char sep) {
  csv_file_t* csv = csv_file_create(filename, sep);
  return_value_if_fail(csv != NULL, NULL);

  return csv_file_object_create(csv);
}

tk_object_t* csv_file_object_load_from_buff(const void* buff, uint32_t size, char sep) {
  csv_file_t* csv = NULL;
  if (buff != NULL) {
    csv = csv_file_create_with_buff(buff, size, sep);
  } else {
    csv = csv_file_create_empty(sep, NULL, NULL);
  }
  return_value_if_fail(csv != NULL, NULL);

  return csv_file_object_create(csv);
}

ret_t csv_file_object_save_to_buff(tk_object_t* obj, wbuffer_t* wb) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return csv_file_save_to_buff(o->csv, wb);
}

ret_t csv_file_object_save_as(tk_object_t* obj, const char* filename) {
  csv_file_object_t* o = CSV_FILE_OBJECT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  return csv_file_save(o->csv, filename);
}
