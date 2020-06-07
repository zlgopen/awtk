/**
 * File:   ini.c
 * Author: AWTK Develop Team
 * Brief:  ini 
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
 * 2020-05-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "conf_io/conf_ini.h"

typedef enum _parser_state_t {
  STATE_NONE = 0,
  STATE_BEFORE_GROUP,
  STATE_GROUP,
  STATE_KEY,
  STATE_BEFORE_VALUE,
  STATE_VALUE,
  STATE_COMMENT
} parser_state_t;

conf_doc_t* conf_doc_load_ini(const char* data) {
  str_t str;
  value_t v;
  str_t* s = NULL;
  char c = 0;
  char last_c = 0;
  const char* p = data;
  conf_doc_t* doc = NULL;
  conf_node_t* node = NULL;
  conf_node_t* group = NULL;
  parser_state_t state = STATE_NONE;
  return_value_if_fail(data != NULL, NULL);

  doc = conf_doc_create(100);
  return_value_if_fail(doc != NULL, NULL);

  doc->max_deep_level = 2;
  doc->root = conf_doc_create_node(doc, CONF_NODE_ROOT_NAME);
  if (doc->root == NULL) {
    TKMEM_FREE(doc);
    return NULL;
  }

  s = str_init(&str, 64);
  return_value_if_fail(s != NULL, NULL);

  while (*p) {
    last_c = c;
    c = *p++;

    if (c == '\\') {
      if (last_c != '\\') {
        continue;
      } else {
        last_c = 0;
      }
    }

    switch (state) {
      case STATE_NONE: {
        if (c == '[') {
          state = STATE_BEFORE_GROUP;
        } else if (c == '#') {
          state = STATE_COMMENT;
        } else if (!isspace(c)) {
          state = STATE_KEY;
          str_set_with_len(s, &c, 1);
        }
        break;
      }
      case STATE_COMMENT: {
        if (c == '\r' || c == '\n') {
          state = STATE_NONE;
        }
        break;
      }
      case STATE_BEFORE_GROUP: {
        if (!isspace(c)) {
          state = STATE_GROUP;
          str_set_with_len(s, &c, 1);
        }
        break;
      }
      case STATE_GROUP: {
        if (c == ']') {
          state = STATE_NONE;
          str_trim_right(s, " \t");
          group = conf_doc_create_node(doc, s->str);
          return_value_if_fail(group != NULL, doc);
          conf_doc_append_child(doc, doc->root, group);
        } else {
          str_append_char(s, c);
        }
        break;
      }
      case STATE_KEY: {
        if (c == '=') {
          state = STATE_BEFORE_VALUE;
          str_trim_right(s, " \t");
          node = conf_doc_create_node(doc, s->str);
          return_value_if_fail(node != NULL, doc);
          if (group == NULL) {
            conf_doc_append_child(doc, doc->root, node);
          } else {
            conf_doc_append_child(doc, group, node);
          }
        } else {
          str_append_char(s, c);
        }
        break;
      }
      case STATE_BEFORE_VALUE: {
        if (!isspace(c)) {
          state = STATE_VALUE;
          str_set_with_len(s, &c, 1);
        }

        break;
      }
      case STATE_VALUE: {
        if (last_c == '\\') {
          str_append_char(s, c);
          continue;
        }

        if (c == '#' || c == '\n') {
          str_trim_right(s, " \t\r\n");
          value_set_str(&v, s->str);
          conf_node_set_value(node, &v);

          if (c == '#') {
            state = STATE_COMMENT;
          } else {
            state = STATE_NONE;
          }
        } else {
          str_append_char(s, c);
        }
        break;
      }
    }
  }

  if (state == STATE_VALUE) {
    str_trim_right(s, " \t\r\n");
    value_set_str(&v, s->str);
    conf_node_set_value(node, &v);
  }

  str_reset(&str);

  return doc;
}

static ret_t conf_doc_save_value(const value_t* v, str_t* str) {
  char buff[32];

  switch (v->type) {
    case VALUE_TYPE_STRING: {
      const char* p = value_str(v);
      while (*p) {
        if (*p == '#' || *p == '\\' || *p == '\n') {
          return_value_if_fail(str_append_char(str, '\\') == RET_OK, RET_OOM);
        }
        return_value_if_fail(str_append_char(str, *p) == RET_OK, RET_OOM);
        p++;
      }
      return RET_OK;
    }

    case VALUE_TYPE_FLOAT32: {
      tk_snprintf(buff, sizeof(buff) - 1, "%f", value_float32(v));
      break;
    }
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_DOUBLE: {
      tk_snprintf(buff, sizeof(buff) - 1, "%lf", value_double(v));
      break;
    }
    case VALUE_TYPE_INT64: {
      tk_snprintf(buff, sizeof(buff) - 1, "%lld", value_int64(v));
      break;
    }
    case VALUE_TYPE_UINT64: {
      tk_snprintf(buff, sizeof(buff) - 1, "%llu", value_uint64(v));
      break;
    }
    default: {
      tk_snprintf(buff, sizeof(buff) - 1, "%d", value_int(v));
      break;
    }
  }

  return str_append(str, buff);
}

static ret_t conf_doc_save_leaf_node(conf_node_t* node, str_t* str) {
  value_t v;
  const char* key = conf_node_get_name(node);

  conf_node_get_value(node, &v);

  return_value_if_fail(str_append_more(str, "  ", key, " = ", NULL) == RET_OK, RET_OOM);
  return_value_if_fail(conf_doc_save_value(&v, str) == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, "\n") == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t conf_doc_save_ini(conf_doc_t* doc, str_t* str) {
  conf_node_t* iter = NULL;
  conf_node_t* group = NULL;
  return_value_if_fail(doc != NULL && str != NULL, RET_BAD_PARAMS);
  str_set(str, "");

  group = conf_node_get_first_child(doc->root);

  while (group != NULL) {
    if (group->value_type != CONF_NODE_VALUE_NODE && group->value_type != CONF_NODE_VALUE_NONE) {
      conf_doc_save_leaf_node(group, str);
    } else {
      const char* group_name = conf_node_get_name(group);
      return_value_if_fail(str_append_more(str, "[", group_name, "]\n", NULL) == RET_OK, RET_OOM);
      iter = conf_node_get_first_child(group);
      while (iter != NULL) {
        conf_doc_save_leaf_node(iter, str);
        iter = iter->next;
      }
    }

    group = group->next;
  }

  return RET_OK;
}

static conf_doc_t* conf_doc_load_ini_reader(data_reader_t* reader) {
  char* data = NULL;
  int32_t rsize = 0;
  conf_doc_t* doc = NULL;
  uint64_t size = data_reader_get_size(reader);
  return_value_if_fail(reader != NULL && size > 0, NULL);
  data = TKMEM_ALLOC(size + 1);
  return_value_if_fail(data != NULL, NULL);

  memset(data, 0x00, size + 1);
  rsize = data_reader_read(reader, 0, data, size);
  if (rsize > 0) {
    doc = conf_doc_load_ini(data);
  }
  TKMEM_FREE(data);

  return doc;
}

static ret_t conf_doc_save_ini_writer(conf_doc_t* doc, data_writer_t* writer) {
  str_t str;
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(str_init(&str, 1024) != NULL);
  goto_error_if_fail(conf_doc_save_ini(doc, &str) == RET_OK);
  goto_error_if_fail(data_writer_write(writer, 0, str.str, str.size) == str.size);
  str_reset(&str);

  return RET_OK;

error:
  str_reset(&str);

  return RET_FAIL;
}

object_t* conf_ini_load(const char* url, bool_t create_if_not_exist) {
  return conf_obj_create(conf_doc_save_ini_writer, conf_doc_load_ini_reader, url,
                         create_if_not_exist);
}
