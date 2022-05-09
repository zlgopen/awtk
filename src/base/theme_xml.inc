/**
 * File:   theme_xml.c
 * Author: AWTK Develop Team
 * Brief:  load theme data from xml
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-05-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "theme_default.h"
#include "xml/xml_parser.h"

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/buffer.h"
#include "tkc/object_default.h"

#include "base/style.h"

#define TAG_PROPERTY "property"

typedef struct _xml_builder_t {
  XmlBuilder builder;
  tk_object_t* widget_style;
  tk_object_t* share_style;
  tk_object_t* state_style;

  uint16_t level;
  char* style_name;
  char* widget_type;
  char* state_name;

  wbuffer_t wbuffer;
  bool_t is_property;
  char property_name[TK_NAME_LEN * 2 + 2];

  darray_t styles;
} xml_builder_t;

static void xml_gen_style(xml_builder_t* b, tk_object_t* s, const char** attrs) {
  value_t v;
  uint32_t i = 0;

  value_set_int(&v, 0);
  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];
    ENSURE(style_normalize_value(name, value, &v) == RET_OK);

    if (strcmp(name, "name") != 0) {
      tk_object_set_prop(s, name, &v);
    }
    value_reset(&v);

    i += 2;
  }
}

static void xml_gen_on_widget(xml_builder_t* b, const char* tag, const char** attrs) {
  object_default_clear_props(b->widget_style);
  xml_gen_style(b, b->widget_style, attrs);
  b->widget_type = tk_str_copy(b->widget_type, tag);
  b->style_name = tk_str_copy(b->style_name, TK_DEFAULT_STYLE);
}

static void xml_gen_on_style(xml_builder_t* b, const char* tag, const char** attrs) {
  uint32_t i = 0;

  b->style_name = tk_str_copy(b->style_name, TK_DEFAULT_STYLE);

  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];

    if (strcmp(name, "name") == 0) {
      b->style_name = tk_str_copy(b->style_name, value);
    }

    i += 2;
  }

  object_default_clear_props(b->share_style);
  xml_gen_style(b, b->share_style, attrs);
}

static void xml_gen_on_state(xml_builder_t* b, const char* tag, const char** attrs) {
  b->state_name = tk_str_copy(b->state_name, tag);
  object_default_clear_props(b->state_style);
  xml_gen_style(b, b->state_style, attrs);

  return;
}

static void xml_gen_on_start_property(XmlBuilder* thiz, const char* tag, const char** attrs) {
  uint32_t i = 0;
  xml_builder_t* b = (xml_builder_t*)thiz;

  b->property_name[0] = '\0';
  while (attrs[i] != NULL) {
    const char* key = attrs[i];
    const char* value = attrs[i + 1];
    if (tk_str_eq(key, "name")) {
      tk_strncpy(b->property_name, value, TK_NAME_LEN * 2 + 1);
      break;
    }
  }
}

static void xml_gen_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (tk_str_eq(tag, TAG_PROPERTY)) {
    b->is_property = TRUE;
    xml_gen_on_start_property(thiz, tag, attrs);
  } else {
    b->is_property = FALSE;
    if (b->level == 0) {
      xml_gen_on_widget(b, tag, attrs);
    } else if (b->level == 1) {
      xml_gen_on_style(b, tag, attrs);
    } else {
      xml_gen_on_state(b, tag, attrs);
    }
    b->level++;
  }

  return;
}

static void xml_gen_on_widget_end(XmlBuilder* thiz) {
}

static void xml_gen_on_style_end(XmlBuilder* thiz) {
}

static void xml_gen_on_state_end(XmlBuilder* thiz) {
  theme_item_t item;
  tk_object_t* style = NULL;
  xml_builder_t* b = (xml_builder_t*)thiz;

  memset(&item, 0x00, sizeof(item));
  style = tk_object_clone(b->widget_style);
  return_if_fail(style != NULL);
  tk_object_copy_props(style, b->share_style, TRUE);
  tk_object_copy_props(style, b->state_style, TRUE);

  item.offset = 0;
  tk_strncpy_s(item.name, sizeof(item.name) - 1, b->style_name, tk_strlen(b->style_name));
  tk_strncpy_s(item.state, sizeof(item.state) - 1, b->state_name, tk_strlen(b->state_name));
  tk_strncpy_s(item.widget_type, sizeof(item.widget_type) - 1, b->widget_type,
               tk_strlen(b->widget_type));

  if (darray_push(&(b->styles), style) == RET_OK) {
    ENSURE(wbuffer_write_binary(&(b->wbuffer), &item, sizeof(item)) == RET_OK);
  } else {
    TK_OBJECT_UNREF(style);
  }
}

static void xml_gen_on_end(XmlBuilder* thiz, const char* tag) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (tk_str_eq(tag, TAG_PROPERTY)) {
    b->is_property = FALSE;
  } else {
    if (b->level == 1) {
      xml_gen_on_widget_end(thiz);
    } else if (b->level == 2) {
      xml_gen_on_style_end(thiz);
    } else if (b->level == 3) {
      xml_gen_on_state_end(thiz);
    }
    b->level--;
  }

  return;
}

static void xml_gen_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (b->is_property) {
    tk_object_t* s = NULL;

    if (b->level == 1) {
      s = b->widget_style;
    } else if (b->level == 2) {
      s = b->share_style;
    } else if (b->level == 3) {
      s = b->state_style;
    }

    if (s != NULL) {
      value_t v;
      ENSURE(style_normalize_value(b->property_name, text, &v) == RET_OK);
      tk_object_set_prop(s, b->property_name, &v);
      value_reset(&v);
    }
  }

  return;
}

static void xml_gen_on_comment(XmlBuilder* thiz, const char* text, size_t length) {
  (void)thiz;
  (void)text;
  (void)length;

  return;
}

static void xml_gen_on_pi(XmlBuilder* thiz, const char* tag, const char** attrs) {
  (void)thiz;
  (void)tag;
  (void)attrs;
  return;
}

static void xml_gen_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  (void)thiz;
  log_warn("parse error: %d:%d %s\n", line, row, message);
  return;
}

static void xml_gen_destroy(XmlBuilder* thiz) {
  (void)thiz;
  return;
}

static XmlBuilder* builder_init(xml_builder_t* b, uint32_t prealloc_size) {
  theme_header_t header;
  memset(b, 0x00, sizeof(xml_builder_t));

  b->builder.on_start = xml_gen_on_start;
  b->builder.on_end = xml_gen_on_end;
  b->builder.on_text = xml_gen_on_text;
  b->builder.on_error = xml_gen_on_error;
  b->builder.on_comment = xml_gen_on_comment;
  b->builder.on_pi = xml_gen_on_pi;
  b->builder.destroy = xml_gen_destroy;

  b->level = 0;
  b->style_name = NULL;
  b->widget_type = NULL;
  b->state_name = NULL;
  b->is_property = FALSE;
  b->widget_style = object_default_create_ex(FALSE);
  b->share_style = object_default_create_ex(FALSE);
  b->state_style = object_default_create_ex(FALSE);

  wbuffer_init_extendable(&(b->wbuffer));
  return_value_if_fail(wbuffer_extend_capacity(&(b->wbuffer), prealloc_size + 512) == RET_OK, NULL);

  memset(&header, 0x00, sizeof(header));
  wbuffer_write_binary(&(b->wbuffer), &header, sizeof(header));
  darray_init(&(b->styles), 10, (tk_destroy_t)tk_object_unref, NULL);

  return &(b->builder);
}

static ret_t write_prop(wbuffer_t* wbuffer, const char* name, value_t* v) {
  style_name_value_header_t nv;

  nv.type = v->type;
  nv.name_size = tk_strlen(name) + 1;

  switch (v->type) {
    case VALUE_TYPE_INT32:
    case VALUE_TYPE_UINT32: {
      nv.value_size = sizeof(int32_t);
      break;
    }
    case VALUE_TYPE_STRING: {
      nv.value_size = tk_strlen(value_str(v)) + 1;
      break;
    }
    case VALUE_TYPE_BINARY: {
      binary_data_t* data = value_binary_data(v);
      nv.value_size = data->size;
      break;
    }
    case VALUE_TYPE_GRADIENT: {
      binary_data_t* data = value_gradient(v);
      nv.value_size = data->size;
      break;
    }
    default: {
      assert(!"invalid type");
    }
  }

  wbuffer_write_binary(wbuffer, &nv, sizeof(nv));
  wbuffer_write_binary(wbuffer, name, tk_strlen(name) + 1);

  switch (v->type) {
    case VALUE_TYPE_INT32: {
      wbuffer_write_int32(wbuffer, value_int(v));
      break;
    }
    case VALUE_TYPE_UINT32: {
      wbuffer_write_uint32(wbuffer, value_uint32(v));
      break;
    }
    case VALUE_TYPE_STRING: {
      wbuffer_write_binary(wbuffer, value_str(v), tk_strlen(value_str(v)) + 1);
      break;
    }
    case VALUE_TYPE_BINARY: {
      binary_data_t* data = value_binary_data(v);
      wbuffer_write_binary(wbuffer, data->data, data->size);
      break;
    }
    case VALUE_TYPE_GRADIENT: {
      binary_data_t* data = value_gradient(v);
      wbuffer_write_binary(wbuffer, data->data, data->size);
      break;
    }
    default: {
      assert(!"invalid type");
    }
  }

  return RET_OK;
}

static ret_t write_int_props(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  wbuffer_t* wb = (wbuffer_t*)ctx;
  value_t* v = &(nv->value);
  if (v->type != VALUE_TYPE_INT32) {
    return RET_OK;
  }

  return write_prop(wb, nv->name, v);
}

static ret_t write_uint_props(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  wbuffer_t* wb = (wbuffer_t*)ctx;
  value_t* v = &(nv->value);
  if (v->type != VALUE_TYPE_UINT32) {
    return RET_OK;
  }

  return write_prop(wb, nv->name, v);
}

static ret_t write_string_props(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  wbuffer_t* wb = (wbuffer_t*)ctx;
  value_t* v = &(nv->value);
  if (v->type != VALUE_TYPE_STRING && v->type !=  VALUE_TYPE_GRADIENT) {
    return RET_OK;
  }

  return write_prop(wb, nv->name, v);
}

static ret_t write_binary_props(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  wbuffer_t* wb = (wbuffer_t*)ctx;
  value_t* v = &(nv->value);
  if (v->type != VALUE_TYPE_BINARY) {
    return RET_OK;
  }

  return write_prop(wb, nv->name, v);
}

static ret_t builder_gen(xml_builder_t* b) {
  uint32_t i = 0;
  uint8_t* p = NULL;
  theme_item_t* item = NULL;
  uint32_t n = b->styles.size;
  wbuffer_t* wb = &(b->wbuffer);
  theme_header_t* header = (theme_header_t*)(wb->data);

  header->magic = THEME_MAGIC;
  header->version = 0;
  header->nr = n;

  for (i = 0; i < n; i++) {
    tk_object_t* s = (tk_object_t*)darray_get(&(b->styles), i);
    uint32_t size = tk_object_get_prop_uint32(s, TK_OBJECT_PROP_SIZE, 0);

    p = wb->data + sizeof(theme_header_t) + i * sizeof(theme_item_t);
    item = (theme_item_t*)p;
    item->offset = wb->cursor;

    wbuffer_write_uint32(wb, size);
    tk_object_foreach_prop(s, write_int_props, wb);
    tk_object_foreach_prop(s, write_uint_props, wb);
    tk_object_foreach_prop(s, write_string_props, wb);
    tk_object_foreach_prop(s, write_binary_props, wb);
  }

  return RET_OK;
}

static ret_t builder_deinit(xml_builder_t* b) {
  TKMEM_FREE(b->state_name);
  TKMEM_FREE(b->widget_type);
  TKMEM_FREE(b->style_name);
  TK_OBJECT_UNREF(b->share_style);
  TK_OBJECT_UNREF(b->widget_style);
  TK_OBJECT_UNREF(b->state_style);

  wbuffer_deinit(&(b->wbuffer));
  darray_deinit(&(b->styles));

  return RET_OK;
}

uint8_t* theme_xml_gen(const char* xml, uint32_t* size) {
  xml_builder_t b;
  uint8_t* data = NULL;
  XmlParser* parser = NULL;
  uint32_t xml_len = tk_strlen(xml);
  XmlBuilder* xb = builder_init(&b, xml_len);
  return_value_if_fail(xb != NULL, NULL);
  return_value_if_fail(xml != NULL && size != NULL, NULL);

  parser = xml_parser_create();
  return_value_if_fail(parser != NULL, NULL);

  xml_parser_set_builder(parser, xb);
  xml_parser_parse(parser, xml, xml_len);
  builder_gen(&b);

  data = b.wbuffer.data;
  *size = b.wbuffer.cursor;
  b.wbuffer.data = NULL;

  builder_deinit(&b);
  xml_parser_destroy(parser);

  return data;
}

theme_t* theme_xml_create(const char* xml) {
  uint32_t size = 0;
  uint8_t* data = NULL;
  return_value_if_fail(xml != NULL, NULL);
  data = theme_xml_gen(xml, &size);
  return_value_if_fail(data != NULL, NULL);

  return theme_default_create_ex(data, TRUE);
}
