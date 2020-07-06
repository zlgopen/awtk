/**
 * File:   ubjson.c
 * Author: AWTK Develop Team
 * Brief:  ubjson 
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
#include "tkc/buffer.h"
#include "ubjson/ubjson_parser.h"
#include "conf_io/conf_ubjson.h"

static ret_t conf_ubjson_save_node(conf_node_t* node, ubjson_writer_t* writer);
static ret_t conf_ubjson_save_node_value(conf_node_t* node, ubjson_writer_t* writer);
static ret_t conf_ubjson_save_node_children(conf_node_t* node, ubjson_writer_t* writer);

static ret_t conf_ubjson_save_node_value_object(conf_node_t* node, ubjson_writer_t* writer) {
  return conf_ubjson_save_node_children(node, writer);
}

static ret_t conf_ubjson_save_node_value_array(conf_node_t* node, ubjson_writer_t* writer) {
  return conf_ubjson_save_node_children(node, writer);
}

static ret_t conf_ubjson_save_node_value_simple(conf_node_t* node, ubjson_writer_t* writer) {
  value_t v;
  return_value_if_fail(conf_node_get_value(node, &v) == RET_OK, RET_BAD_PARAMS);

  if (v.type == VALUE_TYPE_BOOL) {
    if (value_bool(&v)) {
      return ubjson_writer_write_true(writer);
    } else {
      return ubjson_writer_write_false(writer);
    }
  } else if (v.type == VALUE_TYPE_STRING) {
    return ubjson_writer_write_str(writer, value_str(&v));
  } else {
    return ubjson_writer_write_int(writer, value_int(&v));
  }
}

static ret_t conf_ubjson_save_node_value(conf_node_t* node, ubjson_writer_t* writer) {
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);
  if (node->node_type == CONF_NODE_OBJECT) {
    return conf_ubjson_save_node_value_object(node, writer);
  } else if (node->node_type == CONF_NODE_ARRAY) {
    return conf_ubjson_save_node_value_array(node, writer);
  } else {
    return conf_ubjson_save_node_value_simple(node, writer);
  }
}

static ret_t conf_ubjson_save_node(conf_node_t* node, ubjson_writer_t* writer) {
  const char* name = conf_node_get_name(node);
  return_value_if_fail(ubjson_writer_write_key(writer, name) == RET_OK, RET_OOM);

  return conf_ubjson_save_node_value(node, writer);
}

static ret_t conf_ubjson_save_node_children_object(conf_node_t* node, ubjson_writer_t* writer) {
  conf_node_t* iter = conf_node_get_first_child(node);
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_object_begin(writer) == RET_OK, RET_OOM);

  while (iter != NULL) {
    return_value_if_fail(conf_ubjson_save_node(iter, writer) == RET_OK, RET_OOM);
    iter = iter->next;
  }

  return_value_if_fail(ubjson_writer_write_object_end(writer) == RET_OK, RET_OOM);

  return RET_OK;
}

static ret_t conf_ubjson_save_node_children_array(conf_node_t* node, ubjson_writer_t* writer) {
  conf_node_t* iter = conf_node_get_first_child(node);
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_array_begin(writer) == RET_OK, RET_OOM);

  while (iter != NULL) {
    return_value_if_fail(conf_ubjson_save_node_value(iter, writer) == RET_OK, RET_OOM);
    iter = iter->next;
  }

  return_value_if_fail(ubjson_writer_write_array_end(writer) == RET_OK, RET_OOM);

  return RET_OK;
}

static ret_t conf_ubjson_save_node_children(conf_node_t* node, ubjson_writer_t* writer) {
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);

  if (node->node_type == CONF_NODE_OBJECT) {
    return conf_ubjson_save_node_children_object(node, writer);
  } else if (node->node_type == CONF_NODE_ARRAY) {
    return conf_ubjson_save_node_children_array(node, writer);
  } else {
    assert(!"invalid type");
    return RET_FAIL;
  }
}

ret_t conf_doc_save_ubjson(conf_doc_t* doc, ubjson_writer_t* writer) {
  return_value_if_fail(doc != NULL && doc->root != NULL && writer != NULL, RET_BAD_PARAMS);

  return conf_ubjson_save_node_children(doc->root, writer);
}

typedef struct _parse_ctx_t {
  conf_doc_t* doc;
  conf_node_t* node;
} parse_ctx_t;

static ret_t ubjson_conf_on_key_value(void* ctx, const char* key, value_t* v) {
  ret_t ret = RET_OK;
  parse_ctx_t* parser = (parse_ctx_t*)ctx;
  conf_node_t* current = parser->node;
  conf_node_t* node = NULL;

  if (v->type == VALUE_TYPE_TOKEN) {
    uint32_t token = value_token(v);
    if (token == UBJSON_MARKER_OBJECT_END || token == UBJSON_MARKER_ARRAY_END) {
      return_value_if_fail(current->parent != NULL, RET_BAD_PARAMS);

      parser->node = current->parent;
      return RET_OK;
    }
  }

  if (parser->node == NULL) {
    assert(key == NULL);
    assert(parser->doc->root == NULL);

    node = conf_doc_create_node(parser->doc, CONF_NODE_ROOT_NAME);
    return_value_if_fail(node != NULL, RET_OOM);
    parser->doc->root = node;
    parser->node = node;
  } else {
    node = conf_doc_create_node(parser->doc, key);
    return_value_if_fail(node != NULL, RET_OOM);
    conf_doc_append_child(parser->doc, current, node);
  }

  if (v->type == VALUE_TYPE_TOKEN) {
    uint32_t token = value_token(v);

    if (token == UBJSON_MARKER_OBJECT_BEGIN) {
      node->node_type = CONF_NODE_OBJECT;
      parser->node = node;
    } else if (token == UBJSON_MARKER_ARRAY_BEGIN) {
      node->node_type = CONF_NODE_ARRAY;
      parser->node = node;
    } else {
      assert(!"not supported");
      ret = RET_NOT_IMPL;
    }
  } else {
    node->node_type = CONF_NODE_SIMPLE;
    ret = conf_node_set_value(node, v);
  }

  return ret;
}

conf_doc_t* conf_doc_load_ubjson(const void* data, uint32_t size) {
  parse_ctx_t parser;
  return_value_if_fail(data != NULL, NULL);
  memset(&parser, 0x00, sizeof(parser));

  parser.doc = conf_doc_create(100);

  ubjson_parse((void*)data, size, ubjson_conf_on_key_value, &parser);

  return parser.doc;
}

static conf_doc_t* conf_doc_load_ubjson_reader(data_reader_t* reader) {
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
    doc = conf_doc_load_ubjson(data, rsize);
  }
  TKMEM_FREE(data);

  return doc;
}

static ret_t conf_doc_save_ubjson_writer(conf_doc_t* doc, data_writer_t* writer) {
  wbuffer_t wb;
  ubjson_writer_t ub;
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(wbuffer_init_extendable(&wb) != NULL);
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);
  goto_error_if_fail(conf_doc_save_ubjson(doc, &ub) == RET_OK);
  goto_error_if_fail(data_writer_write(writer, 0, wb.data, wb.cursor) == wb.cursor);
  wbuffer_deinit(&wb);

  return RET_OK;

error:
  wbuffer_deinit(&wb);

  return RET_FAIL;
}

object_t* conf_ubjson_load(const char* url, bool_t create_if_not_exist) {
  return conf_obj_create(conf_doc_save_ubjson_writer, conf_doc_load_ubjson_reader, url,
                         create_if_not_exist);
}
