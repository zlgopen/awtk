/**
 * File:   ui_loader_xml.h
 * Author: AWTK Develop Team
 * Brief:  default ui_loader
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/str.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "base/layout.h"
#include "tkc/tokenizer.h"
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
  str_t widget_name;

  bool_t is_include;
  bool_t is_set_name;
  bool_t is_on_start;
  bool_t is_property;
  bool_t format_error;
  uint32_t include_count;
  const char** include_name;
  props_state_t properties_state;
  const char** include_attrs;
  const char*** include_attrs_list;
  char property_name[TK_NAME_LEN * 2 + 2];
} xml_builder_t;

static XmlBuilder* builder_init(xml_builder_t* b, ui_builder_t* ui_builder);
const char window_tag[][TK_NAME_LEN] = {WIDGET_TYPE_NORMAL_WINDOW, WIDGET_TYPE_OVERLAY, WIDGET_TYPE_TOOL_BAR,
                                        WIDGET_TYPE_DIALOG, WIDGET_TYPE_POPUP, WIDGET_TYPE_SYSTEM_BAR,
                                        WIDGET_TYPE_SYSTEM_BAR_BOTTOM, WIDGET_TYPE_SPRITE, WIDGET_TYPE_KEYBOARD};

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
  return_if_fail(b->format_error == FALSE);

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
    if (b->is_include) {
      if (tk_str_eq(key, "name")) {
        if (b->widget_name.size > 0) {
          str_append(&(b->widget_name), ".");
        }
        str_append(&(b->widget_name), value);
        b->is_set_name = TRUE;
      }
    }
    if (!is_precedence_prop(tag, key)) {
      if (str_decode_xml_entity(&(b->str), value) == RET_OK) {
        str_unescape(&(b->str));
      } else {
        log_warn("decode xml entiry %s failed\n", value);
      }

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
  return_if_fail(b->format_error == FALSE);

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
  int32_t i = 0;
  int32_t k = 0;
  tokenizer_t t;
  const char* widget_name = NULL;
  const char* widget_prop = NULL;
  const char** include_attrs = NULL;
  return_if_fail(b->format_error == FALSE);

  if (b->is_include && b->is_on_start) {
    if (!b->is_set_name) {
      str_append(&(b->widget_name), ".");
    }
    for (k = 0; k < b->include_count; k++) {
      include_attrs = b->include_attrs_list[k];
      i = 0;
      while (include_attrs[i] != NULL) {
        const char* key = include_attrs[i];
        const char* value = include_attrs[i + 1];
        if (tk_str_eq(key, "filename")) {
          i += 2;
          continue;
        }
        tokenizer_init(&t, key, tk_strlen(key), "-");
        widget_name = tokenizer_next(&t);
        if (tk_str_eq(widget_name, b->widget_name.str)) {
          widget_prop = tokenizer_next(&t);
          ui_builder_on_widget_prop(b->ui_builder, widget_prop, value);
        }
        tokenizer_deinit(&t);
        i += 2;
      }
    }

    b->is_on_start = FALSE;
    b->is_set_name = FALSE;
  }

  if (b->properties_state == PROPS_STATE_START) {
    b->properties_state = PROPS_STATE_END;
    ui_builder_on_widget_prop_end(b->ui_builder);
  }
}

static void xml_loader_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  return_if_fail(b->format_error == FALSE);

  if (tk_str_eq(tag, TAG_PROPERTY)) {
    b->is_property = TRUE;
    xml_loader_on_start_property(thiz, tag, attrs);
  } else {
    b->is_property = FALSE;
    xml_loader_on_prop_end(thiz);

    if (b->is_include) {
      b->is_on_start = TRUE;
      for (int i = 0; i < ARRAY_SIZE(window_tag); i++) {
        if (tk_str_eq(tag, window_tag[i])) {
          b->format_error = TRUE;
        }
      }
    }

    xml_loader_on_start_widget(thiz, tag, attrs);
  }
}

static void xml_loader_on_end(XmlBuilder* thiz, const char* tag) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  int32_t offset;
  const char* widget_name_start = NULL;
  return_if_fail(b->format_error == FALSE);

  if (tk_str_eq(tag, TAG_PROPERTY)) {
    b->is_property = FALSE;
  } else {
    xml_loader_on_prop_end(thiz);

    if (b->is_include) {
      widget_name_start = tk_strrstr(b->widget_name.str, ".");
      if (widget_name_start == NULL) {
        widget_name_start = b->widget_name.str;
      }
      offset = widget_name_start - b->widget_name.str;
      str_remove(&(b->widget_name), offset, b->widget_name.size - offset);
    }

    ui_builder_on_widget_end(b->ui_builder);
  }

  return;
}

static void xml_loader_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  return_if_fail(b->format_error == FALSE);

  if (b->is_property) {
    assert(b->properties_state == PROPS_STATE_START);

    str_set_with_len(&(b->str), text, length);
    if (b->is_include) {
      if (tk_str_eq(b->property_name, "name")) {
        if (b->widget_name.size > 0) {
          str_append(&(b->widget_name), ".");
        }
        str_append(&(b->widget_name), b->str.str);
        b->is_set_name = TRUE;
      }
    }
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
  xml_builder_t* b = (xml_builder_t*)thiz;
  int32_t i = 0;
  uint32_t size = 0;
  str_t xml_data;
  char* data = NULL;
  const char* filename = NULL;
  bool_t include_name_loop = FALSE;
  char extname[MAX_PATH] = {0};
  char subfilename[MAX_PATH] = {0};
  char absfilename[MAX_PATH] = {0};
  char bname_normalize[MAX_PATH] = {0};
  char filename_with_ext[MAX_PATH] = {0};
  char filename_without_ext[MAX_PATH] = {0};
  XmlParser* parser = NULL;
  const asset_info_t* ui = NULL;
  ui_builder_t* builder = b->ui_builder;
  assets_manager_t* am = assets_manager();
  str_init(&xml_data, 100);
  return_if_fail(b->format_error == FALSE);

  if (tk_str_eq(tag, "include")) {
    while (attrs[i] != NULL) {
      const char* key = attrs[i];
      const char* value = attrs[i + 1];
      if (tk_str_eq(key, "filename")) {
        filename = value;
        break;
      }
      i += 2;
    }
    // 格式化路径
    path_normalize(builder->name, bname_normalize, MAX_PATH);

    if (filename != NULL) {
      memcpy(filename_with_ext, filename, strlen(filename));
      // 检测参数合法性
      if (!tk_str_end_with(filename, ".xml")) {
        tk_str_append(filename_with_ext, MAX_PATH, ".xml");
      }

      // 替换路径名
      if (b->is_include) {
        path_replace_basename(subfilename, MAX_PATH, b->include_name[b->include_count - 1], filename_with_ext);
      } else {
        path_replace_basename(subfilename, MAX_PATH, bname_normalize, filename_with_ext);
      }
      path_abs_normalize(subfilename, absfilename, MAX_PATH);
      
      // 判断是否重复包含
      if (tk_str_eq(bname_normalize, absfilename)) {
        include_name_loop = TRUE;
      }
      for (i = 0; i < b->include_count; i++) {
        if (tk_str_eq(b->include_name[i], absfilename)) {
          include_name_loop = TRUE;
        }
      }

      if (include_name_loop == FALSE) {
        data = (char* )file_read(absfilename, &size);
        str_set_with_len(&xml_data, data, size);
        TKMEM_FREE(data);
      } else {
        if (am != NULL) {
          return;
        } else {
          if (b->include_count > 0) {
            log_warn("!!!File name = %s error include file %s\n", b->include_name[b->include_count - 1], filename);
          } else {
            log_warn("!!!File name = %s error include file %s\n", bname_normalize, filename);
          }
          exit(EXIT_FAILURE);
        }
      }

      if (xml_data.size != 0) {
        xml_builder_t b_include;
        parser = xml_parser_create();
        xml_parser_set_builder(parser, builder_init(&b_include, b->ui_builder));
        b_include.is_include = TRUE;
        b_include.format_error = FALSE;
        b_include.include_attrs = attrs;
        b_include.properties_state = b->properties_state;
        b_include.is_set_name = b->is_set_name;
        b_include.is_on_start = b->is_on_start;
        str_set(&(b_include.widget_name), b->widget_name.str);
        b_include.include_name = (const char**)TKMEM_ZALLOCN(char*, b->include_count + 1);
        b_include.include_attrs_list = (const char***)TKMEM_ZALLOCN(char**, b->include_count + 1);
        for (i = 0; i < b->include_count; i++) {
          b_include.include_name[i] = b->include_name[i];
          b_include.include_attrs_list[i] = b->include_attrs_list[i];
        }
        b_include.include_name[b->include_count] = absfilename;
        b_include.include_attrs_list[b->include_count] = attrs;
        b_include.include_count = b->include_count + 1;
        xml_parser_parse(parser, xml_data.str, xml_data.size);
        // 判断被包含的路径是否为非组件文件
        if (am == NULL && b_include.format_error) {
          if (b_include.include_count > 1) {
            log_warn("!!!File name = %s error include file %s\n", b->include_name[b->include_count - 1], filename);  
          } else {
            log_warn("!!!File name = %s error include file %s\n", bname_normalize, filename);  
          }
          exit(EXIT_FAILURE);
        }
        b->properties_state = b_include.properties_state;
        b_include.is_include = FALSE;
        xml_parser_destroy(parser);
        str_reset(&(b_include.str));
        str_reset(&(b_include.widget_name));
        TKMEM_FREE(b_include.include_name);
        TKMEM_FREE(b_include.include_attrs_list);
      } else {
        log_warn("!!!File is empty\n");
      }

      if (am != NULL && ui != NULL) {
        assets_manager_unref(am, ui);
      }
      str_reset(&xml_data);
    }
  }

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
  b->format_error = FALSE;
  str_init(&(b->str), 100);
  str_init(&(b->widget_name), 100);

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
  str_reset(&(b.widget_name));

  return RET_OK;
}

static const ui_loader_t s_xml_ui_loader = {.load = ui_loader_load_xml};

ui_loader_t* xml_ui_loader() {
  return (ui_loader_t*)&s_xml_ui_loader;
}
