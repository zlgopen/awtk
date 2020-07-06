/**
 * File:   conf_json.c
 * Author: AWTK Develop Team
 * Brief:  json 
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
 * 2020-05-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "conf_io/conf_json.h"

typedef enum _parser_state_t {
  STATE_NONE = 0,
  STATE_IN_NAME,
  STATE_IN_VALUE_NUMBER,
  STATE_IN_VALUE_STR,
  STATE_GROUP,
  STATE_BEFORE_KEY,
  STATE_KEY,
  STATE_BEFORE_VALUE,
  STATE_AFTER_VALUE
} parser_state_t;

typedef struct _json_parser_t {
  const char* data;
  uint32_t size;
  uint32_t cursor;

  str_t str;
  conf_doc_t* doc;
  conf_node_t* node;
  conf_node_t* current;
} json_parser_t;

static ret_t conf_json_parse_value(json_parser_t* parser);
static ret_t conf_json_skip_to_char(json_parser_t* parser, char c);

static ret_t conf_json_parse_name(json_parser_t* parser) {
  str_t* s = &(parser->str);
  conf_node_t* node = NULL;
  uint32_t i = parser->cursor;
  const char* p = parser->data;

  return_value_if_fail(conf_json_skip_to_char(parser, '\"') == RET_OK, RET_BAD_PARAMS);
  i = ++(parser->cursor);
  while (i < parser->size && p[i] != '\"') {
    i++;
  }

  str_set_with_len(s, parser->data + parser->cursor, i - parser->cursor);
  parser->cursor = i + 1;
  node = conf_doc_create_node(parser->doc, s->str);
  return_value_if_fail(node != NULL, RET_OOM);

  conf_doc_append_child(parser->doc, parser->current, node);
  parser->current = node;

  return RET_OK;
}

static ret_t conf_json_skip_to_char(json_parser_t* parser, char c) {
  while (parser->cursor < parser->size) {
    if (parser->data[parser->cursor] == '\\') {
      parser->cursor += 2;
      continue;
    }
    if (parser->data[parser->cursor] == c) {
      break;
    }
    parser->cursor++;
  }

  return parser->cursor < parser->size ? RET_OK : RET_BAD_PARAMS;
}

static ret_t conf_json_skip_to_value_end(json_parser_t* parser) {
  while (parser->cursor < parser->size) {
    char c = parser->data[parser->cursor];

    if (c == ',' || c == '}' || c == ']') {
      break;
    }
    parser->cursor++;
  }

  return parser->cursor < parser->size ? RET_OK : RET_BAD_PARAMS;
}

static ret_t conf_json_skip_spaces(json_parser_t* parser) {
  while (parser->cursor < parser->size) {
    if (!isspace(parser->data[parser->cursor])) {
      break;
    }
    parser->cursor++;
  }

  return parser->cursor < parser->size ? RET_OK : RET_BAD_PARAMS;
}

static ret_t conf_json_parse_object(json_parser_t* parser) {
  char c = '\0';

  return_value_if_fail(conf_json_skip_spaces(parser) == RET_OK, RET_BAD_PARAMS);
  parser->cursor++;
  while (parser->cursor < parser->size) {
    return_value_if_fail(conf_json_skip_spaces(parser) == RET_OK, RET_BAD_PARAMS);

    if (parser->data[parser->cursor] == '}') {
      parser->cursor++;
      break;
    }

    return_value_if_fail(conf_json_parse_name(parser) == RET_OK, RET_FAIL);
    return_value_if_fail(conf_json_skip_to_char(parser, ':') == RET_OK, RET_BAD_PARAMS);
    parser->cursor++;
    return_value_if_fail(conf_json_parse_value(parser) == RET_OK, RET_FAIL);
    parser->current = parser->current->parent;

    return_value_if_fail(conf_json_skip_spaces(parser) == RET_OK, RET_BAD_PARAMS);
    c = parser->data[parser->cursor];
    if (c == '}') {
      parser->cursor++;
      break;
    } else if (c != ',') {
      log_debug("unexpected char %c at %s\n", c, parser->data + parser->cursor);
    }
  }

  return RET_OK;
}

static ret_t conf_json_parse_array(json_parser_t* parser) {
  char c = '\0';
  uint32_t i = 0;
  char name[TK_NAME_LEN + 1];
  conf_node_t* node = NULL;

  parser->cursor++;

  while (parser->cursor < parser->size) {
    tk_snprintf(name, TK_NAME_LEN, "%u", i++);
    node = conf_doc_create_node(parser->doc, name);
    return_value_if_fail(node != NULL, RET_OOM);
    conf_doc_append_child(parser->doc, parser->current, node);
    parser->current = node;
    return_value_if_fail(conf_json_parse_value(parser) == RET_OK, RET_FAIL);
    parser->current = node->parent;

    conf_json_skip_spaces(parser);
    c = parser->data[parser->cursor];
    if (c == ']') {
      parser->cursor++;
      break;
    } else if (c == ',') {
      parser->cursor++;
      continue;
    } else {
      log_debug("unexpected char %c at %s\n", c, parser->data + parser->cursor);
    }
  }

  return RET_OK;
}

static ret_t conf_json_parse_bool(json_parser_t* parser) {
  value_t v;
  char c = parser->data[parser->cursor];

  conf_json_skip_to_value_end(parser);
  value_set_bool(&v, (c == 't' || c == 'T'));

  return conf_node_set_value(parser->current, &v);
}

static ret_t conf_json_parse_number(json_parser_t* parser) {
  value_t v;
  str_t* s = &(parser->str);
  uint32_t start = parser->cursor;

  conf_json_skip_to_value_end(parser);
  str_set_with_len(s, parser->data + start, parser->cursor - start);
  value_set_double(&v, tk_atof(s->str));

  return conf_node_set_value(parser->current, &v);
}

static ret_t conf_json_parse_string(json_parser_t* parser) {
  value_t v;
  uint32_t start = 0;
  str_t* s = &(parser->str);

  return_value_if_fail(conf_json_skip_to_char(parser, '\"') == RET_OK, RET_BAD_PARAMS);
  start = ++(parser->cursor);
  return_value_if_fail(conf_json_skip_to_char(parser, '\"') == RET_OK, RET_BAD_PARAMS);

  str_set_with_len(s, parser->data + start, parser->cursor - start);
  conf_json_skip_to_value_end(parser);

  str_unescape(s);
  value_set_str(&v, s->str);

  return conf_node_set_value(parser->current, &v);
}

static ret_t conf_json_parse_value(json_parser_t* parser) {
  char c = 0;
  conf_node_t* node = NULL;
  const char* p = parser->data;
  conf_doc_t* doc = parser->doc;

  for (; parser->cursor < parser->size; parser->cursor++) {
    c = p[parser->cursor];

    if (c == '{' || c == '[') {
      if (doc->root == NULL) {
        node = conf_doc_create_node(doc, CONF_NODE_ROOT_NAME);
        return_value_if_fail(node != NULL, RET_OOM);
        doc->root = node;
        parser->current = node;
      }

      if (c == '[') {
        parser->current->node_type = CONF_NODE_ARRAY;
        return conf_json_parse_array(parser);
      } else {
        return conf_json_parse_object(parser);
      }
    } else if (!isspace(c)) {
      if (c == '\"') {
        return conf_json_parse_string(parser);
      } else if (c == 't' || c == 'f') {
        return conf_json_parse_bool(parser);
      } else {
        return conf_json_parse_number(parser);
      }
    }
  }

  return RET_OK;
}

conf_doc_t* conf_doc_load_json(const char* data, int32_t size) {
  json_parser_t parser;
  return_value_if_fail(data != NULL, NULL);
  size = size < 0 ? strlen(data) : size;

  memset(&parser, 0x00, sizeof(parser));

  parser.doc = conf_doc_create(((size) / sizeof(conf_node_t)) + 5);
  return_value_if_fail(parser.doc != NULL, NULL);

  if (str_init(&(parser.str), 32) == NULL) {
    conf_doc_destroy(parser.doc);
    return NULL;
  }

  parser.data = data;
  parser.size = size;

  conf_json_parse_value(&parser);
  str_reset(&(parser.str));

  return parser.doc;
}

static ret_t conf_json_save_node(conf_node_t* node, str_t* str, uint32_t level);
static ret_t conf_json_save_node_value(conf_node_t* node, str_t* str, uint32_t level);
static ret_t conf_json_save_node_children(conf_node_t* node, str_t* str, uint32_t level);

static ret_t conf_json_save_node_value_object(conf_node_t* node, str_t* str, uint32_t level) {
  return conf_json_save_node_children(node, str, level);
}

static ret_t conf_json_save_node_value_array(conf_node_t* node, str_t* str, uint32_t level) {
  return conf_json_save_node_children(node, str, level);
}

static ret_t conf_json_save_node_value_simple(conf_node_t* node, str_t* str, uint32_t level) {
  value_t v;
  return_value_if_fail(conf_node_get_value(node, &v) == RET_OK, RET_BAD_PARAMS);

  if (v.type == VALUE_TYPE_BOOL) {
    if (value_bool(&v)) {
      return str_append(str, "true");
    } else {
      return str_append(str, "false");
    }
  } else if (v.type == VALUE_TYPE_STRING) {
    const char* s = value_str(&v);
    return_value_if_fail(str_append_char(str, '\"') == RET_OK, RET_OOM);
    if (s != NULL) {
      while (*s) {
        if (*s == '\"') {
          return_value_if_fail(str_append_char(str, '\\') == RET_OK, RET_OOM);
        }
        return_value_if_fail(str_append_char(str, *s) == RET_OK, RET_OOM);
        s++;
      }
    }
    return_value_if_fail(str_append_char(str, '\"') == RET_OK, RET_OOM);

    return RET_OK;
  } else {
    double d = value_double(&v);
    if (d == (int64_t)d) {
      return str_append_int(str, (int)d);
    } else {
      return str_append_double(str, "%.4f", value_double(&v));
    }
  }
}

static ret_t conf_json_save_node_value(conf_node_t* node, str_t* str, uint32_t level) {
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);
  if (node->node_type == CONF_NODE_OBJECT) {
    return conf_json_save_node_value_object(node, str, level);
  } else if (node->node_type == CONF_NODE_ARRAY) {
    return conf_json_save_node_value_array(node, str, level);
  } else {
    return conf_json_save_node_value_simple(node, str, level);
  }
}

static ret_t conf_json_write_indent(str_t* str, int32_t level) {
  uint32_t i = 0;
  if (level < 0) {
    return RET_OK;
  }

  for (i = 0; i < level; i++) {
    return_value_if_fail(str_append(str, "    ") == RET_OK, RET_OOM);
  }

  return RET_OK;
}

static ret_t conf_json_save_node(conf_node_t* node, str_t* str, uint32_t level) {
  const char* name = conf_node_get_name(node);
  return_value_if_fail(conf_json_write_indent(str, level) == RET_OK, RET_OOM);
  return_value_if_fail(str_append_more(str, "\"", name, "\" : ", NULL) == RET_OK, RET_OOM);

  return conf_json_save_node_value(node, str, level);
}

static ret_t conf_json_save_node_children_object(conf_node_t* node, str_t* str, uint32_t level) {
  conf_node_t* iter = conf_node_get_first_child(node);
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_append(str, "{\n") == RET_OK, RET_OOM);

  while (iter != NULL) {
    return_value_if_fail(conf_json_save_node(iter, str, level + 1) == RET_OK, RET_OOM);
    iter = iter->next;
    if (iter != NULL) {
      return_value_if_fail(str_append(str, ",\n") == RET_OK, RET_OOM);
    }
  }

  return_value_if_fail(str_append(str, "\n") == RET_OK, RET_OOM);
  return_value_if_fail(conf_json_write_indent(str, level) == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, "}") == RET_OK, RET_OOM);

  return RET_OK;
}

static ret_t conf_json_save_node_children_array(conf_node_t* node, str_t* str, uint32_t level) {
  conf_node_t* iter = conf_node_get_first_child(node);
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_append(str, "[\n") == RET_OK, RET_OOM);

  while (iter != NULL) {
    return_value_if_fail(conf_json_write_indent(str, level + 1) == RET_OK, RET_OOM);
    return_value_if_fail(conf_json_save_node_value(iter, str, level + 1) == RET_OK, RET_OOM);
    iter = iter->next;
    if (iter != NULL) {
      return_value_if_fail(str_append(str, ",\n") == RET_OK, RET_OOM);
    }
  }

  return_value_if_fail(str_append(str, "\n") == RET_OK, RET_OOM);
  return_value_if_fail(conf_json_write_indent(str, level) == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, "]") == RET_OK, RET_OOM);

  return RET_OK;
}

static ret_t conf_json_save_node_children(conf_node_t* node, str_t* str, uint32_t level) {
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);

  if (node->node_type == CONF_NODE_OBJECT) {
    return conf_json_save_node_children_object(node, str, level);
  } else if (node->node_type == CONF_NODE_ARRAY) {
    return conf_json_save_node_children_array(node, str, level);
  } else {
    assert(!"invalid type");
    return RET_FAIL;
  }
}

ret_t conf_doc_save_json(conf_doc_t* doc, str_t* str) {
  return_value_if_fail(doc != NULL && doc->root != NULL && str != NULL, RET_BAD_PARAMS);

  str_set(str, "");
  return conf_json_save_node_children(doc->root, str, 0);
}

static conf_doc_t* conf_doc_load_json_reader(data_reader_t* reader) {
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
    doc = conf_doc_load_json(data, rsize);
  }
  TKMEM_FREE(data);

  return doc;
}

static ret_t conf_doc_save_json_writer(conf_doc_t* doc, data_writer_t* writer) {
  str_t str;
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(str_init(&str, 1024) != NULL);
  goto_error_if_fail(conf_doc_save_json(doc, &str) == RET_OK);
  goto_error_if_fail(data_writer_write(writer, 0, str.str, str.size) == str.size);
  str_reset(&str);

  return RET_OK;

error:
  str_reset(&str);

  return RET_FAIL;
}

object_t* conf_json_load(const char* url, bool_t create_if_not_exist) {
  return conf_obj_create(conf_doc_save_json_writer, conf_doc_load_json_reader, url,
                         create_if_not_exist);
}
