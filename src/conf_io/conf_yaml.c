/**
 * File:   yaml.c
 * Author: AWTK Develop Team
 * Brief:  yaml 
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-04-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "conf_io/conf_yaml.h"
#include "tkc/data_reader_mem.h"
#include "tkc/data_writer_wbuffer.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"

/*root的leading_spaces为0，其它实际的节点均加上 YAML_LEADING_SPACE_OFFSET */
#define YAML_LEADING_SPACE_OFFSET 2

#define YAML_SPACES_PER_LEVEL 2
#define YAML_SEP_CHAR ':'
#define YAML_COMMENT_CHAR '#'
#define YAML_LIST_START_CHAR '-'

typedef struct _yaml_parser_t {
  str_t str;
  const char* data;
  const char* cursor;

  conf_doc_t* doc;
  conf_node_t* current_node;
} yaml_parser_t;

static ret_t yaml_parser_deinit(yaml_parser_t* parser);

static ret_t yaml_parser_init(yaml_parser_t* parser, const char* data) {
  conf_doc_t* doc = NULL;
  return_value_if_fail(parser != NULL && data != NULL, RET_BAD_PARAMS);

  memset(parser, 0x00, sizeof(*parser));
  doc = conf_doc_create(100);
  return_value_if_fail(doc != NULL, RET_OOM);

  doc->max_deep_level = 20;
  doc->root = conf_doc_create_node(doc, CONF_NODE_ROOT_NAME);
  if (doc->root == NULL) {
    TKMEM_FREE(doc);
    return RET_OOM;
  }

  parser->doc = doc;
  goto_error_if_fail(str_init(&parser->str, 128) != NULL);

  parser->data = data;
  parser->cursor = data;
  parser->current_node = doc->root;

  return RET_OK;
error:
  yaml_parser_deinit(parser);

  return RET_FAIL;
}

static ret_t yaml_parser_deinit(yaml_parser_t* parser) {
  return_value_if_fail(parser != NULL, RET_BAD_PARAMS);

  if (parser->doc != NULL) {
    conf_doc_destroy(parser->doc);
    parser->doc = NULL;
  }

  str_reset(&parser->str);
  memset(parser, 0x00, sizeof(*parser));

  return RET_OK;
}

static uint32_t yaml_parser_skip_leading_space(yaml_parser_t* parser) {
  uint32_t leading_spaces = 0;
  const char* p = parser->cursor;

  while (*p) {
    if (*p == ' ') {
      leading_spaces++;
    } else if (*p == '\t') {
      leading_spaces += YAML_SPACES_PER_LEVEL;
    } else if (*p == '\r' || *p == '\n') {
      leading_spaces = 0;
    } else {
      break;
    }
    p++;
  }

  parser->cursor = p;

  return leading_spaces;
}

static ret_t yaml_parser_skip_to_line_end(yaml_parser_t* parser) {
  parser->cursor = tk_skip_to_chars(parser->cursor, "\r\n");
  parser->cursor = tk_skip_chars(parser->cursor, "\r\n");

  return RET_OK;
}

static const char* yaml_parser_parse_value(yaml_parser_t* parser, uint32_t csep) {
  char c = '\0';
  uint32_t n = 0;
  str_t* s = &(parser->str);
  const char* p = parser->cursor;

  if (*p != csep) {
    return NULL;
  } else {
    p++;
  }

  str_clear(s);
  do {
    if (*p == '\\') {
      p++;
      if (*p == YAML_COMMENT_CHAR) {
        c = *p;
        p++;
      } else {
        c = str_unescape_char(p, &n);
        p += n;
      }
    } else if (*p == YAML_COMMENT_CHAR) {
      break;
    } else if (*p == '\r' || *p == '\n' || *p == '\0') {
      break;
    } else {
      c = *p++;
    }

    str_append_char(s, c);
  } while(1);

  parser->cursor = p;
  str_trim(s, " \t\r\n");
  yaml_parser_skip_to_line_end(parser);

  return s->str;
}

