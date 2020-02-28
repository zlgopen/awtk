/**
 * File:   ui_loader_xml.h
 * Author: AWTK Develop Team
 * Brief:  default ui_loader
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/str.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "base/layout.h"
#include "xml/xml_parser.h"
#include "ui_loader/ui_loader_xml.h"

#define TAG_PROPERTY "property"

typedef enum _props_state_t {
  PROPS_STATE_NONE = 0,
  PROPS_STATE_START,
  PROPS_STATE_END
} props_state_t;

typedef struct _xml_builder_t {
  XmlBuilder builder;
  ui_builder_t* ui_builder;
  str_t str;

  bool_t is_property;
  props_state_t properties_state;
  char property_name[TK_NAME_LEN * 2 + 2];
} xml_builder_t;

/*FIXME: it is not a good solution to hardcode*/
static bool_t is_precedence_prop(const char* tag, const char* prop) {
  if ((tk_str_eq(tag, WIDGET_TYPE_EDIT) || tk_str_eq(tag, WIDGET_TYPE_SPIN_BOX)) &&
      tk_str_eq(prop, WIDGET_PROP_INPUT_TYPE)) {
    return TRUE;
  } else if (tk_str_eq(prop, WIDGET_PROP_OPTIONS)) {
    return TRUE;
  } else if (tk_str_eq(prop, "visible_nr")) {
    return TRUE;
  }

  return FALSE;
}

static bool_t is_valid_layout_param(const char* v) {
  if (v != NULL) {
    const char* p = v;
    while (*p) {
      if (*p < '0' || *p > '9') {
        return TRUE;
      }
      p++;
    }
  }

  return FALSE;
}

static bool_t is_valid_self_layout(const char* x, const char* y, const char* w, const char* h) {
  return is_valid_layout_param(x) || is_valid_layout_param(y) || is_valid_layout_param(w) ||
         is_valid_layout_param(h);
}

static void xml_loader_on_start_widget(XmlBuilder* thiz, const char* tag, const char** attrs) {
  char c = '\0';
  uint32_t i = 0;
  const char* x = NULL;
  const char* y = NULL;
  const char* w = NULL;
  const char* h = NULL;
  widget_desc_t desc;
  const char* key = NULL;
  const char* value = NULL;
  xml_builder_t* b = (xml_builder_t*)thiz;

  memset(&desc, 0x00, sizeof(desc));

  while (attrs[i] != NULL) {
    key = attrs[i];
    value = attrs[i + 1];

    c = key[0];
    if (key[1] == '\0') {
      if (c == 'x') {
        x = (value);
      } else if (c == 'y') {
        y = (value);
      } else if (c == 'w') {
        w = (value);
      } else if (c == 'h') {
        h = (value);
      }
    }

    i += 2;
  }

  strncpy(desc.type, tag, TK_NAME_LEN);
  if (x != NULL) desc.layout.x = tk_atoi(x);
  if (y != NULL) desc.layout.y = tk_atoi(y);
  if (w != NULL) desc.layout.w = tk_atoi(w);
  if (h != NULL) desc.layout.h = tk_atoi(h);
  ui_builder_on_widget_start(b->ui_builder, &desc);

  if (is_valid_self_layout(x, y, w, h)) {
    str_t* s = &(b->str);
    s->size = 0;
    str_append(s, "default(");
    if (x != NULL) {
      str_append(s, "x=");
      str_append(s, x);
      str_append(s, ",");
    }

    if (y != NULL) {
      str_append(s, "y=");
      str_append(s, y);
      str_append(s, ",");
    }

    if (w != NULL) {
      str_append(s, "w=");
      str_append(s, w);
      str_append(s, ",");
    }

    if (h != NULL) {
      str_append(s, "h=");
      str_append(s, h);
      str_append(s, ",");
    }

    while (s->size > 0 && s->str[s->size - 1] == ',') {
      s->size--;
    }

    str_append(s, ")");
    ui_builder_on_widget_prop(b->ui_builder, WIDGET_PROP_SELF_LAYOUT, b->str.str);
  }

  /*set highest priority props*/
  i = 0;
  while (attrs[i] != NULL) {
    key = attrs[i];
    value = attrs[i + 1];
    c = key[0];

    if (key[1] == '\0') {
      if (c == 'x' || c == 'y' || c == 'w' || c == 'h') {
        i += 2;
        continue;
      }
    }

    if (is_precedence_prop(tag, key)) {
      ENSURE(str_decode_xml_entity(&(b->str), value) == RET_OK);
      str_unescape(&(b->str));
      ui_builder_on_widget_prop(b->ui_builder, key, b->str.str);
    }

    i += 2;
  }

  /*set normal priority props*/
  i = 0;
  while (attrs[i] != NULL) {
    key = attrs[i];
    value = attrs[i + 1];
    c = key[0];

    if (key[1] == '\0') {
      if (c == 'x' || c == 'y' || c == 'w' || c == 'h') {
        i += 2;
        continue;
      }
    }

    if (!is_precedence_prop(tag, key)) {
      ENSURE(str_decode_xml_entity(&(b->str), value) == RET_OK);
      str_unescape(&(b->str));
      ui_builder_on_widget_prop(b->ui_builder, key, b->str.str);
    }

    i += 2;
  }

  b->properties_state = PROPS_STATE_START;

  return;
}

