/**
 * File:   ui_serializer.c
 * Author: AWTK Develop Team
 * Brief:  ui_serializer
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
 * 2018-07-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "ui_loader/ui_serializer.h"

ret_t ui_widget_serialize_prop(ui_builder_t* writer, const char* name, value_t* value) {
  str_t* str = &(((ui_xml_writer_t*)writer)->temp);

  if (tk_str_eq(name, WIDGET_PROP_ENABLE) && value_bool(value)) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPACITY) && value_int(value) == 0xff) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VISIBLE) && value_bool(value)) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SENSITIVE) && value_bool(value)) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FLOATING) && !value_bool(value)) {
    return RET_OK;
  }

  return_value_if_fail(str_from_value(str, value) == RET_OK, RET_OOM);

  if (tk_str_eq(name, WIDGET_PROP_TEXT) && str->size == 0) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_NAME) && str->size == 0) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TR_TEXT) && str->size == 0) {
    return RET_OK;
  }

  return ui_builder_on_widget_prop(writer, name, str->str);
}

static ret_t ui_widget_serialize_props(ui_builder_t* writer, widget_t* widget,
                                       const char* const* properties) {
  value_t v;
  value_t defv;
  uint32_t i = 0;
  for (i = 0; properties[i] != NULL; i++) {
    const char* prop = properties[i];
    if (widget_get_prop(widget, prop, &v) == RET_OK) {
      if (widget_get_prop_default_value(widget, prop, &defv) == RET_OK) {
        if (v.type == VALUE_TYPE_STRING && (v.value.str == NULL || v.value.str[0] == '\0')) {
          continue;
        }

        if (v.type == VALUE_TYPE_WSTRING && (v.value.str == NULL || v.value.wstr[0] == '\0')) {
          continue;
        }

        if (value_equal(&v, &defv)) {
          log_debug("skip default value %s\n", prop);
          continue;
        }
      }

      ui_widget_serialize_prop(writer, prop, &v);
    }
  }

  return RET_OK;
}

ret_t ui_widget_serialize(ui_builder_t* writer, widget_t* widget) {
  widget_desc_t desc;

  if (widget->auto_created) {
    return RET_OK;
  }

  memset(&desc, 0x00, sizeof(desc));
  tk_strncpy(desc.type, widget->vt->type, TK_NAME_LEN);

  desc.layout.x = widget->x;
  desc.layout.y = widget->y;
  desc.layout.w = widget->w;
  desc.layout.h = widget->h;

  ui_builder_on_widget_start(writer, &desc);

  ui_widget_serialize_props(writer, widget, widget_get_persistent_props());
  if (widget->vt->clone_properties || widget->vt->persistent_properties) {
    const char* const* properties = widget->vt->persistent_properties;
    if (properties == NULL) {
      properties = widget->vt->clone_properties;
    }
    ui_widget_serialize_props(writer, widget, properties);
  }

  ui_builder_on_widget_prop_end(writer);
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  ui_widget_serialize(writer, iter);
  WIDGET_FOR_EACH_CHILD_END();
  ui_builder_on_widget_end(writer);

  return RET_OK;
}

ret_t widget_to_xml(widget_t* widget, str_t* str) {
  ui_xml_writer_t ui_xml_writer;
  return_value_if_fail(widget != NULL && str != NULL, RET_BAD_PARAMS);

  return ui_widget_serialize(ui_xml_writer_init(&ui_xml_writer, str), widget);
}
