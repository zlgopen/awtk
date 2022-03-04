/**
 * File:   csv_file.c
 * Author: AWTK Develop Team
 * Brief:  csv file
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
 * 2020-06-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"

#include "csv_file.h"
#include "streams/mem/istream_mem.h"
#include "streams/file/istream_file.h"

static const uint8_t s_utf8_bom[3] = {0xEF, 0xBB, 0xBF};

ret_t csv_file_reset(csv_file_t* csv);
ret_t csv_file_clear(csv_file_t* csv);
static ret_t csv_rows_extend_rows(csv_rows_t* rows, uint32_t delta);
static csv_file_t* csv_file_load_input(csv_file_t* csv, tk_istream_t* input);

const char* csv_row_get(csv_row_t* row, uint32_t col) {
  uint32_t i = 0;
  uint32_t c = 0;
  return_value_if_fail(row != NULL, NULL);

  while (i < row->size) {
    const char* p = row->buff + i;
    if (c == col) {
      return p;
    }

    c++;
    i += strlen(p) + 1;
  }

  return NULL;
}

int32_t csv_row_get_col(csv_row_t* row, const char* value) {
  uint32_t i = 0;
  uint32_t col = 0;
  return_value_if_fail(row != NULL && value != NULL, -1);

  while (i < row->size) {
    const char* p = row->buff + i;
    if (tk_str_eq(p, value)) {
      return col;
    }

    col++;
    i += strlen(p) + 1;
  }

  return -1;
}

static ret_t csv_col_to_str(str_t* str, const char* data, char sep) {
  const char* p = data;
  bool_t need_escape = strchr(p, sep) != NULL || strchr(p, '\"') != NULL;

  if (need_escape) {
    str_append_char(str, '\"');
    while (*p) {
      if (*p == '\\' || *p == '\"') {
        str_append_char(str, '\\');
      }
      str_append_char(str, *p);
      p++;
    }
    str_append_char(str, '\"');
  } else {
    str_append(str, data);
  }

  return RET_OK;
}

ret_t csv_row_to_str(csv_row_t* row, str_t* str, char sep) {
  uint32_t i = 0;
  return_value_if_fail(row != NULL && str != NULL, RET_BAD_PARAMS);

  str_set(str, "");
  while (i < row->size) {
    const char* p = row->buff + i;
    csv_col_to_str(str, p, sep);
    str_append_char(str, sep);

    i += strlen(p) + 1;
  }

  if (str->size > 0) {
    /*remove last seperator*/
    str->size--;
    str->str[str->size] = '\0';
  }

  str_append(str, "\r\n");

  return RET_OK;
}

uint32_t csv_row_count_cols(csv_row_t* row) {
  uint32_t i = 0;
  uint32_t cols = 0;
  return_value_if_fail(row != NULL, 0);

  while (i < row->size) {
    const char* p = row->buff + i;
    cols++;
    i += strlen(p) + 1;
  }

  return cols;
}