static void xml_loader_on_start_property(XmlBuilder* thiz, const char* tag, const char** attrs) {
  uint32_t i = 0;
  xml_builder_t* b = (xml_builder_t*)thiz;

  while (attrs[i] != NULL) {
    const char* key = attrs[i];
    const char* value = attrs[i + 1];
    if (tk_str_eq(key, "name")) {
      tk_strncpy(b->property_name, value, TK_NAME_LEN * 2 + 1);
      break;
    }
  }
}

static void xml_loader_on_prop_end(XmlBuilder* thiz) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (b->properties_state == PROPS_STATE_START) {
    b->properties_state = PROPS_STATE_END;
    ui_builder_on_widget_prop_end(b->ui_builder);
  }
}

static void xml_loader_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (tk_str_eq(tag, TAG_PROPERTY)) {
    b->is_property = TRUE;
    xml_loader_on_start_property(thiz, tag, attrs);
  } else {
    b->is_property = FALSE;

    xml_loader_on_prop_end(thiz);
    xml_loader_on_start_widget(thiz, tag, attrs);
  }
}

static void xml_loader_on_end(XmlBuilder* thiz, const char* tag) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  (void)thiz;
  (void)tag;
  if (tk_str_eq(tag, TAG_PROPERTY)) {
    b->is_property = FALSE;
  } else {
    xml_loader_on_prop_end(thiz);
    ui_builder_on_widget_end(b->ui_builder);
  }

  return;
}

static void xml_loader_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (b->is_property) {
    assert(b->properties_state == PROPS_STATE_START);

    str_set_with_len(&(b->str), text, length);
    ui_builder_on_widget_prop(b->ui_builder, b->property_name, b->str.str);
  }

  return;
}

static void xml_loader_on_comment(XmlBuilder* thiz, const char* text, size_t length) {
  (void)thiz;
  (void)text;
  (void)length;

  return;
}

static void xml_loader_on_pi(XmlBuilder* thiz, const char* tag, const char** attrs) {
  (void)thiz;
  (void)tag;
  (void)attrs;
  return;
}

static void xml_loader_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  (void)thiz;
  log_debug("parse error: %d:%d %s\n", line, row, message);
  return;
}

static void xml_loader_destroy(XmlBuilder* thiz) {
  (void)thiz;
  return;
}

static XmlBuilder* builder_init(xml_builder_t* b, ui_builder_t* ui_builder) {
  memset(b, 0x00, sizeof(xml_builder_t));

  b->builder.on_start = xml_loader_on_start;
  b->builder.on_end = xml_loader_on_end;
  b->builder.on_text = xml_loader_on_text;
  b->builder.on_error = xml_loader_on_error;
  b->builder.on_comment = xml_loader_on_comment;
  b->builder.on_pi = xml_loader_on_pi;
  b->builder.destroy = xml_loader_destroy;
  b->ui_builder = ui_builder;
  str_init(&(b->str), 100);

  return &(b->builder);
}

ret_t ui_loader_load_xml(ui_loader_t* loader, const uint8_t* data, uint32_t size,
                         ui_builder_t* ui_builder) {
  xml_builder_t b;
  XmlParser* parser = NULL;
  return_value_if_fail(loader != NULL && data != NULL && ui_builder != NULL, RET_BAD_PARAMS);

  parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(&b, ui_builder));
  ui_builder_on_start(ui_builder);
  xml_parser_parse(parser, (const char*)data, size);
  ui_builder_on_end(ui_builder);
  xml_parser_destroy(parser);
  str_reset(&(b.str));

  return RET_OK;
}

static const ui_loader_t s_xml_ui_loader = {.load = ui_loader_load_xml};

ui_loader_t* xml_ui_loader() {
  return (ui_loader_t*)&s_xml_ui_loader;
}