static const char* yaml_parser_parse_name(yaml_parser_t* parser) {
  const char* p = parser->cursor;
  str_t* s = &parser->str;

  str_clear(s);
  do {
    char c = *p;
    if (c == YAML_SEP_CHAR) {
      break;
    } else if (c == '\0') {
      log_warn("yaml unexpected end\n");
      break;
    } else if (c == '\r' || c == '\n') {
      log_warn("yaml unexpected line end\n");
      break;
    } else if (tk_isalpha(c) || tk_isdigit(c) || c == '_') {
      str_append_char(s, *p);
    } else {
      log_warn("yaml invalid char '%c' for name\n", c);
    }
    p++;
  } while(1);

  parser->cursor = p;
  str_trim(s, " \t\r\n");

  return s->str;
}

static conf_node_t* yaml_parser_get_parent_node(yaml_parser_t* parser, uint32_t leading_spaces) {
  conf_node_t* parent = parser->current_node;

  while (parent->leading_spaces >= leading_spaces) {
    parent = parent->parent;
  }

  return parent;
}

static ret_t yaml_parser_parse_line(yaml_parser_t* parser) {
  ret_t ret = RET_OK;
  conf_node_t* node = NULL;
  conf_node_t* parent = NULL;
  const char* name = NULL;
  const char* value = NULL;
  char index_name[32] = {0};
  bool_t is_list = FALSE;
  uint32_t leading_spaces = yaml_parser_skip_leading_space(parser) + YAML_LEADING_SPACE_OFFSET;

  parent = yaml_parser_get_parent_node(parser, leading_spaces);
  ENSURE(parent != NULL);

  if (parser->cursor[0] == YAML_COMMENT_CHAR) {
    return yaml_parser_skip_to_line_end(parser);
  } else if (parser->cursor[0] == '\r' || parser->cursor[0] == '\n') {
    return yaml_parser_skip_to_line_end(parser);
  } else if (parser->cursor[0] == '\0') {
    return RET_EOS;
  } else if (parser->cursor[0] == YAML_LIST_START_CHAR) {
    uint32_t index = conf_node_count_children(parent);
    tk_snprintf(index_name, TK_NAME_LEN, "%u", index);
    name = index_name;
    is_list = TRUE;
    parent->node_type = CONF_NODE_ARRAY;
  } else {
    name = yaml_parser_parse_name(parser);
  }

  node = conf_doc_create_node(parser->doc, name);
  return_value_if_fail(node != NULL, RET_OOM);
  node->leading_spaces = leading_spaces;

  value = yaml_parser_parse_value(parser, is_list ? YAML_LIST_START_CHAR : YAML_SEP_CHAR);
  if (value != NULL && *value) {
    value_t v;
    value_set_str(&v, value);
    conf_node_set_value(node, &v);
  }

  ret = conf_doc_append_child(parser->doc, parent, node);
  if (ret == RET_OK) {
    parser->current_node = node;
  } else {
    conf_doc_destroy_node(parser->doc, node);
  }

  return ret;
}

static ret_t yaml_parser_parse(yaml_parser_t* parser) {
  do {
    ret_t ret = yaml_parser_parse_line(parser);
    if (ret == RET_EOS) {
      break;
    } else if (ret != RET_OK) {
      return ret;
    }
  } while (1);

  return RET_OK;
}

conf_doc_t* conf_doc_load_yaml(const char* data) {
  ret_t ret = RET_OK;
  yaml_parser_t parser;
  conf_doc_t* doc = NULL;
  return_value_if_fail(yaml_parser_init(&parser, data) == RET_OK, NULL);

  ret = yaml_parser_parse(&parser);
  if (ret == RET_OK) {
    doc = parser.doc;
    parser.doc = NULL;
  }

  yaml_parser_deinit(&parser);

  return doc;
}