ret_t csv_row_set(csv_row_t* row, uint32_t col, const char* value) {
  char* p = NULL;
  uint32_t old_len = 0;
  uint32_t new_len = 0;
  return_value_if_fail(row != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(row->buff != NULL, RET_BAD_PARAMS);
  p = (char*)csv_row_get(row, col);
  return_value_if_fail(p != NULL, RET_BAD_PARAMS);

  old_len = strlen(p);
  new_len = strlen(value);

  if (old_len == new_len) {
    strcpy(p, value);
    return RET_OK;
  } else if (old_len > new_len) {
    uint32_t len = row->size - (p + old_len - row->buff);
    strcpy(p, value);
    memmove(p + new_len, p + old_len, len);
    row->size = row->size + new_len - old_len;
    return RET_OK;
  } else {
    uint32_t d = 0;
    uint32_t s = 0;
    uint32_t size = 0;
    uint32_t len = row->size + new_len - old_len;
    char* buff = TKMEM_ALLOC(len);
    return_value_if_fail(buff != NULL, RET_OOM);

    memset(buff, 0x00, len);
    size = p - row->buff;
    memcpy(buff, row->buff, size);

    d = size;
    s = size;
    memcpy(buff + d, value, new_len + 1);

    d += new_len + 1;
    s += old_len + 1;
    size = row->size - s;
    memcpy(buff + d, row->buff + s, size);

    csv_row_reset(row);

    row->buff = buff;
    row->size = len;
    row->should_free_buff = TRUE;
    return RET_OK;
  }
}

ret_t csv_row_init(csv_row_t* row, char* buff, uint32_t size, bool_t should_free_buff) {
  return_value_if_fail(row != NULL && buff != NULL, RET_BAD_PARAMS);

  row->buff = buff;
  row->size = size;
  row->should_free_buff = should_free_buff;

  return RET_OK;
}

static ret_t csv_row_parse(csv_row_t* row, char sep) {
  char* s = row->buff;
  char* d = row->buff;
  bool_t escape = FALSE;
  bool_t in_quota = FALSE;

  while (*s) {
    char c = *s;

    s++;
    if (in_quota) {
      if (escape) {
        *d++ = c;
        escape = FALSE;
      } else {
        if (c == '\"') {
          in_quota = FALSE;
        } else if (c == '\\') {
          escape = TRUE;
        } else {
          *d++ = c;
        }
      }
    } else {
      if (c == sep) {
        *d++ = '\0';
      } else if (c == '\"') {
        in_quota = TRUE;
      } else {
        *d++ = c;
      }
    }
  }
  *d = '\0';
  row->size = d - row->buff + 1;

  return RET_OK;
}

ret_t csv_row_set_data(csv_row_t* row, const char* data, char sep) {
  csv_row_reset(row);

  row->buff = tk_strdup(data);
  return_value_if_fail(row->buff != NULL, RET_OOM);

  row->should_free_buff = TRUE;
  row->size = strlen(data) + 1;

  return csv_row_parse(row, sep);
}

ret_t csv_row_reset(csv_row_t* row) {
  return_value_if_fail(row != NULL, RET_BAD_PARAMS);
  if (row->should_free_buff) {
    TKMEM_FREE(row->buff);
  }
  memset(row, 0x00, sizeof(*row));

  return RET_OK;
}

static ret_t csv_rows_extend_rows(csv_rows_t* rows, uint32_t delta) {
  uint32_t capacity = 0;
  csv_row_t* first = NULL;
  return_value_if_fail(rows != NULL, RET_BAD_PARAMS);

  if ((rows->size + delta) < rows->capacity) {
    return RET_OK;
  }

  first = rows->rows;
  capacity = rows->capacity * 1.5 + delta;
  first = TKMEM_REALLOC(first, capacity * sizeof(csv_row_t));
  return_value_if_fail(first != NULL, RET_OOM);

  rows->rows = first;
  rows->capacity = capacity;

  return RET_OK;
}

ret_t csv_rows_remove(csv_rows_t* rows, uint32_t row) {
  uint32_t i = 0;
  csv_row_t* r = NULL;
  return_value_if_fail(rows != NULL && row < rows->size, RET_BAD_PARAMS);

  csv_row_reset(rows->rows + row);

  r = rows->rows;
  for (i = row; i < rows->size; i++) {
    r[i] = r[i + 1];
  }
  rows->size--;

  return RET_OK;
}

csv_row_t* csv_rows_append(csv_rows_t* rows) {
  csv_row_t* r = NULL;
  return_value_if_fail(csv_rows_extend_rows(rows, 1) == RET_OK, NULL);

  r = rows->rows + rows->size++;
  memset(r, 0x00, sizeof(*r));

  return r;
}

csv_row_t* csv_rows_insert(csv_rows_t* rows, uint32_t row) {
  uint32_t i = 0;
  csv_row_t* r = NULL;
  return_value_if_fail(rows != NULL, NULL);
  return_value_if_fail(csv_rows_extend_rows(rows, 1) == RET_OK, NULL);

  if (row >= rows->size) {
    return csv_rows_append(rows);
  }

  r = rows->rows;
  for (i = rows->size; i > row; i--) {
    r[i] = r[i - 1];
  }
  r = r + row;
  memset(r, 0x00, sizeof(*r));
  rows->size++;

  return r;
}

csv_row_t* csv_rows_get(csv_rows_t* rows, uint32_t row) {
  return_value_if_fail(rows != NULL && row < rows->size, NULL);

  return rows->rows + row;
}

ret_t csv_rows_reset(csv_rows_t* rows) {
  return_value_if_fail(rows != NULL && rows->rows != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(rows->rows);
  memset(rows, 0x00, sizeof(*rows));

  return RET_OK;
}

ret_t csv_rows_init(csv_rows_t* rows, uint32_t init_capacity) {
  return_value_if_fail(rows != NULL, RET_BAD_PARAMS);

  init_capacity = tk_max(10, init_capacity);

  rows->size = 0;
  rows->capacity = init_capacity;
  rows->rows = TKMEM_ZALLOCN(csv_row_t, init_capacity);

  return rows->rows != NULL ? RET_OK : RET_OOM;
}

csv_file_t* csv_file_load(csv_file_t* csv) {
  tk_istream_t* input = NULL;
  return_value_if_fail(csv != NULL && csv->filename != NULL, NULL);

  input = tk_istream_file_create(csv->filename);
  return_value_if_fail(input != NULL, NULL);
  csv_file_load_input(csv, input);
  TK_OBJECT_UNREF(input);

  return csv;
}

static csv_file_t* csv_file_load_input(csv_file_t* csv, tk_istream_t* input) {
  str_t str;
  char* p = NULL;
  uint32_t index = 0;
  csv_row_t* r = NULL;
  char sep = csv->sep;

  str_init(&str, 2048);
  while (tk_istream_read_line_str(input, &str) == RET_OK) {
    p = str.str;
    if (csv->rows.size == 0) {
      if (memcmp(p, s_utf8_bom, sizeof(s_utf8_bom)) == 0) {
        p += sizeof(s_utf8_bom);
        log_debug("skip utf-8 bom\n");
      }
    }

    /*skip malformed data*/
    while ((p - str.str) < str.size) {
      if (*p == '\0') {
        p++;
      } else {
        break;
      }
    }

    if (*p == '\0') {
      log_debug("skip empty line\n");
      continue;
    }

    r = csv_rows_append(&(csv->rows));
    goto_error_if_fail(r != NULL);
    goto_error_if_fail(csv_row_init(r, tk_strdup(p), str.size + 1, TRUE) == RET_OK);
    csv_row_parse(r, sep);

    if (csv->filter != NULL) {
      ret_t ret = csv->filter(csv->filter_ctx, csv, index, r);
      if (ret == RET_STOP) {
        break;
      } else if (ret == RET_FAIL) {
        csv_row_reset(r);
        csv->rows.size--;
      }
    }

    index++;
  }
  str_reset(&str);

  r = csv_file_get_row(csv, 0);
  if (r != NULL) {
    uint32_t cols1 = 0;
    uint32_t cols0 = csv_row_count_cols(r);

    if (cols0 == 1) {
      r = csv_file_get_row(csv, 1);
      cols1 = csv_row_count_cols(r);
      if (cols1 > cols0) {
        csv->has_title = TRUE;
      }
      csv->cols = tk_max(cols0, cols1);
    } else {
      csv->cols = csv_row_count_cols(r);
    }
  }
  str_reset(&str);

  return csv;

error:
  str_reset(&str);
  return NULL;
}

csv_file_t* csv_file_create_empty(char sep, csv_file_filter_t filter, void* ctx) {
  csv_file_t* csv = TKMEM_ZALLOC(csv_file_t);
  return_value_if_fail(csv != NULL, NULL);

  if (csv_rows_init(&(csv->rows), 100) == RET_OK) {
    csv->sep = sep;
    csv->filter = filter;
    csv->filter_ctx = ctx;
  } else {
    TKMEM_FREE(csv);
  }

  return csv;
}

ret_t csv_file_set_filter(csv_file_t* csv, csv_file_filter_t filter, void* ctx) {
  return_value_if_fail(csv != NULL, RET_BAD_PARAMS);

  csv->filter = filter;
  csv->filter_ctx = ctx;

  return RET_OK;
}

csv_file_t* csv_file_create(const char* filename, char sep) {
  csv_file_t* csv = NULL;
  return_value_if_fail(filename != NULL, NULL);
  csv = csv_file_create_empty(sep, NULL, NULL);
  return_value_if_fail(csv != NULL, NULL);

  if (csv_file_load_file(csv, filename) != RET_OK) {
    csv_file_destroy(csv);
    csv = NULL;
  }

  return csv;
}

ret_t csv_file_load_buff(csv_file_t* csv, const char* buff, uint32_t size) {
  ret_t ret = RET_OK;
  tk_istream_t* input = NULL;

  return_value_if_fail(csv != NULL && buff != NULL && size > 0, RET_BAD_PARAMS);
  input = tk_istream_mem_create((uint8_t*)buff, size, 0, FALSE);
  return_value_if_fail(input != NULL, RET_BAD_PARAMS);

  csv_file_clear(csv);
  ret = csv_file_load_input(csv, input) != NULL ? RET_OK : RET_FAIL;
  TK_OBJECT_UNREF(input);

  return ret;
}

csv_file_t* csv_file_create_with_buff(const char* buff, uint32_t size, char sep) {
  csv_file_t* csv = NULL;
  csv = csv_file_create_empty(sep, NULL, NULL);
  return_value_if_fail(csv != NULL, NULL);

  if (csv_file_load_buff(csv, buff, size) != RET_OK) {
    csv_file_destroy(csv);
    csv = NULL;
  }

  return csv;
}

const char* csv_file_get(csv_file_t* csv, uint32_t row, uint32_t col) {
  csv_row_t* r = csv_file_get_row(csv, row);
  return_value_if_fail(r != NULL, NULL);
  return_value_if_fail(col < csv_file_get_cols(csv), NULL);

  return csv_row_get(r, col);
}

int32_t csv_file_get_col_of_name(csv_file_t* csv, const char* name) {
  return csv_row_get_col(csv_file_get_row(csv, 0), name);
}

const char* csv_file_get_by_name(csv_file_t* csv, uint32_t row, const char* name) {
  int32_t col = csv_file_get_col_of_name(csv, name);
  return_value_if_fail(col >= 0, NULL);

  return csv_file_get(csv, row, col);
}

uint32_t csv_file_get_rows(csv_file_t* csv) {
  return_value_if_fail(csv != NULL, 0);

  return csv->rows.size;
}

uint32_t csv_file_get_checked_rows(csv_file_t* csv) {
  uint32_t i = 0;
  uint32_t nr = 0;
  csv_row_t* r = NULL;
  csv_rows_t* rows = NULL;
  return_value_if_fail(csv != NULL, 0);

  rows = &(csv->rows);
  for (i = 0; i < rows->size; i++) {
    r = rows->rows + i;
    if (r->checked) {
      nr++;
    }
  }

  return nr;
}

uint32_t csv_file_get_cols(csv_file_t* csv) {
  return_value_if_fail(csv != NULL, 0);

  return csv->cols;
}

ret_t csv_file_set(csv_file_t* csv, uint32_t row, uint32_t col, const char* value) {
  csv_row_t* r = csv_file_get_row(csv, row);
  return_value_if_fail(r != NULL, RET_BAD_PARAMS);
  return_value_if_fail(col < csv_file_get_cols(csv), RET_BAD_PARAMS);

  return csv_row_set(r, col, value);
}

ret_t csv_file_set_row_checked(csv_file_t* csv, uint32_t row, bool_t checked) {
  csv_row_t* r = csv_file_get_row(csv, row);
  return_value_if_fail(r != NULL, RET_BAD_PARAMS);

  r->checked = checked;

  return RET_OK;
}

bool_t csv_file_is_row_checked(csv_file_t* csv, uint32_t row) {
  csv_row_t* r = csv_file_get_row(csv, row);
  return_value_if_fail(r != NULL, FALSE);

  return r->checked;
}

ret_t csv_file_remove_checked_rows(csv_file_t* csv) {
  uint32_t i = 0;
  uint32_t d = 0;
  csv_row_t* r = NULL;
  csv_rows_t* rows = NULL;
  return_value_if_fail(csv != NULL, RET_BAD_PARAMS);

  rows = &(csv->rows);
  for (i = 0; i < rows->size; i++) {
    r = rows->rows + i;
    if (r->checked) {
      csv_row_reset(r);
    } else {
      rows->rows[d++] = rows->rows[i];
    }
  }

  rows->size = d;

  return RET_OK;
}

csv_row_t* csv_file_get_row(csv_file_t* csv, uint32_t row) {
  return_value_if_fail(csv != NULL, NULL);

  return csv_rows_get(&(csv->rows), row);
}

ret_t csv_file_insert_row(csv_file_t* csv, uint32_t row, const char* data) {
  csv_row_t* r = NULL;
  return_value_if_fail(csv != NULL && data != NULL, RET_BAD_PARAMS);

  r = csv_rows_insert(&(csv->rows), row);
  return_value_if_fail(r != NULL, RET_OOM);

  return csv_row_set_data(r, data, csv->sep);
}

ret_t csv_file_append_row(csv_file_t* csv, const char* data) {
  csv_row_t* r = NULL;
  return_value_if_fail(csv != NULL && data != NULL, RET_BAD_PARAMS);

  r = csv_rows_append(&(csv->rows));
  return_value_if_fail(r != NULL, RET_OOM);

  return csv_row_set_data(r, data, csv->sep);
}

ret_t csv_file_remove_row(csv_file_t* csv, uint32_t row) {
  return_value_if_fail(csv != NULL, RET_BAD_PARAMS);

  return csv_rows_remove(&(csv->rows), row);
}

ret_t csv_file_save(csv_file_t* csv, const char* filename) {
  str_t str;
  uint32_t i = 0;
  csv_row_t* r = NULL;
  fs_file_t* f = NULL;
  return_value_if_fail(csv != NULL, RET_BAD_PARAMS);
  filename = filename != NULL ? filename : csv->filename;
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_init(&str, 512) != NULL, RET_OOM);

  f = fs_open_file(os_fs(), filename, "wb+");
  if (f != NULL) {
    ENSURE(fs_file_write(f, s_utf8_bom, sizeof(s_utf8_bom)) == sizeof(s_utf8_bom));

    for (i = 0; i < csv->rows.size; i++) {
      r = csv->rows.rows + i;
      csv_row_to_str(r, &str, csv->sep);
      ENSURE(fs_file_write(f, str.str, str.size) == str.size);
    }
    fs_file_close(f);

    if (csv->filename != filename) {
      TKMEM_FREE(csv->filename);
      csv->filename = tk_strdup(filename);
    }
  }
  str_reset(&str);

  return RET_NOT_IMPL;
}

const char* csv_file_get_title(csv_file_t* csv) {
  csv_row_t* r = NULL;
  return_value_if_fail(csv != NULL && csv->has_title, NULL);

  r = csv_file_get_row(csv, 0);
  return_value_if_fail(r != NULL, NULL);

  return r->buff;
}

ret_t csv_file_clear(csv_file_t* csv) {
  uint32_t i = 0;
  return_value_if_fail(csv != NULL, RET_BAD_PARAMS);

  for (i = 0; i < csv->rows.size; i++) {
    csv_row_t* r = csv->rows.rows + i;
    csv_row_reset(r);
  }
  csv->rows.size = 0;

  return RET_OK;
}

ret_t csv_file_reset(csv_file_t* csv) {
  return_value_if_fail(csv != NULL, RET_BAD_PARAMS);

  csv_file_clear(csv);
  TKMEM_FREE(csv->filename);
  TKMEM_FREE(csv->rows.rows);
  memset(csv, 0x00, sizeof(*csv));

  return RET_OK;
}

ret_t csv_file_destroy(csv_file_t* csv) {
  if (csv_file_reset(csv) == RET_OK) {
    TKMEM_FREE(csv);
  }

  return RET_OK;
}

ret_t csv_file_reload(csv_file_t* csv) {
  return_value_if_fail(csv != NULL && csv->filename != NULL, RET_BAD_PARAMS);

  csv_file_clear(csv);

  return (csv_file_load(csv) != NULL) ? RET_OK : RET_FAIL;
}

ret_t csv_file_load_file(csv_file_t* csv, const char* filename) {
  return_value_if_fail(csv != NULL && filename != NULL, RET_BAD_PARAMS);

  csv_file_clear(csv);
  csv->filename = tk_str_copy(csv->filename, filename);

  return (csv_file_load(csv) != NULL) ? RET_OK : RET_FAIL;
}
