/**
 * File:   conf_xml.c
 * Author: AWTK Develop Team
 * Brief:  xml 
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-05-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "xml/xml_parser.h"
#include "conf_io/conf_xml.h"
#include "tkc/data_reader_mem.h"
#include "tkc/data_writer_wbuffer.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"

typedef struct _xml_builter_t {
  XmlBuilder builder;
  conf_doc_t* doc;
  conf_node_t* node;
  uint32_t level;
  str_t str;
} xml_builter_t;

static void my_xml_builder_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  uint32_t i = 0;
  xml_builter_t* b = (xml_builter_t*)thiz;
  str_t* str = &(b->str);
  conf_node_t* node = conf_doc_create_node(b->doc, tag);
  return_if_fail(node != NULL);

  conf_doc_append_child(b->doc, b->node, node);
  node->value_type = CONF_NODE_VALUE_NODE;
  b->node = node;
  b->level++;

  while (attrs[i]) {
    value_t v;
    const char* name = attrs[i];
    const char* value = attrs[i + 1];

    str_clear(str);
    str_decode_xml_entity(str, value);
    node = conf_doc_create_node(b->doc, name);
    return_if_fail(node != NULL);

    conf_doc_append_child(b->doc, b->node, node);
    value_set_str(&v, str->str);
    conf_node_set_value(node, &v);

    i += 2;
  }

  return;
}

static void my_xml_builder_on_end(XmlBuilder* thiz, const char* tag) {
  xml_builter_t* b = (xml_builter_t*)thiz;

  if (b->node != NULL) {
    b->node = b->node->parent;
  }
  b->level--;

  return;
}

static void my_xml_builder_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  log_debug("%d:%d %s\n", line, row, message);
}

static void my_xml_builder_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  xml_builter_t* b = (xml_builter_t*)thiz;
  str_t* str = &(b->str);
  if (text != NULL && length > 0) {
    str_decode_xml_entity(str, text);
    str_trim(str, "\r\n\t ");
    if (str->size > 0) {
      value_t v;
      conf_node_t* node = conf_doc_create_node(b->doc, CONF_XML_TEXT);
      return_if_fail(node != NULL);

      conf_doc_append_child(b->doc, b->node, node);
      value_set_str(&v, str->str);
      conf_node_set_value(node, &v);
    }
  }

  return;
}

static void my_xml_builder_destroy(XmlBuilder* thiz) {
  xml_builter_t* b = (xml_builter_t*)thiz;
  str_reset(&(b->str));
  return;
}

static XmlBuilder* builder_init(xml_builter_t* b, conf_doc_t* doc) {
  memset(b, 0x00, sizeof(xml_builter_t));

  b->builder.on_start = my_xml_builder_on_start;
  b->builder.on_end = my_xml_builder_on_end;
  b->builder.on_text = my_xml_builder_on_text;
  b->builder.on_error = my_xml_builder_on_error;
  b->builder.destroy = my_xml_builder_destroy;

  b->level = 0;
  b->doc = doc;
  b->node = doc->root;
  str_init(&(b->str), 100);

  return (XmlBuilder*)b;
}

static ret_t builder_deinit(xml_builter_t* b) {
  return_value_if_fail(b != NULL, RET_BAD_PARAMS);
  str_reset(&(b->str));
  return RET_OK;
}

conf_doc_t* conf_doc_load_xml(const char* data) {
  conf_doc_t* doc = NULL;
  return_value_if_fail(data != NULL, NULL);

  doc = conf_doc_create(100);
  return_value_if_fail(doc != NULL, NULL);

  doc->max_deep_level = 20;
  doc->root = conf_doc_create_node(doc, CONF_NODE_ROOT_NAME);
  if (doc->root == NULL) {
    TKMEM_FREE(doc);
    return NULL;
  } else {
    xml_builter_t b;
    XmlParser* p = xml_parser_create();
    xml_parser_set_builder(p, builder_init(&b, doc));
    xml_parser_parse(p, data, strlen(data));
    xml_parser_destroy(p);
    builder_deinit(&b);
  }

  return doc;
}

static ret_t conf_doc_save_prop(conf_node_t* node, str_t* str) {
  value_t v;
  char buff[64] = {0};
  const char* key = conf_node_get_name(node);

  conf_node_get_value(node, &v);

  return_value_if_fail(str_append_more(str, " ", key, "=\"", NULL) == RET_OK, RET_OOM);
  return_value_if_fail(
      str_encode_xml_entity(str, value_str_ex(&v, buff, sizeof(buff) - 1)) == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, "\"") == RET_OK, RET_OOM);

  return RET_OK;
}

static ret_t conf_doc_save_xml_node(conf_doc_t* doc, str_t* str, conf_node_t* node,
                                    uint32_t level) {
  uint32_t size = 0;
  const char* name = conf_node_get_name(node);
  conf_node_t* text = NULL;
  conf_node_t* iter = conf_node_get_first_child(node);

  str_append_n_chars(str, ' ', level * 2);
  str_append_format(str, strlen(name) + 10, "<%s", name);

  while (iter != NULL) {
    if (iter->value_type != CONF_NODE_VALUE_NODE && iter->value_type != CONF_NODE_VALUE_NONE) {
      const char* key = conf_node_get_name(iter);
      if (!tk_str_eq(key, CONF_XML_TEXT)) {
        conf_doc_save_prop(iter, str);
      } else {
        text = iter;
      }
    }
    iter = iter->next;
  }

  if (text == NULL) {
    str_append(str, ">\n");
  } else {
    str_append(str, ">");
  }
  size = str->size;

  iter = conf_node_get_first_child(node);
  while (iter != NULL) {
    if (iter->value_type == CONF_NODE_VALUE_NODE) {
      conf_doc_save_xml_node(doc, str, iter, level + 1);
    }
    iter = iter->next;
  }

  if (text != NULL) {
    value_t v;
    if (conf_node_get_value(text, &v) == RET_OK) {
      const char* p = value_str(&v);
      if (tk_str_start_with(p, "<![CDATA[") && tk_str_end_with(p, "]]>")) {
        return_value_if_fail(str_append(str, p) == RET_OK, RET_OOM);
      } else {
        return_value_if_fail(str_encode_xml_entity(str, p) == RET_OK, RET_OOM);
      }
    }
  }

  if (size == str->size) {
    if (text == NULL) {
      str->size -= 2;
    } else {
      str->size -= 1;
    }
    str_append(str, "/>\n");
  } else {
    if (text == NULL) {
      str_append_n_chars(str, ' ', level * 2);
    }
    str_append_format(str, strlen(name) + 10, "</%s>\n", name);
  }

  return RET_OK;
}

ret_t conf_doc_save_xml(conf_doc_t* doc, str_t* str) {
  conf_node_t* iter = conf_node_get_first_child(doc->root);
  return_value_if_fail(doc != NULL && str != NULL, RET_BAD_PARAMS);

  str_clear(str);
  while (iter != NULL) {
    if (conf_doc_save_xml_node(doc, str, iter, 0) != RET_OK) {
      return RET_OOM;
    }
    iter = iter->next;
  }

  return RET_OK;
}

static conf_doc_t* conf_doc_load_xml_reader(data_reader_t* reader) {
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
    doc = conf_doc_load_xml(data);
  }
  TKMEM_FREE(data);

  return doc;
}

static ret_t conf_doc_save_xml_writer(conf_doc_t* doc, data_writer_t* writer) {
  str_t str;
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(str_init(&str, 1024) != NULL);
  goto_error_if_fail(conf_doc_save_xml(doc, &str) == RET_OK);
  goto_error_if_fail(data_writer_write(writer, 0, str.str, str.size) == str.size);
  str_reset(&str);

  return RET_OK;

error:
  str_reset(&str);

  return RET_FAIL;
}

tk_object_t* conf_xml_load(const char* url, bool_t create_if_not_exist) {
  return conf_obj_create(conf_doc_save_xml_writer, conf_doc_load_xml_reader, url,
                         create_if_not_exist);
}

ret_t conf_xml_save_as(tk_object_t* obj, const char* url) {
  data_writer_t* writer = NULL;
  conf_doc_t* doc = conf_obj_get_doc(obj);
  return_value_if_fail(doc != NULL && url != NULL, RET_BAD_PARAMS);
  writer = data_writer_factory_create_writer(data_writer_factory(), url);
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  conf_doc_save_xml_writer(doc, writer);
  data_writer_destroy(writer);

  return RET_OK;
}

tk_object_t* conf_xml_create(void) {
  return conf_xml_load(NULL, TRUE);
}

tk_object_t* conf_xml_load_from_buff(const void* buff, uint32_t size, bool_t create_if_not_exist) {
  char url[MAX_PATH+1] = {0};
  return_value_if_fail(buff != NULL, NULL);
  data_reader_mem_build_url(buff, size, url);

  return conf_xml_load(url, create_if_not_exist);
}

ret_t conf_xml_save_to_buff(tk_object_t* obj, wbuffer_t* wb) {
  char url[MAX_PATH + 1] = {0};
  return_value_if_fail(obj != NULL && wb != NULL, RET_BAD_PARAMS);

  wbuffer_init_extendable(wb);
  data_writer_wbuffer_build_url(wb, url);

  return conf_xml_save_as(obj, url);
}