static ret_t conf_doc_save_yaml_node_name_value(conf_node_t* node, str_t* str, uint32_t levels) {
  value_t v;
  ret_t ret = RET_OK;
  const char* key = conf_node_get_name(node);

  if (levels > 0) {
    ret = str_append_n_chars(str, ' ', levels * 2);
    return_value_if_fail(ret == RET_OK, ret);
  }

  if (node->parent->node_type == CONF_NODE_ARRAY) {
    return_value_if_fail(str_append_char(str, YAML_LIST_START_CHAR) == RET_OK, RET_OOM);
  } else {
    return_value_if_fail(str_append(str, key) == RET_OK, RET_OOM);
    return_value_if_fail(str_append_char(str, YAML_SEP_CHAR) == RET_OK, RET_OOM);
  }

  ret = conf_node_get_value(node, &v);
  if (ret == RET_OK) {
    return_value_if_fail(str_append(str, " ") == RET_OK, RET_OOM);
    return_value_if_fail(conf_node_save_value(str, &v, YAML_COMMENT_CHAR) == RET_OK, RET_OOM);
  }
  return_value_if_fail(str_append(str, "\n") == RET_OK, RET_OOM);

  return RET_OK;
}

static ret_t conf_doc_save_yaml_node(conf_node_t* node, str_t* str, uint32_t levels) {
  return_value_if_fail(node != NULL && str != NULL, RET_BAD_PARAMS);

  while (node != NULL) {
    conf_doc_save_yaml_node_name_value(node, str, levels);

    if (node->value_type == CONF_NODE_VALUE_NODE) {
      conf_node_t* iter = conf_node_get_first_child(node);
      conf_doc_save_yaml_node(iter, str, levels + 1);
    }

    node = node->next;
  }

  return RET_OK;
}

ret_t conf_doc_save_yaml(conf_doc_t* doc, str_t* str) {
  conf_node_t* node = NULL;
  return_value_if_fail(doc != NULL && str != NULL, RET_BAD_PARAMS);
  str_clear(str);

  node = conf_node_get_first_child(doc->root);
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);

  return conf_doc_save_yaml_node(node, str, 0);
}

static conf_doc_t* conf_doc_load_yaml_reader(data_reader_t* reader) {
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
    doc = conf_doc_load_yaml(data);
  }
  TKMEM_FREE(data);

  return doc;
}

static ret_t conf_doc_save_yaml_writer(conf_doc_t* doc, data_writer_t* writer) {
  str_t str;
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(str_init(&str, 1024) != NULL);
  goto_error_if_fail(conf_doc_save_yaml(doc, &str) == RET_OK);
  goto_error_if_fail(data_writer_write(writer, 0, str.str, str.size) == str.size);
  str_reset(&str);

  return RET_OK;

error:
  str_reset(&str);

  return RET_FAIL;
}

tk_object_t* conf_yaml_load(const char* url, bool_t create_if_not_exist) {
  return conf_obj_create(conf_doc_save_yaml_writer, conf_doc_load_yaml_reader, url,
                         create_if_not_exist);
}

ret_t conf_yaml_save_as(tk_object_t* obj, const char* url) {
  data_writer_t* writer = NULL;
  conf_doc_t* doc = conf_obj_get_doc(obj);
  return_value_if_fail(doc != NULL && url != NULL, RET_BAD_PARAMS);
  writer = data_writer_factory_create_writer(data_writer_factory(), url);
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  conf_doc_save_yaml_writer(doc, writer);
  data_writer_destroy(writer);

  return RET_OK;
}

tk_object_t* conf_yaml_create(void) {
  return conf_yaml_load(NULL, TRUE);
}

tk_object_t* conf_yaml_load_from_buff(const void* buff, uint32_t size, bool_t create_if_not_exist) {
  char url[MAX_PATH + 1] = {0};
  return_value_if_fail(buff != NULL, NULL);
  data_reader_mem_build_url(buff, size, url);

  return conf_yaml_load(url, create_if_not_exist);
}

ret_t conf_yaml_save_to_buff(tk_object_t* obj, wbuffer_t* wb) {
  char url[MAX_PATH + 1] = {0};
  return_value_if_fail(obj != NULL && wb != NULL, RET_BAD_PARAMS);

  wbuffer_init_extendable(wb);
  data_writer_wbuffer_build_url(wb, url);

  return conf_yaml_save_as(obj, url);
}
